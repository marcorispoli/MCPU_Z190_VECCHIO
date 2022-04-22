#ifndef ERRORTABLE_H
#define ERRORTABLE_H

#include <QObject>
#include <QMap>
#include <QApplication>

//! Class to handle the error items
/**
 * This class defines the structure of a system error item.
 *
 * An error item is defined with the following element:
 * + A code field: this is the unique string code identifying a given alarm;
 * The code is formatted as "Exxxxx" where xxxxx is an error number.
 * Example: E00001
 * + A Title field: it is a string representing the error title;
 * + An error description: this is a multi line string with the error description.
 * + A Pixmap UID: this is the path of the internal pixmap that will be showed with the error message
 * + An active status flag: identifies if the error is currently active;
 * + An auto expire flag: set this active error as an auto expire (one shot) error.
 * The error of this type reset itself when the error window closes.
 *
 * All the variables of the class are private so they can be
 * handled only with the public methods.
 */
class errorBlock
{
public:

    explicit errorBlock(){} //!< Default constructor

    #define _ERR_PIX_PATH "//Errors/Errors/" //!< Path in the assets where are located the error pix

    /**
     * Constructor of a given error block
     * @param string_code: is the unique error identifier in the string format "Exxxxx"
     * @param string: the error message containing the Title and description, separated with the new line character
     * @param PixmapUID: this is the internal UID (in string format) of the Pixmap that shall be assigned to the message
     */
    explicit errorBlock(QString string, QString PixmapUID){
        this->title =  errorBlock::retriveTitle(string);
        this->description = errorBlock::retriveDescription(string);
        this->PixmapUID = _ERR_PIX_PATH + PixmapUID;
        this->active = false;
    }


    //! Get the title element of the error block
    _inline QString getTitle(void){
        return title;
    }

    //! Get the description element of the error block
    _inline QString getDescription(void){
        return description;
    }

    //! Get the Pixmap UID element of the error block
    _inline QString getPixmapUID(void){
        return PixmapUID;
    }

    //! Get the current activation status of the error block
    _inline bool isActive(void){
        return active;
    }

    //! Set the current activation status of the error block
    _inline void setActive(bool status){
        active = status;
    }

     //! Get the Auto-Expire status of the error block
    _inline bool isAutoExpire(void){
        return autoexpire;
    }

    //! Set the Auto-Expire status of the error block
    _inline void setAutoExpire(bool status){
        autoexpire = status;
    }

public:

    QString title;          //! Translated Error Title
    QString description;    //! Translated Error Description
    QString PixmapUID;      //! Internal UID of the related Pixmap (if there is one)
    bool    active;         //! Status of the error
    bool    autoexpire;     //! Set if the error auto expires after visualization

    /**
     * @brief getTitle returns the Title part of the translated message
     *
     * The title is identified by the first new line character
     * found in the message string
     *
     * @param string is the complete message string
     * @return the Title part of the message string
     */
    static  QString retriveTitle(QString string){

        // The first \n identifies the title block
        // If no match then the string is the title
        int i = string.indexOf('\n',0);
        if(i<0) return string;
        return string.left(i);
    };

    /**
     * @brief getDescription returns the description part of the translated message
     *
     * The Description is the string part following the title.
     *
     * @return The description string part of the translated message
     */
    static QString retriveDescription(QString string){

        // The first \n identifies the title block
        int i = string.indexOf('\n',0);
        if(i<0) return "";
        i++;
        return string.right(string.length() - i);
    };


};

class errorTable : public QObject
{
    Q_OBJECT

public:
    explicit errorTable(void){setupErrors();}
    ~errorTable(){}

signals:
    void activateErrorSgn(QString code);
    void clearErrorSgn(void);


public:

    //! Error command option flags
    typedef enum{
     _ERR_NONE          = 0,     //!< No option
     _ERR_NO_SGN        = 0x1,   //! Prevents the change signals emission
     _ERR_AUTOEXPIRE    = 0x2,   //!< The Error is activated with the auto expire flag
     _ERR_FORCE         = 0x4,  //!< Force the signal when an already set error is activated

    }_ErrOptionsType;


