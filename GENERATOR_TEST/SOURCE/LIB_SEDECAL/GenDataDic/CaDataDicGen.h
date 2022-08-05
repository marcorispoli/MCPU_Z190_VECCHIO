/*!
 * \file      CaDataDicGen.cpp
 * \brief     Definition Of R2CP Generator Datatadic.
 * \author   
 * \bug       Unknown bugs
 * \version
 *  \htmlonly
 *		
 *	\endhtmlonly
 * \date      01/08/2012
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/

#ifndef __CADATADICGEN__
    #define __CADATADICGEN__

#include "R2CP_Eth.h"
#include "CaDataDic.h"
#include "R2CP_Index.h"
#include "Interfaces/aDataDicRadInterface.h"
#include "Interfaces/aDataDicNetworkInterface.h"
#include "Interfaces/aDataDicPatientInterface.h"
#include "Interfaces/aDataDicSystemInterface.h"
#include "Interfaces/aDataDicR2CPInterface.h"
#include "Interfaces/aDataDicServiceInterface.h"



#define PROTOCOL_VERSION_6	6
#define PROTOCOL_VERSION_5	5

const tDataDicProtocolVersion ProtocolVersionSupported[] =
{
	{ PROTOCOL_VERSION_5 , 5 , 'A' },
	{ PROTOCOL_VERSION_6 , 1 , 'A' },
};

namespace R2CP
{
    class CaDataDicGen : public CaDataDic
    {

	public:
		/*!
		 * Generator Datadic SingleTone
		 */
		static CaDataDicGen *GetInstance(void);
	
		/*!
		 *	\brief DataDic Initialitation
		 */
		void Initialitation(void);
	
		/*!
		 * \brief Set Communication Channel
		 */
		void SetCommunicationForm( CR2CP_Eth *Type);
	
		/*!
		 * \brief Inject Interfaces
		 */
	
		
		/*!
		 * \brief Checks messages versus geneator status
		 */
		virtual bool Check_NotAvailable_Message_vs_Status( word index, word subindex, tInfoMessage *MessageInfo );

		/*!
		 * \brief Lock Generator Datadic
		 */
		void Lock_DataDicGenMutex(void);

		/*!
		 * \brief Unlock Generator Datadic
		 */
		void UnLock_DataDicGenMutex(void);
	
	
		///////////////////////////////////  R2CP  DATADIC ///////////////////////////////////
		static void R2CP_HwVersion			(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_SwVersion			(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_Error				(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_Status				(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_MasterLifeTime		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_Reset				(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_ProtocolVersion	(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_BootVersion		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_PackageVersion		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_SerialNumber		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_Description		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void R2CP_SystemVersion		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		
		//Events
		void R2CP_Error_Event(byte Error);
		void R2CP_Status_Event(void);
		void R2CP_PackageVersion_Event(char *pStringPackageVersion);
		void R2CP_SystemVersion_Event( tSystemVersion *pSystemVersion );
		
		

		
		////////////////////////////////////GENERATOR INDEX////////////////////////////////////////////////////////////
		static void Generator_DataBank_AssignExposure					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_DataBank_ExposureAcceptance				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		//static void Generator_DataBank_DefineProcedure					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		//static void Generator_DataBank_ProcedureDatabankActivate		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadDataBank_Load							( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadDataBank_ProcedureAcceptance			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_TechniqueMode		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_KVP					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_MAS					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_MA					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_MS					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_MinIntegrationTime	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_MaxIntegrationTime	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_FocalSpot 			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_AECsensitivity		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_AECdensity			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_AECReference			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_AECchambers_V5 		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_TubePowerLimit		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_AECchambers			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_FPS					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_TrackingId			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_RadExposureParameter_PatientSize			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);		
		static void Generator_OtherFunctions_GenPowerLimit				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_OtherFunctions_Filmanents_Enable			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_OtherFunctions_TubePowerLimit				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_CurrentRadDataBank_RADparams				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_ExposureManagement_GeneratorStatus		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_DataBank_DefineProcedure                  ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_Miscellaneous_RadRanges					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_Miscellaneous_FlRanges					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_ExposureManagement_RadPostExposure		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_ExposureManagement_FlPostExposure			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Generator_ExposureManagement_StartStopExposure		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_CommunicationsInhibitTimeout				(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_ExposureManagement_ResetExposureCounter   (tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_ExposureSwitchesActivationDevice			(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_Miscellaneous_MaxProcedure				(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_Miscellaneous_MaxDatabank					(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr); 
		static void Generator_Miscellaneous_AecReferenceMode			(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr); 
		static void Generator_Miscellaneous_CountersData				(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_Miscellaneous_DigitalInputsOutputs		(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_LockIn_Datatbank							(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_LockIn_AssignToProcedure					(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_LockIn_FrameReached						(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);
		static void Generator_Injector									(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);

		
		static void Generator_Miscellaneous_RadProcedureParameterRanges  	 ( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr );
		static void Generator_Miscellaneous_AssignRadProcedureParameterRanges( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr );
		static void Generator_Miscellaneous_RadParameterScales				 ( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr );
		static void Generator_Miscellaneous_CalibrationStatus				 ( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr );
		
		

		//Common Gen Events    
		void Generator_DataBank_DefineProcedure_Event( tProcedure *pProcedure );
		void Generator_DataBank_Define_NExposures_Procedure_Event( tProcedure *pProcedure );
		void Generator_ClearAllProcedure_Event(void);
		void Generator_DataBank_ProcedureDatabankActivate_Event( byte procedureID , byte Procedure_Status , byte ExpDBSequenceNumber );
		void Generator_DataBank_AssignExposure_Event(byte procedureID, byte ExpSequenceNumber, byte ExpDBId , bool Allowed);
		
		//Rad Gen Events
		void Generator_DataBank_ExposureAcceptance_Event(byte ProcedureId, byte ExpSeqNumber , bool Allowed);
		void Generator_RadDataBankLoad_Deprecated_Event(tRadDb *RadDatabank);
		void Generator_RadDataBankLoad_Event(tRadDb *RadDatabank);
		void Generator_RadDataBank_ProcedureAcceptance_Event(byte ProcedureID, bool Allowed, word NumberExposuresAllowed);
		void Generator_CurrentRadDataBank_RADparams_Event(tRadDb *RadDatabank);
		void Generator_Miscellaneous_RadExposureParameterScale_Event( tScale kVpScale , tScale mAsScale , tScale mAScale , tScale msScale  );
		void Generator_Miscellaneous_RadProcedureParameterRanges_Event( R2CP::tRadParameterRanges  *pRadParameterRanges );
		void Generator_Miscellaneous_AssignRadProcedureParameterRanges_Event( byte ProcedureId , byte DatabankId , bool Allowed );
		

		void Generator_DoseLevel_Event( byte DatabankId , uword DoseLevelId );
		
		void Generator_OtherFunctions_GenPowerLimit_Event( bool Allowed , byte GenPowerLimit );
		void Generator_OtherFunctions_FilmanentsEnable_Event(bool Status);
		
		void Generator_Miscellaneous_RadRanges_Event(tRadExpParamRange *pRadRanges);
		
		void Generator_CommunicationsInhibitTimeout_Event( byte CommInhibit_sec , bool status , byte SourceNode);
		void Generator_ExposureManagement_RadPostExposure_Event( tRadPostExpPostCondition *pPostExpPostCondition );
//		void Generator_ExposureManagement_GeneratorStatus_Event(tGeneratorStatus *LastGeneratorStatus);
		
		
		void Generator_DataBank_ProcedureDatabankDefault_Event(byte ProcedureDefaultId , byte ExpSeqId );
		
		void Generator_Miscellaneous_MaxProcedure_Event(byte MaxNumProcedureAllowed);
		void Generator_Miscellaneous_MaxDatabank_Event(byte MaxNumDBAllowed);
		void Generator_Miscellaneous_AecReferenceMode_Event( tTypeAecReference AecReference );
		void Generator_Miscellaneous_CountersData_Event(byte TubeId , byte CommandId , dword SmallFocusData , dword LargeFocusData );
		void Generator_Miscellaneous_DigitalInputsOutputs_Event( word DigitalInputsMask , word DigitalInputsValue);
		void Generator_ExposureSwitchesActivationDevice_Event( tExposureSwitchesActivation ExposureSwitchesActivation );
		void Generator_Miscellaneous_CalibrationStatus_Event( tCalibrationStatus CalibrationStatus );
		void Generator_LockInDatabank_Event(  byte LockInDb , bool AutoLockInEnabled , byte AutoLockInTime_Sec10 );
		void Generator_LockInAssignToProcedure_Event(  byte ProcedureId , byte LockInDb , bool Allowed );
		void Generator_LockInFrameReached_Event( byte LockInDb , byte LockInFrameReached );
		void Generator_Injector_Event( tInjectorStatus InjectorStatus );
		

		///////////////////////////////////PATIENT INDEX////////////////////////////////////////////////////////////////
		static void Patient_Procedure_NExposures_Definition		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Procedure_Activate 					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Procedure_Default					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Procedure_ClearAll					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Redefine_HandFootswitchActivation	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Redefine_Workstation				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_Sync_Up				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_WorkingMode			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_Pos_Cal_Number		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_PowerOff				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_PowerOffRequest		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_PowerOffCancel		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Patient_Miscellaneous_PowerOffConfirm		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		//Events
		void Patient_Miscellaneous_WorkingMode_Event(byte WorkingMode, byte Node);
		void Patient_Miscellaneous_PowerOffCancel_Event(void);
		void Patient_Miscellaneous_PowerOff_Request_Event( tPowerOffType Type );
		void Patient_Miscellaneous_Positioner_Calibration_Number_Event( byte Pos_Id , bool SendCp = false , byte CommandProcessedCode = 0 , byte Sequence = 0); 

		
		//////////////////////////////////SYSTEM INDEX////////////////////////////////////////////////////////////////
		static void System_SystemMessage			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_AllSystemMessages		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_LoadDesktop				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_DesktopDisplay			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_NumericPropertySetup 	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_NumericPropertyValue 	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);	
		static void System_StringProperty		 	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);	
		static void Sytem_NumericValueTable			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Sytem_NumericValueTableItem 	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_StringTable				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr); 
		static void System_SingleUserRequest		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void System_UserRequestTable			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
			
		//Events
		void System_SystemMessage_Event(tSystemMessage *pSystemMessage, bool status);
		
		//////////////////////////////////NETWORK INDEX////////////////////////////////////////////////////////////////
		static void Network_HeartBeat				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_ConnectionChanged		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_NodeStatus				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_ConnectionRequest		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_IpConfig				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_FileConfigUpdate		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_SnapShot				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_BackupSnapShot			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_Restore					( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Network_CalConfigUpdate			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
	
		//Events
		void Network_IpConfig_Event(udword IpAddress, udword MaskSubNetwork , udword Gateway , udword ShIpAdress);
		void Network_ConnectionRequest_Event(byte node_dest, byte node_iss);
		void Network_BackupSnapShot_Event ( tBackupSnapShotStatus Status );
		void Network_Restore_Event ( tRestoreStatus Status );
		
		//////////////////////////////////SERVICE INDEX//////////////////////////////////////////////////////////////////
		static void Service_StandardLicense			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		static void Service_ExtendedLicense			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		
		//////////////////////////////// IMAGE SYSTEM////////////////////////////////////////////////////////////////////
		static void ImageSystem_CurrentLsb			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
		
		//Events
		void Service_Log_Event(byte *ServiceLog , word SizeofLog);
		void Service_StandardLicense_Event (tStandardLicense *StandardLicense);
		void Service_ExtendedLicense_Event (tExtendedLicense *ExtendedLicense);
		
		//Heartbeat 
		bool HeartbeatGot(byte node, byte keyWord, byte status){return true;}
		bool ReservedGot( byte Priority, byte Node , byte Index , byte SubIndex , byte *pData , word Size );
	
		static bool	m_EnableLogInfo;
		
		/*!
		 * \brief Log function Callback
		 */
		static void (*m_fcb_log_) ( byte LogLevel , const char *string, ... );
		

        // Get Functions
        void Protocol_Get_Version(void);
        void Protocol_Set_Version6(void);
        void Protocol_Set_Version5(void);


        void Generator_Get_StatusV5(void);
        void Generator_Get_StatusV6(void);
        void SystemMessages_Get_AllMessages(void);

        bool SystemMessages_Clear_Message(uint ID);
        void SystemMessages_Clear_AllMessages(void);

        void Patient_SetupProcedureV5(byte num);
        void Patient_SetupProcedureV6(byte num);
        void Generator_Set_Databank(byte num);

	private:
		
		/*!
		 * \brief Constructor
		 */
		CaDataDicGen(byte Node);
		/*!
		 * \brief Destructor
		 */
		~CaDataDicGen();
	
		/*!
		 * \brief  Gets Node to send 
		 */
		byte GetNodeEvent(bool IsFromCp = false);
	
		/*!
		 *	\brief Sets Node 
		 */
		void SetNodeEvent(byte Node, bool IsFromGet = false);
		
		/*!
		 * \brief Objet SingleTone Pointer
		 */
		static CaDataDicGen *m_p_instance_;	   
	
		/*!
		 * \brief Current Node pending message
		 */
		byte m_Current_Node_Dest_;
	
		/*!
		 * \brief Get Request Pending
		 */
		bool m_IsGetRequest_Pending_;
	
		/*!
		 * \brief Template Communication Type pointer
		 */
		static CR2CP_Eth * m_Type_;
		
		/*Interface Pointers*/
		static CaDataDicInterface		  	*m_p_DataDicInterface_;
		static CaDataDicNetworkInterface    *m_p_NetworkInteface_;
		static CaDataDicR2CPInterface 	  	*m_p_R2CPInterface_;		
        static CaDataDicRadInterface	  	*m_p_RadInterface_;
        static CaDataDicSystemInterface  	*m_p_SystemInterface_;
		static CaDataDicPatientInterface 	*m_p_PatientInterface_;		
		static CaDataDicServiceInterface	*m_p_ServiceInterface_;


    public:
        CaDataDicRadInterface       radInterface;
        CaDataDicSystemInterface    systemInterface;
        tDataDicProtocolVersion     protocolVersion;
        bool isProtoV6(void){return (protocolVersion.Version == 6) ? true: false;}
        bool isProtoV5(void){return (protocolVersion.Version == 5) ? true: false;}

	};
};//namespace R2CP

#endif
