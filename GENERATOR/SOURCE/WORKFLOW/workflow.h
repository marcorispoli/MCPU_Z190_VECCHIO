#ifndef WORKFLOW_MANAGER_H
    #define WORKFLOW_MANAGER_H



#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTimer>

/**
* \defgroup  workflowModule Workflow Management Module.
*
* This Module implements the Workflow of the Application.
*
* # GENERAL OVERVIEW
*
* The Module uses the following files:
* - status.cpp: implements the function to manage the workflow;
* - workflow.h: defines all the classes used in the module;
*
* The Workflow is a particolar running *State* where the Application\n
* executes in a given sequence, several activities.
*
* There are several Workflows, each one totally implemented into a given
* function of the workflowManager class, and each one devoted to execute a given
* job.
*
* The steps of every Workflow routine is scheduled into the EventLoop queue where\n
* the time schedule is determined after each step completion, based on the occurrence.
*
* The function 'workflowManager::handleCurrentStatus()' is the root of the
* workflow handling: the function determines what is the current active workflow\n
* and runs its handling function class. This method shall be resceduled every \n
* step completion.
*
* There two types of Workflow routines:
* - Workflow routine: this is a major routine without a Parent routine to return;
* - Sub-Workflow routine: this is a Workflow routine that termines returning to the caller workflow.
*
* The Sub Workflow is used inside a major Workflow in order to execute a repetitive operation.
*
* In order to change the major Workflow, the Application shall call the 'workflowManager::changeWorkflow()';
*
* In order to call a Sub Workflow the Application shall call the 'workflowManager::changeSubWorkflow()' routine;
*
* Inside a SubWorkflow, the Application shall call the 'workflowManager::returnSubWorkflow()' to return to the caller major Workflow.
*
* # SUB MODULES
*
* The workflowModule implements three classes:
*
* - DbManager class: this class is used to read a SQLLite based database;
* - exposureManager class: this class implement the features related to the Exposure sequences;
* - workflowManager class: this class stictly implements the Workflow management;
*
* The workflowManager class contains the exposureManager class declaration and the DbManager declaraton
* as public elements.
*
* # WORKFLOW FUNCTIONS
*
* ## Smart Hub Connection Workflow
*
* - Function: 'workflowManager::handle_SH_CONNECTION()';
* - Workflow Code: SMS_SMART_HUB_CONNECTION;
*
* This workflow runs soon after the Application startup and every time
* the connection should drop. This workflow has the most priority over the rest
* of workflows.
*
* ## Generator Connection Workflow
*
* - Function: 'workflowManager::handle_GENERATOR_CONNECTION()';
* - Workflow Code: SMS_GENERATOR_CONNECTION;
*
* This workflow runs soon after the connection with the Smart Hub and every time \n
* the connection with the generator should drop. This workflow has the second most priority \n
* after the Smart Hub connection workflow.
*
* ## Generator Setup Workflow
*
* - Function: 'workflowManager::handle_SETUP_GENERATOR()';
* - Workflow Code: SMS_SETUP_GENERATOR;
*
* This workflow runs always after the connection with the Generator \n.
* in order to setup the Data structures ready for the EXPOSURE-> \n
*
* This Workflow can be called as a Sub-Workflow from the Tomo Exposure Workflows
* whenever the Tomo configuration changes.
*
* ## Clear Generator System Messages
*
* - Function: 'workflowManager::handle_CLEAR_ALL_SMS_MESSAGES()';
* - Workflow Code: SMS_CLEAR_SYS_MESSAGES;
*
* This Workflow can be called as a Sub-Workflow from every Workflow
* whenever the Generator System messages need to be cleared.
*
* ## Error Workflow
*
* - Function: 'workflowManager::handle_SMS_ERROR()';
* - Workflow Code: SMS_ERROR;
*
* This Workflow is activated whenever there are active system error messages\n
* into the Generator device.
*
* ## Idle Workflow
*
* - Function: 'workflowManager::handle_SMS_IDLE()';
* - Workflow Code: SMS_IDLE;
*
* This Workflow runs after the Generator initialization and after \n
* any exposure workflow termines. This shall considered as the
* Operating ready status workflow because in this status the Application \n
* can initiate an Exposure sequence.
*
*
* ## Exposure Workflows
*
* - Functions:
*   - 'exposureManager::handle_2D_MANUAL()';
*   - 'exposureManager::handle_3D_MANUAL()';
*   - 'exposureManager::handle_2D_AEC()';
*   - 'exposureManager::handle_3D_AEC()';
*
* - Workflow Codes:
*   - SMS_EXECUTING_2D_MANUAL;
*   - SMS_EXECUTING_2D_AEC;
*   - SMS_EXECUTING_3D_MANUAL;
*   - SMS_EXECUTING_3D_AEC;
*
*   Those are the Workflows implementing the Exposure sequences.
*   All those seqeunce starts from the IDLE status and termines into the \n
*   IDLE status.
*
*
*
*/