    typedef QMap<QString, errorBlock>::iterator errorIterator;


    /**
     * @brief Function to activate an error condition by error string identifier
     *
     * The function shall be used to activate the given error condition.
     *
     * The error can be static or can be flag as "Auto-Expire"  (_ERR_AUTOEXPIRE option).
     * In case of Auto-Expire, it can be reset with the errorTable::clearAutoexpireErrors(),
     * toghether with all the AutoExpire errors in the error list.
     *
     * If the error is activated, the signal errorTable::activateErrorSgn() is generated.
     *
     * @param errcode:  QString Type error code
     * @param opt
     * Command options:
     * + _ERR_FORCE: in case of an already active alarm, it forces the generation of the errorTable::activateErrorSgn() signal;
     * + _ERR_AUTOEXPIRE: flag the error as Auto Expiring error.
     * + _ERR_EMIT_SGN: allows the emission of the errorTable::activateErrorSgn();
     */
    void activateError(QString errcode, unsigned char opt = _ERR_NONE){

        // Find the error in the list of the current active errors
        if(activeErrors.size()){
            for(int i=0; i<activeErrors.size(); i++){
                if(activeErrors[i].key() == errcode){
                    if((opt & _ERR_FORCE) == 0) return;
                    else {
                        if(!(opt & _ERR_NO_SGN)) emit activateErrorSgn(errcode);
                        return;
                    }
                }
            }
        }

        // The error is not in the error list yet
        errorIterator i = errorList.find(errcode);
        if(i == errorList.end()) return;

        // Activate the error
        i.value().setActive(true);

        // AUTOEXPIRE option
        if(opt & _ERR_AUTOEXPIRE){
            i.value().setAutoExpire(true);
        }else {
            i.value().setAutoExpire(false);
        }

        // Set the error in the activeError list
        activeErrors.append(i);
        if(!(opt & _ERR_NO_SGN)) emit activateErrorSgn(errcode);
        return;
    }

    /**
     * @brief This function reset an error condition.
     *
     * This function shall be used to clear a current active error.
     * The error is then removed from the active error list
     *
     * In case of  the error should be actually cleared (or in case of _ERR_FORCE),\n
     * the errorTable::clearErrorSgn() is then generated.
     *
     *
     * @param errcode: this is the error code identifier string
     * @param opt: this is the option field:
     * + _ERR_FORCE: forces the generation of the errorTable::clearErrorSgn();
     * + _ERR_EMIT_SGN: allows the emission of the errorTable::clearErrorSgn();
     */
    void clearError(QString errcode, unsigned char opt = _ERR_NONE){

        // Find the error in the active error list
        for(int i=0; i< activeErrors.size(); i++){
            if(activeErrors[i].key() == errcode){

                // deactivate the error
                activeErrors[i].value().setActive(false);

                // Remove the error from the activeErrors
                activeErrors.removeAt(i);
                if(!(opt & _ERR_NO_SGN)) emit clearErrorSgn();
                return;
            }
        }

        if((opt & _ERR_FORCE) == 0) return;
        if(!(opt & _ERR_NO_SGN)) emit clearErrorSgn();
    }

    /**
     * @brief This function clears all the active errors that should
     * be flagged as auto expire.
     *
     * @param sgn: option flag to signal
     * + _ERR_EMIT_SGN: allows the emission of the errorTable::clearErrorSgn();
     */
    void clearAutoExpireErrors(unsigned char opt = _ERR_NONE){
        int i = 0;
        bool cleared = false;

        // Clear all the errors in the activeErrror list that are flagged as AutoExpire
        while(i<activeErrors.size()){
            if(activeErrors[i].value().isAutoExpire()){

                // deactivate the error
                activeErrors[i].value().setActive(false);

                // Remove the error from the activeErrors
                activeErrors.removeAt(i);
                cleared = true;
                continue;
            }
            i++;
        }

        // If the signal emission is requested then emit the clear signal
        if(!cleared) return;
        if(!(opt & _ERR_NO_SGN)) emit clearErrorSgn();
    }

    //! Return true in case of any error should be active
    _inline bool isAnyErrorActive(void){
        if(activeErrors.size()) return true;
        else return false;
    }

