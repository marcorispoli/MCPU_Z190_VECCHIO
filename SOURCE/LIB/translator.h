#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QWidget>
#include <QTranslator>

//! This is the main class for string translations
/*!
 * # ABSTRACT
 *
 * This class allows to translate the GUI strings
 * or messagges with the predefined languages.
 *
 * The language translation file for every available language
 * is built-in into the assets.qrc files: traduzione_xxx.qm file.
 *
 * # LANGUAGE PREPARATION
 * In order to update the whole project with the latest translation content
 * it has to be launched the QtCreator->Tools/External/Linguistic/"Update Translation".
 *
 * After the update procedure completes the files traduzione_xxx.ts will be ready to be
 * modified with the QTLanguage tool.
 *
 * When the translation has been completed it has to be launched the
 * QtCreator->Tools/External/Linguistic/"Release Translation". This will
 * create/replace the *.qm files.
 *
 * To complete the translation procedure the new *.qm files shall be replaced
 * into the assets.qrc file.
 *
 * # USAGE
 * There are two methodes to activate a given translation:
 * + Instancing the Class with a default language is the first step ;
 * + Using the method translatorClass::SetLanguage ;
 *
 * After the Class declaration, every subsequent instance of a translable String will be
 * translated with the default language.
 *
 * In order to change the current language after the Class declaration, it shall be used
 * the translatorClass::SetLanguage(...) member.
 *
 * NOTE: using the SetLanguage(...) does not translate the strings already translated.
 * So in order to retranslate ALL the desired strings, every module or class responsible
 * of those strings should re-instance the strings after detecting the signal  "changeLanguageSgn".
 *
 * In order to let a string to be translated, the string shall be declared with the following
 * sintax:
 * > QApplication::translate("CLASS","TAG");
 * Example:
 *
 * > QString stringa = QApplication::translate("STARTUP-WINDOW","HELLO WORLD");
 *
 * NOTE: the translation is made at the instance time!
 *
 */
class translatorClass : QObject
{
    Q_OBJECT

public:

    /// Subset of codes idenifying the available languages
    enum LANGUAGES
    {
        _LNG_ITA=0,
        _LNG_DEU,
        _LNG_FRA,
        _LNG_ENG,
        _LNG_PRT,
        _LNG_RUS,
        _LNG_ESP,
        _LNG_TUR,
        _LNG_POL,
        _LNG_CHN,
        _LNG_LTU,
        _LNG_UNDEF
    } ;


    // Costruttore
    translatorClass(LANGUAGES deflang); //!< Creates the class with default translation using the integer tag
    translatorClass(QString deflang);   //!< Creates the class with default translation using a String tag

    //!Distructor member
    virtual ~translatorClass(){};

    // Funzioni di impostazione traduzione corrente
    void setLanguage(LANGUAGES lng);    //!< Set a given language using the integer tag
    void setLanguage(QString lng);      //!< Set a given language using the string tag


signals:
    // Segnale di upgrade lingua per aggiornamento dinamico
    void changeLanguageSgn();           //!< Signal emitted after language activation

public:

private:
    LANGUAGES selectedLanguage;             //!< Current language TAG
    QTranslator   traduttore;               //!< Translator Object
    unsigned char isLanguage(QString lng);  //!< Check if the requested language is available

};

#endif // TRANSLATOR_H