/**
 * @brief This class implements the SQL access to the Generator Error Messages database
 *
 * The Sedecal generator provides a SQLLite Database containing the
 * Generator System Error descriptions.
 *
 * The database is stored into C:\OEM\AppData\system_messags.sqlite.
 *
 * The Class allows to open an arbitrary sqlite database with
 * an arbitrary path in order to be more flexible for future path and name\n
 * modifications.
 *
 * \ingroup workflowModule
 */
class DbManager
{
public:

    /**
     * @brief The class creator automatically opens the database
     * @param path: this is the path and name of the Generator database.
     */
    DbManager(const QString& path){
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(path);
        m_db.open();
     }

    /**
     * @brief This function returns the string description of an error code
     * @param errcode: this is the error code
     * @return
     */
    QString getErrorString(ulong errcode){
        QString val;

      // Exec the query to the database, selecting the row corresponding to the error code
      QSqlQuery query;
      QString qstr = QString("SELECT ID,DESC FROM SystemMessage WHERE ID = %1").arg(errcode);
      query.exec(qstr);

      // From the table row, extract only the description field string
      if(query.next()){
          return query.value(0).toString() + " " + query.value(1).toString();
      }else return "";

    }

private:
    QSqlDatabase m_db;
};



/**
 * @brief This is the workflowManager implementation class
 *
 * This class implements the Workflow management.
 *
 *
 * \ingroup workflowModule
 */
class workflowManager : public QObject
{
    Q_OBJECT

public:
    explicit workflowManager(QObject *parent = nullptr);

    /// This is the enumeration for Workflow status
    typedef enum{
      SMS_UNDEFINED = 0,            //!< [0] Not defined state
      SMS_SMART_HUB_CONNECTION,     //!< [1] Smart Hup connection Workflow
      SMS_GENERATOR_CONNECTION,     //!< [2] Generator connection Workflow
      SMS_SETUP_GENERATOR,          //!< [3] Setup Generator  Workflow
      SMS_IDLE,                     //!< [4] Idle  Workflow
      SMS_ERROR,                    //!< [5] Error  Workflow
      SMS_CLEAR_SYS_MESSAGES,       //!< [6] Clear Generator System Messages  Workflow
      SMS_EXECUTING_2D_MANUAL,      //!< [7] Exposure 2D Workflow
      SMS_EXECUTING_2D_AEC,         //!< [8] Exposure 2D+AEC Workflow
      SMS_EXECUTING_3D_MANUAL,      //!< [9] Exposure 3D Workflow
      SMS_EXECUTING_3D_AEC,         //!< [10] Exposure 3D+AEC Workflow
    }_tWorkflowCode;

    /// This is the enumeration of the Workflow Status
    typedef enum{
        _NOT_READY_FOR_EXPOSURE = 0, //!< [0] Not ready to execute exposures
        _READY_FOR_EXPOSURE = 1,     //!< [1] Ready to execute exposures
        _STATUS_ERROR = 2,           //!< [2] The Application is in Error condition
    }tWorkflowStatus;