    /**
     * @brief Test if a given error is active
     * @param errcode : this is the error identifier to be tested;
     * @return true in case the error should be active;
     */
    bool isErrorActive(QString errcode){
        for(int i=0; i< activeErrors.size(); i++){
            if(activeErrors[i].key() == errcode) return true;
        }
        return false;
    }


    bool errorExist(QString errcode){
        if( errorList.find(errcode) == errorList.end() ) return false;
        return true;
    }

    errorIterator getErrorIterator(QString errcode){
        return errorList.find(errcode);
    }
    bool isValidIterator(errorIterator i){
        if(i != errorList.end()) return true;
        return false;
    }

    _inline QString getErrorCode(QMap<QString, errorBlock>::iterator item){
        return item.key();
    }

    _inline QString getTitle(QMap<QString, errorBlock>::iterator item){
        return item.value().getTitle();
    }

    _inline QString getDescription(QMap<QString, errorBlock>::iterator item){
        return item.value().getDescription();
    }


    _inline QString getPixmapUID(QMap<QString, errorBlock>::iterator item){
        return item.value().PixmapUID;
    }


    _inline bool isActive(QMap<QString, errorBlock>::iterator item){
        return item.value().active;
    }


    _inline bool isAutoExpire(QMap<QString, errorBlock>::iterator item){
        return item.value().autoexpire;
    }


private:

    QMap<QString, errorBlock> errorList;    //!< List of the error description block
    QList<errorIterator> activeErrors;    //!< List of the current errors

    void setupErrors(void)
    {

        activeErrors.clear();

        // Error structure reserved for test purpose
        errorList["TEST01"] = errorBlock(QString("TITLE01\n TEST01 DESCRIPTION"),"errorPix.png");
        errorList["TEST02"] = errorBlock(QString("TITLE02\n TEST02 DESCRIPTION \n CONTINUE"),"errorPix.png");
        errorList["TEST03"] = errorBlock(QString("TITLE03"),"errorPix.png");
        errorList["TEST04"] = errorBlock(QString("TITLE04\n"),"errorPix.png");

        // Compose the error structure
        errorList["E00101"] = errorBlock(QApplication::translate("ERRORS_ARM","E00101"),"errorPix.png");
        errorList["E00102"] = errorBlock(QApplication::translate("ERRORS_ARM","E00102"),"errorPix.png");
        errorList["E00103"] = errorBlock(QApplication::translate("ERRORS_ARM","E00103"),"errorPix.png");
        errorList["E00104"] = errorBlock(QApplication::translate("ERRORS_ARM","E00104"),"errorPix.png");
        errorList["E00105"] = errorBlock(QApplication::translate("ERRORS_ARM","E00105"),"errorPix.png");
        errorList["E00106"] = errorBlock(QApplication::translate("ERRORS_ARM","E00106"),"errorPix.png");
        errorList["E00107"] = errorBlock(QApplication::translate("ERRORS_ARM","E00107"),"errorPix.png");
        errorList["E00108"] = errorBlock(QApplication::translate("ERRORS_ARM","E00108"),"errorPix.png");
        errorList["E00109"] = errorBlock(QApplication::translate("ERRORS_ARM","E00109"),"errorPix.png");
        errorList["E00110"] = errorBlock(QApplication::translate("ERRORS_ARM","E00110"),"errorPix.png");
        errorList["E00111"] = errorBlock(QApplication::translate("ERRORS_ARM","E00111"),"errorPix.png");
        errorList["E00112"] = errorBlock(QApplication::translate("ERRORS_ARM","E00112"),"errorPix.png");
        errorList["E00113"] = errorBlock(QApplication::translate("ERRORS_ARM","E00113"),"errorPix.png");
        errorList["E00114"] = errorBlock(QApplication::translate("ERRORS_ARM","E00114"),"errorPix.png");
        errorList["E00115"] = errorBlock(QApplication::translate("ERRORS_ARM","E00115"),"errorPix.png");
        errorList["E00116"] = errorBlock(QApplication::translate("ERRORS_ARM","E00116"),"errorPix.png");

    };
};


#endif // ERRORTABLE_H