    /// This function requests the workflow to activate the 2D manual sequence
    _inline void start2DExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_2D_MANUAL;  }

    /// This function requests the workflow to activate the 2D+AEC sequence
    _inline void start2DAecExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_2D_AEC;  }

    /// This function requests the workflow to activate the 3D manual sequence
    _inline void start3DExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_3D_MANUAL;  }

    /// This function requests the workflow to activate the 3D+AEC sequence
    _inline void start3DAecExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_3D_AEC;  }

    /**
     * @brief This function tests the IDLE Workflow presence
     * @return true if the workflowManager is executing the IDLE status
     */
    _inline bool isIdle(void){ return  (executingWorkflow == SMS_IDLE ); }

    /// This function request to abort the current Exposure Procedure
    _inline void requestAbortProcedure(void){ abortRxRequest = true; }


    /**
     * @brief This function schedules the execution of the next workflow step
     * @param time: is the milliseconds delay time. 0 = immediate.
     */
    _inline void scheduleWorkflow(ulong time){ QTimer::singleShot(time, this, SLOT(handleCurrentStatus())); }

    /**
     * @brief This function activate the timer of the AEC waiting data timeout
     *
     * When the timer expires, the exposureManager::abortAecTimeoutRequest will be set.
     *
     * @param time: ms delay time for the timeout event
     */
    _inline void scheduleAecTmo(ulong time){ QTimer::singleShot(time, this, SLOT(timeoutAec())); }

    /**
     * @brief This function requests to activate a major workflow
     *
     * The new Workflow will start from the Substatus = 0.
     * The new workflow will start at the next scheduled event.
     *
     * @param new_workflow: is the requested workflow code.
     */
    void changeWorkflow(_tWorkflowCode new_workflow){
        if(executingWorkflow == new_workflow) return;

        executingWorkflow = new_workflow;
        subStatus = 0;

        returnWorkflow = executingWorkflow;
        returnSubStatus = subStatus;
        requestState = new_workflow;

        wait_command_processed = false;
        command_process_state = true;
        cp_timeout = 100;

        scheduleWorkflow(0);
    }

    /**
     * @brief This function request the activation of a SubWorkflow
     *
     * The SubWorkflow will be activated at the next schedule event.
     *
     * The SubWorkflow shall termines with the returSubWorkflow() routine
     *
     * @param new_workflow: this is the SubWorkflow code;
     * @param ret_substatus: this is the substatus of the calling workflow to return;
     */
    void  changeSubWorkflow(_tWorkflowCode new_workflow, uchar ret_substatus){
        if(executingWorkflow == new_workflow) return;

        returnWorkflow = executingWorkflow;
        returnSubStatus = ret_substatus;
        executingWorkflow = new_workflow;
        requestState = new_workflow;

        subStatus = 0;
        wait_command_processed = false;
        command_process_state = true;
        cp_timeout = 100;
        scheduleWorkflow(0);
    }

    /// This function shall be called by the SubWorkflow to return to the calling Workflow
    void returSubWorkflow(void){

        subStatus = returnSubStatus;
        executingWorkflow = returnWorkflow;
        requestState = returnWorkflow;

        wait_command_processed = false;
        command_process_state = true;
        cp_timeout = 100;

        scheduleWorkflow(0);
    }

private slots:
    void handleCurrentStatus(void); //!< This is the scheduled function that handles the workflows
    void timeoutAec(void);

public:


    bool    abortRxRequest;    //!< This flag is externally set to request the current exposure abort
    uchar subStatus;           //!< Current executing substatus of a running workflow

    bool wait_command_processed; //!< Request to check the reception of the command processed frame fro the generator
    uint8_t command_process_code;//!< Received command processed code
    bool command_process_state;  //!< Status of the command preocessed frame acquisition

    bool procedureCreated;      //!< The Generator procedure setup has been successfully completed

private:
    DbManager* DBSysMessages;           //!< Pointer to the SQLLite Database manager
    tWorkflowStatus interfaceStatus;    //!< Current status of the Application
    _tWorkflowCode requestState;        //!< External request to change the current workflow (only in IDLE status)
    _tWorkflowCode executingWorkflow;   //!< Current executing workflow code
    _tWorkflowCode returnWorkflow;      //!< Code of the parent workflow to return
    uchar          returnSubStatus;     //!< Return substatus code


    uint cp_timeout; //!< Timer for the command processed await


    void handleCommandProcessedState(void);
    void handle_SH_CONNECTION(void);
    void handle_GENERATOR_CONNECTION(void);
    void handle_SETUP_GENERATOR(void);
    void handle_CLEAR_ALL_SMS_MESSAGES(void);
    void handle_SMS_IDLE(void);
    void handle_SMS_ERROR(void);

};

/*
 *
 */
#endif // WORKFLOW_MANAGER_H
