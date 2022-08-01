/*!
 * \file      CaDataDicGen.cpp
 * \brief     Definition of the local object dictionary.
 * \author   
 * \bug       Unknown bugs
 * \version
 *  \htmlonly
 *		<A HREF="">V1R01.1</A>
 *	\endhtmlonly
 * \date      01/08/2012
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/
#include <string.h>

#include "CaDataDicGen.h"

namespace R2CP
{
	const byte R2CP_VERSION_VERSION 		= 0x05;
	const byte R2CP_VERSION_SUBVERSION		= 0x05;
	const byte R2CP_VERSION_REVISION		= '0';
	
	bool	CaDataDicGen::m_EnableLogInfo = false;
	CR2CP_Eth * CaDataDicGen::m_Type_ = nullptr;
	CaDataDicGen *CaDataDicGen::m_p_instance_ = nullptr;
	void (*CaDataDicGen::m_fcb_log_) ( byte LogLevel , const char *string, ... ) = nullptr;
	
	/*Interface Pointers*/
	CaDataDicInterface		  	*CaDataDicGen::m_p_DataDicInterface_	= nullptr;
    CaDataDicNetworkInterface   *CaDataDicGen::m_p_NetworkInteface_		= nullptr;
    CaDataDicRadInterface	  	*CaDataDicGen::m_p_RadInterface_		= nullptr;
	CaDataDicR2CPInterface 	  	*CaDataDicGen::m_p_R2CPInterface_		= nullptr;
	CaDataDicSystemInterface  	*CaDataDicGen::m_p_SystemInterface_		= nullptr;
	CaDataDicPatientInterface 	*CaDataDicGen::m_p_PatientInterface_	= nullptr;
	CaDataDicServiceInterface	*CaDataDicGen::m_p_ServiceInterface_	= nullptr;
	
	/*!
	 * \brief Defines lenght for entries which lenght is variable (blocks).
	*/
	#define UNDEFINED_LENGHT        (byte)-1   
	
	const tEntry Row_00HEX[] = { // R2CP
	//	SubIndex,					Func,					tDataDicAccess,								GS,			SS,		    AS		ES
		{R2CP_HW_VERSION,			CaDataDicGen::R2CP_HwVersion,			(tDataDicAccess)DATADIC_ACCESS_GET,		0,			0,			0,		0},
		{R2CP_SW_VERSION,			CaDataDicGen::R2CP_SwVersion,			(tDataDicAccess)DATADIC_ACCESS_GET,		0,			0,			0,		0},
		{R2CP_STATUS,				CaDataDicGen::R2CP_Status,				(tDataDicAccess)DATADIC_ACCESS_GET,		0,			0,			0,		0},
		{R2CP_RESET,				CaDataDicGen::R2CP_Reset,				(tDataDicAccess)DATADIC_ACCESS_SET,		0,			0,			0,		0},
		{R2CP_ERROR_CODE,			CaDataDicGen::R2CP_Error,				(tDataDicAccess)DATADIC_ACCESS_SET_GET,	0,			0,			0,		0},
		{R2CP_MASTER_LIFE_TIME_OUT,	CaDataDicGen::R2CP_MasterLifeTime,		(tDataDicAccess)DATADIC_ACCESS_SET_GET,	0,			2,			0,		0},
		{R2CP_PROTOCOL_VERSION,		CaDataDicGen::R2CP_ProtocolVersion,		(tDataDicAccess)DATADIC_ACCESS_SET_GET, 0,			3,			0,		0},
		{R2CP_BOOT_VERSION,			CaDataDicGen::R2CP_BootVersion,			(tDataDicAccess)DATADIC_ACCESS_GET,     0,			0,			0,		0},
		{R2CP_PACKAGE_VERSION,		CaDataDicGen::R2CP_PackageVersion,		(tDataDicAccess)DATADIC_ACCESS_GET,     0,			0,			0,		0},
		{R2CP_SERIAL_NUMBER,		CaDataDicGen::R2CP_SerialNumber,		(tDataDicAccess)DATADIC_ACCESS_SET_GET, 0,			UNDEFINED_LENGHT,			0,		0},
		{R2CP_DESCRIPTION,			CaDataDicGen::R2CP_Description,			(tDataDicAccess)DATADIC_ACCESS_GET,     0,			0,			0,		0},
		{R2CP_SYSTEM_VERSION,		CaDataDicGen::R2CP_SystemVersion,		(tDataDicAccess)DATADIC_ACCESS_GET,		0,			0,			0,		0}
	};
	

	
	const tEntry Row_20HEX[] = { // GENERATOR
        {GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5,				CaDataDicGen::Generator_ExposureManagement_GeneratorStatus,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		14,		14},
        {GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V6,				CaDataDicGen::Generator_ExposureManagement_GeneratorStatus,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,		14,		14},
/*
		{GENERATOR_DATA_BANK_ASSIGN_EXPOSURE,							CaDataDicGen::Generator_DataBank_AssignExposure,					DATADIC_ACCESS_SET_GET,	2,		3,		0,		0},
		{GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE,						CaDataDicGen::Generator_DataBank_ExposureAcceptance,				DATADIC_ACCESS_GET,		2,		0,		0,		0},
		{GENERATOR_RAD_DATA_BANK_LOAD_V5,								CaDataDicGen::Generator_RadDataBank_Load,							DATADIC_ACCESS_SET_GET,	1,		27,		0,		0},
		{GENERATOR_RAD_DATA_BANK_ACCEPTANCE,							CaDataDicGen::Generator_RadDataBank_ProcedureAcceptance,			DATADIC_ACCESS_GET,		2,		0,		0,		0},
		{GENERATOR_RAD_DATA_BANK_LOAD_V6,								CaDataDicGen::Generator_RadDataBank_Load,							DATADIC_ACCESS_SET_GET,	1,		27,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_TECHNIQUE_MODE,				CaDataDicGen::Generator_RadExposureParameter_TechniqueMode,			DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_KVP,							CaDataDicGen::Generator_RadExposureParameter_KVP,					DATADIC_ACCESS_SET_GET,	1,		4,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_MAS,							CaDataDicGen::Generator_RadExposureParameter_MAS,					DATADIC_ACCESS_SET_GET,	1,		5,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_MA,							CaDataDicGen::Generator_RadExposureParameter_MA,					DATADIC_ACCESS_SET_GET,	1,		5,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_MS,							CaDataDicGen::Generator_RadExposureParameter_MS,					DATADIC_ACCESS_SET_GET,	1,		5,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_MIN_INTEGRATION_TIME,			CaDataDicGen::Generator_RadExposureParameter_MinIntegrationTime,	DATADIC_ACCESS_SET_GET, 1 ,		4,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_MAX_INTEGRATION_TIME,			CaDataDicGen::Generator_RadExposureParameter_MaxIntegrationTime,	DATADIC_ACCESS_SET_GET,	1,		4,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_FOCAL_SPOT,					CaDataDicGen::Generator_RadExposureParameter_FocalSpot,				DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_SENSITIVITY,				CaDataDicGen::Generator_RadExposureParameter_AECsensitivity,		DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_DENSITY,					CaDataDicGen::Generator_RadExposureParameter_AECdensity,			DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_CHAMBERS_V5,				CaDataDicGen::Generator_RadExposureParameter_AECchambers_V5,		DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_TUBEPOWERLIMIT,				CaDataDicGen::Generator_RadExposureParameter_TubePowerLimit,		DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_FPS,							CaDataDicGen::Generator_RadExposureParameter_FPS,					DATADIC_ACCESS_SET_GET,	1,		4,		0,		0}, 
		{GENERATOR_RAD_EXPOSURE_PARAMETER_TRACKINGID,					CaDataDicGen::Generator_RadExposureParameter_TrackingId,			DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_PATIENT_SIZE,					CaDataDicGen::Generator_RadExposureParameter_PatientSize,			DATADIC_ACCESS_SET_GET, 1,		4,		0,		0},
		{GENERATOR_RAD_EXPOSURE_AEC_REFERENCE,							CaDataDicGen::Generator_RadExposureParameter_AECReference,			DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},
		{GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_CHAMBERS_V6,				CaDataDicGen::Generator_RadExposureParameter_AECchambers,			DATADIC_ACCESS_SET_GET,	1,		3,		0,		0},

		

		{GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V5,					CaDataDicGen::Generator_CurrentRadDataBank_RADparams,				DATADIC_ACCESS_GET,		0,		0,		0,		0},
		{GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V6,					CaDataDicGen::Generator_CurrentRadDataBank_RADparams,				DATADIC_ACCESS_GET,		0,		0,		0,		0},
		{GENERATOR_OTHER_FUNCTIONS_GENPOWERLIMIT,						CaDataDicGen::Generator_OtherFunctions_GenPowerLimit,				DATADIC_ACCESS_SET_GET,	0,		2,		0,		0},
		{GENERATOR_OTHER_FUNCTIONS_FILAMENTS_ENABLED,					CaDataDicGen::Generator_OtherFunctions_Filmanents_Enable,			DATADIC_ACCESS_SET_GET,	0,		1,		0,		0},
		{GENERATOR_OTHER_FUNCTIONS_TUBEPOWERLIMIT,						CaDataDicGen::Generator_OtherFunctions_TubePowerLimit,				DATADIC_ACCESS_SET_GET,	0,		3,		0,		0},

		{GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE,				CaDataDicGen::Generator_ExposureManagement_StartStopExposure,		DATADIC_ACCESS_SET,		 0,		1,		0,		0},
		{GENERATOR_EXPOSURE_MANAGEMENT_COMMUNICATIONS_INHIBIT_TIMEOUT,	CaDataDicGen::Generator_CommunicationsInhibitTimeout,				DATADIC_ACCESS_SET_GET,  0,		2,		0,		0},
		{GENERATOR_EXPOSURE_MANAGEMENT_RAD_POST_EXPOSURE,				CaDataDicGen::Generator_ExposureManagement_RadPostExposure,			DATADIC_ACCESS_SET, 	 0,		0,		0,		0},
		{GENERATOR_EXPOSURE_MANAGEMENT_RESET_EXPOSURE_COUNTER,			CaDataDicGen::Generator_ExposureManagement_ResetExposureCounter,	DATADIC_ACCESS_GET, 	 0,		0,		0,		0},		
		{GENERATOR_EXPOSURE_SWITCHES_ACTIVATION_DEVICE,				    CaDataDicGen::Generator_ExposureSwitchesActivationDevice,			DATADIC_ACCESS_GET, 	 0,		0,		0,		0},
		{GENERATOR_MISCELLANEOUS_MAXPROCEDURE,							CaDataDicGen::Generator_Miscellaneous_MaxProcedure,					DATADIC_ACCESS_GET,		 0,		0,		0,		0},
		{GENERATOR_MISCELLANEOUS_MAXDATABANK,							CaDataDicGen::Generator_Miscellaneous_MaxDatabank,					DATADIC_ACCESS_GET,		 0,		0,		0,		0},
		{GENERATOR_MISCELLANEOUS_RAD_EXPOSURE_PARAMETER_RANGES,			CaDataDicGen::Generator_Miscellaneous_RadRanges,					DATADIC_ACCESS_GET,		 0,		0,		0,		0},
		{GENERATOR_MISCELLANEOUS_AEC_EXTERNAL_REFERENCE,				CaDataDicGen::Generator_Miscellaneous_AecReferenceMode,				DATADIC_ACCESS_SET_GET,  0,     1,      0,      0},  
		{GENERATOR_MISCELLANEOUS_COUNTERS_DATA,							CaDataDicGen::Generator_Miscellaneous_CountersData,					DATADIC_ACCESS_GET, 	 2,     0,      0,      0},  
		{GENERATOR_MISCELLANEOUS_DIGITAL_INPUTS_OUTPUTS,				CaDataDicGen::Generator_Miscellaneous_DigitalInputsOutputs,			DATADIC_ACCESS_GET,		 2,		0,		0,		0},
		
		{GENERATOR_LOCKIN_DATABANK,										CaDataDicGen::Generator_LockIn_Datatbank,							DATADIC_ACCESS_SET_GET,	 1,		3,		0,		0},
		{GENERATOR_ASSIGN_LOCKINDB_TO_PROCEDURE,						CaDataDicGen::Generator_LockIn_AssignToProcedure,					DATADIC_ACCESS_SET_GET,	 1,		2,		0,		0},
		{GENERATOR_LOCKIN_FRAME_REACHED,								CaDataDicGen::Generator_LockIn_FrameReached,						DATADIC_ACCESS_GET,		 1,		0,		0,		0},
		{GENERATOR_INJECTOR,											CaDataDicGen::Generator_Injector,									DATADIC_ACCESS_SET_GET,	 0,		1,		0,		0},

		
		{GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES,		CaDataDicGen::Generator_Miscellaneous_RadProcedureParameterRanges,			DATADIC_ACCESS_SET_GET,	 1,		28,		0,		0},
		{GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES_ASSIGN,	CaDataDicGen::Generator_Miscellaneous_AssignRadProcedureParameterRanges,	DATADIC_ACCESS_SET_GET,	 1,		2,		0,		0},
		{GENERATOR_MISCELLANEOUS_RAD_PARAMETER_SCALES,					CaDataDicGen::Generator_Miscellaneous_RadParameterScales,					DATADIC_ACCESS_GET,	 	 0,		0,		0,		0},
		{GENERATOR_MISCELLANEOUS_CALIBRATION_STATUS,					CaDataDicGen::Generator_Miscellaneous_CalibrationStatus,					DATADIC_ACCESS_GET,	 	 0,		0,		0,		0},
*/
	};
	
	
	const tEntry Row_80HEX[] = { // SYSTEM MESSAGE
	//	SubIndex,														Func,												tDataDicAccess,			GetSize,	SetSize
		{SYSTEM_SYSTEM_MESSAGE,											CaDataDicGen::System_SystemMessage,			DATADIC_ACCESS_SET_GET,				4,		10,		0,		0},
		{SYSTEM_ALL_SYSTEM_MESSAGES,									CaDataDicGen::System_AllSystemMessages,		DATADIC_ACCESS_SET_GET,				0,		0,		0,		0},
		{SYSTEM_VD_LOAD_DESKTOP,										CaDataDicGen::System_LoadDesktop,			DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_DESKTOP_DISPLAY,										CaDataDicGen::System_DesktopDisplay,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_PROPERTY_SETUP,								CaDataDicGen::System_NumericPropertySetup, 	DATADIC_ACCESS_GET,					9,		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_PROPERTY_VALUE,								CaDataDicGen::System_NumericPropertyValue,	DATADIC_ACCESS_ANSWER_EVENT, 		0, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_STRING_PROPERTY,										CaDataDicGen::System_StringProperty,		DATADIC_ACCESS_SET_GET,				9, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_VALUE_TABLE,									CaDataDicGen::Sytem_NumericValueTable,		DATADIC_ACCESS_SET_GET, 			9, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_NUMERIC_VALUE_TABLE_ITEM,							CaDataDicGen::Sytem_NumericValueTableItem,	DATADIC_ACCESS_SET_GET, 			10,		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_STRING_TABLE,										CaDataDicGen::System_StringTable,			DATADIC_ACCESS_SET_GET, 			9, 		0, 	UNDEFINED_LENGHT , UNDEFINED_LENGHT },
		{SYSTEM_VD_SINGLE_USER_REQUEST,									CaDataDicGen::System_SingleUserRequest,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	0x05 , 0x05},
		{SYSTEM_VD_USER_REQUEST_TABLE,									CaDataDicGen::System_UserRequestTable,		DATADIC_ACCESS_ANSWER_EVENT,		0,		0,	0x05,  0x05}	
	};
	
	const tEntry Row_A0HEX[] = { // NETWORK
		//	SubIndex,													Func,										tAccess,										GetSize,	SetSize
		{ NETWORK_CONNECTION_CHANGED,									CaDataDicGen::Network_ConnectionChanged,	(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		2,		2 },
		{ NETWORK_NODESTATUS,											CaDataDicGen::Network_NodeStatus,			(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		2,		2 },
		{ NETWORK_HEARTBEAT,											CaDataDicGen::Network_HeartBeat,			(tDataDicAccess)(DATADIC_ACCESS_GET),			2,		 	0,		0,		0 },
		{ NETWORK_CONNECTION_REQUEST,									CaDataDicGen::Network_ConnectionRequest,	(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		1,		1 },
		{ NETWORK_CONFIGURATION,										CaDataDicGen::Network_IpConfig,				(tDataDicAccess)(DATADIC_ACCESS_SET_GET),		0,		  	16,		0,		0 }, 
		{ NETWORK_FILECONFIGUPDATE,										CaDataDicGen::Network_FileConfigUpdate,		(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,		  	0,		0,		0 },
		{ NETWORK_SNAPSHOT,												CaDataDicGen::Network_SnapShot,				(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,			0,		1,		1 },	
		{ NETWORK_BACKUP_SNAPSHOT,										CaDataDicGen::Network_BackupSnapShot,		(tDataDicAccess)(DATADIC_ACCESS_SET),			0,			2,		0,		0 },	
		{ NETWORK_RESTORE,												CaDataDicGen::Network_Restore,				(tDataDicAccess)(DATADIC_ACCESS_SET),			0,			1,		0,		0 },
		{ NETWORK_CALIBRATION_UPDATE,									CaDataDicGen::Network_CalConfigUpdate,		(tDataDicAccess)(DATADIC_ACCESS_ANSWER_EVENT),	0,			0,		0,		0 },
	};
	
	
	const tEntry Row_E0HEX[] = { // PATIENT WORKFLOW
	//	SubIndex,														Func,															tAccess,				GetSize,	  SetSize
		{PATIENT_PROCEDURE_DEFINITION,									CaDataDicGen::Patient_Procedure_Definition,				DATADIC_ACCESS_SET_GET,					1,		 	14,		0,		0},
		{PATIENT_PROCEDURE_ACTIVATE,									CaDataDicGen::Patient_Procedure_Activate,				DATADIC_ACCESS_SET_GET,					1,		  	6,		0,		0},
		{PATIENT_PROCEDURE_DEFAULT,										CaDataDicGen::Patient_Procedure_Default,				DATADIC_ACCESS_SET_GET,					0,		  	6,		0,		0},
		{PATIENT_PROCEDURE_CLEAR_ALL,									CaDataDicGen::Patient_Procedure_ClearAll,				DATADIC_ACCESS_SET,						0,			0,		0,		0},
		{PATIENT_PROCEDURE_NEXPOSURES_DEFINITION,						CaDataDicGen::Patient_Procedure_NExposures_Definition,	DATADIC_ACCESS_SET_GET,					1,			16,		0,		0},
		{PATIENT_REDEFINE_HANDFOOTSWITCH_ACTIVATION,					CaDataDicGen::Patient_Redefine_HandFootswitchActivation,DATADIC_ACCESS_SET,						0,			2,		0,		0},
		{PATIENT_REDEFINE_WORKSTATION,									CaDataDicGen::Patient_Redefine_Workstation,				DATADIC_ACCESS_SET,						0,			2, 		0,		0},
		{PATIENT_MISCELLANEOUS_SYNC_UP,									CaDataDicGen::Patient_Miscellaneous_Sync_Up,			DATADIC_ACCESS_GET,						0,			0, 		0,		0},
		{PATIENT_MISCELLANEOUS_POWEROFF,								CaDataDicGen::Patient_Miscellaneous_PowerOff,			DATADIC_ACCESS_ALL_MEMBERS,				0,			2 ,		2,		2},
		{PATIENT_MISCELLANEOUS_POWEROFF_CANCEL,							CaDataDicGen::Patient_Miscellaneous_PowerOffCancel,		DATADIC_ACCESS_SET,						0,			0,		0,		0},
		{PATIENT_MISCELLANEOUS_POWEROFF_CONFIRM,						CaDataDicGen::Patient_Miscellaneous_PowerOffConfirm,	DATADIC_ACCESS_SET,						0,			1,		0,		0},
		{PATIENT_MISCELLANEOUS_WORKINGMODE,								CaDataDicGen::Patient_Miscellaneous_WorkingMode,		DATADIC_ACCESS_SET_GET,					0,			2,		0,		0},
		{PATIENT_MISCELLANEOUS_POSITIONER_CALIBRATION_NUMBER,			CaDataDicGen::Patient_Miscellaneous_Pos_Cal_Number,		DATADIC_ACCESS_SET_GET,					0,			1,		0,		0},
		
	};
	
	const tEntry Row_C0HEX[] = {
	//	SubIndex,														Func,														tAccess,					GetSize,	  SetSize
		{SERVICE_STANDARD_LICENSE,										CaDataDicGen::Service_StandardLicense,						DATADIC_ACCESS_GET,			0,			0,		0,		0},
		{SERVICE_EXTENDED_LICENSE,										CaDataDicGen::Service_ExtendedLicense,						DATADIC_ACCESS_GET,			0,			0,		0,		0},
	};

	
	const tRow GenDictionary[] = {
		{NETWORK_COMMANDS_ENTRY,			(sizeof(Row_A0HEX) / sizeof(tEntry)),	Row_A0HEX },	// NETWORK
		{R2CP_COMMANDS_ENTRY,       		(sizeof(Row_00HEX)/sizeof(tEntry)),		Row_00HEX },	// R2CP
		{GENERATOR_COMMANDS_ENTRY,			(sizeof(Row_20HEX)/sizeof(tEntry)),		Row_20HEX },	// GENERATOR
        {SYSTEM_COMMANDS_ENTRY,				(sizeof(Row_80HEX)/sizeof(tEntry)),		Row_80HEX },	// SYSTEM MESSAGE
		{PATIENT_COMMANDS_ENTRY,			(sizeof(Row_E0HEX)/sizeof(tEntry)),		Row_E0HEX },	// PATIENT WORKFLOW
		{SERVICE_COMMANDS_ENTRY,			(sizeof(Row_C0HEX)/sizeof(tEntry)),		Row_C0HEX }		// SERVICE
	};
	
	/*!
	 * \brief Object dictionary defined, ready to export.
	*/
	const tTable GeneratorDataDictionary = {(sizeof(GenDictionary)/sizeof(tRow)) ,GenDictionary};
		
	
	
	CaDataDicGen *CaDataDicGen::GetInstance(void)
	{
			
		if (m_p_instance_ == nullptr) 
            m_p_instance_ = new CaDataDicGen(APPLICATION_NODE_ID);
		return m_p_instance_;
	}
	
	CaDataDicGen::CaDataDicGen(byte Node) : CaDataDic( Node )
	{
		/* Initialize mutex attributes */
        // datadic_mutex = OS::Mutex::Create();
		m_Current_Node_Dest_	= 0;
		m_IsGetRequest_Pending_ = false;
		
		pDataDictionary 			= &GeneratorDataDictionary;
		
		mProtocolVersion.Version	= R2CP_VERSION_VERSION;
		mProtocolVersion.SubVersion	= R2CP_VERSION_SUBVERSION;
		mProtocolVersion.Revision	= R2CP_VERSION_REVISION;
		
		m_fcb_log_					= nullptr;
	}
	
	CaDataDicGen::~CaDataDicGen()
	{
		
	}
	

	
	void CaDataDicGen::Initialitation(void)
	{
		if(m_p_DataDicInterface_)
			m_p_DataDicInterface_->Initialitation();
		if(m_p_R2CPInterface_)
			m_p_R2CPInterface_->Initialitation();

        m_p_RadInterface_ = &radInterface;
	}
	
	void CaDataDicGen::SetCommunicationForm( CR2CP_Eth *Type)
	{
		m_Type_ =  Type;
	}
	
	byte CaDataDicGen::GetNodeEvent(bool IsFromCp)
	{
		byte Node =  m_Current_Node_Dest_;

		if(!m_IsGetRequest_Pending_ && !IsFromCp)//Pendiente un set y solicita un evento
			Node = BROADCAST_NODE_ID;
		
		if(IsFromCp)
			m_IsGetRequest_Pending_ = false;
		
		return Node;
	}
	
	void CaDataDicGen::SetNodeEvent(byte Node, bool IsFromGet)
	{
		if(IsFromGet)
			m_IsGetRequest_Pending_ = true;
		else
			m_IsGetRequest_Pending_ = false;
	
		m_Current_Node_Dest_ = Node;
	}
	
	void CaDataDicGen::Lock_DataDicGenMutex(void)
	{
        //OS::Mutex::Lock(datadic_mutex);
	}
	
	void CaDataDicGen::UnLock_DataDicGenMutex(void)
	{
        //OS::Mutex::UnLock(datadic_mutex);
	}

	
	///////////////////////////////////  R2CP   ///////////////////////////////////
	void CaDataDicGen::R2CP_HwVersion(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo) 
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte data[sizeof(tHwVersion)];
		byte pDataCp[] = {  MessageInfo->Sequence , Cp_Ok };
			
		data[0] = m_p_instance_->mHwVersion.Model >> 8 & 0xFF;
		data[1] = m_p_instance_->mHwVersion.Model & 0xFF;
		data[2] = m_p_instance_->mHwVersion.Version;
		data[3] = m_p_instance_->mHwVersion.Revision;
		
		(void)m_Type_->Event_Answer(	(eEthPriorities)MessageInfo->Priority,
										MessageInfo->Node_dest,
										m_p_instance_->mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_HW_VERSION,
										sizeof(data),
										data);
	
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_HW_VERSION,
									sizeof(pDataCp),
									pDataCp);
	}
	
	void CaDataDicGen::R2CP_SwVersion(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo) 
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte data[sizeof(tSwVersion)];
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
	
		data[0] = m_p_instance_->mSwVersion.Version;
		data[1] = m_p_instance_->mSwVersion.Revision;
		data[2] = m_p_instance_->mSwVersion.Subrevision;
	
		(void)m_Type_->Event_Answer(	(eEthPriorities)MessageInfo->Priority,
										MessageInfo->Node_dest,
										m_p_instance_->mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_SW_VERSION,
										sizeof(data),
										data);
	
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_SW_VERSION,
									sizeof(pDataCp),
									pDataCp);
	}
	
	
	void CaDataDicGen::R2CP_Error(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo) {
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		switch (Access)
		{
		case DATADIC_ACCESS_SET:
			if(m_p_R2CPInterface_)
			{
				m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
				pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_Clean_Error();
			}
			break;
	
		case DATADIC_ACCESS_GET:
			if(m_p_R2CPInterface_)
			{
				m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
				pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_Get_Error();
			}
			break;
		default:
			break;
		}
	
		(void)m_Type_->Processed((eEthPriorities)	MessageInfo->Priority,
													m_p_instance_->GetNodeEvent(true),
													m_p_instance_->mNodeId,
													R2CP_COMMANDS_ENTRY,
													R2CP_ERROR_CODE,
													sizeof(pDataCp),
													pDataCp);
	}
	
	void CaDataDicGen::R2CP_Error_Event(byte Error)
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),
										mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_ERROR_CODE,
										sizeof(byte),
										&Error);
	
	}
	
	
	void CaDataDicGen::R2CP_Status(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo) 
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
		
		m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
		m_p_instance_->R2CP_Status_Event();
			
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									m_p_instance_->GetNodeEvent(true),
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_STATUS,
									sizeof(pDataCp),
									pDataCp);
	}
	
	void CaDataDicGen::R2CP_Status_Event(void)
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),
										mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_STATUS,
										sizeof(tDataDicStatusRegister),
										(byte *)&m_p_instance_->mStatusRegister);
	}
	
	void CaDataDicGen::R2CP_Reset(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(	MessageInfo == nullptr )
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if( m_p_R2CPInterface_ )
			pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_Reset();
	
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									BROADCAST_NODE_ID,
									R2CP_COMMANDS_ENTRY,
									R2CP_RESET,
									sizeof(pDataCp),
									pDataCp);
	}
	
	void CaDataDicGen::R2CP_MasterLifeTime(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(	MessageInfo == nullptr )
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(	m_p_R2CPInterface_ )
			pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_MasterLiveTime(pData[1] << 8 | pData[0]);
	
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									BROADCAST_NODE_ID,
									R2CP_COMMANDS_ENTRY,
									R2CP_MASTER_LIFE_TIME_OUT,
									sizeof(pDataCp),
									pDataCp);
	}
	
	void CaDataDicGen::R2CP_ProtocolVersion( tDataDicAccess Access , byte *pData , word nData , tInfoMessage *MessageInfo ) 
	{
		if(	MessageInfo == nullptr )
			return;
	
		byte data[sizeof(tDataDicProtocolVersion)];
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
		byte NodeDest  = MessageInfo->Node_dest;
	
		switch( Access )
		{
		case DATADIC_ACCESS_SET:
		{
			pDataCp[ 1 ]  = Cp_MessageNotAvailable;
			NodeDest = BROADCAST_NODE_ID;
			
			//Controlamos solo la version y revision.
			for( byte i = 0 ; i < sizeof( ProtocolVersionSupported ) / sizeof( tDataDicProtocolVersion ) ; i++ )
			{
				if( reinterpret_cast< tDataDicProtocolVersion *>( pData )->Version != ProtocolVersionSupported[ i ].Version )
					continue;
				if( reinterpret_cast< tDataDicProtocolVersion *>( pData )->Revision > ProtocolVersionSupported[ i ].Revision  )
					continue;
				
				m_p_instance_->mProtocolVersion = *(reinterpret_cast< tDataDicProtocolVersion *>( pData ));
				pDataCp[ 1 ] = Cp_Ok;
			}
		}
			break;
		case DATADIC_ACCESS_GET:
		default:
			break;
		}
		
		data[0] = m_p_instance_->mProtocolVersion.Version;
		data[1] = m_p_instance_->mProtocolVersion.SubVersion;
		data[2] = m_p_instance_->mProtocolVersion.Revision;
		
		m_fcb_log_( 1,"PROTOCOL VERSION [ V%02dR%02d.%c ]", m_p_instance_->mProtocolVersion.Version , 
														    m_p_instance_->mProtocolVersion.SubVersion , 
														    m_p_instance_->mProtocolVersion.Revision );
		
		
		(void)m_Type_->Event_Answer(	(eEthPriorities)MessageInfo->Priority,
										NodeDest,
										MessageInfo->Node_own,
										R2CP_COMMANDS_ENTRY,
										R2CP_PROTOCOL_VERSION,
										sizeof(data),
										(data));
	
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									MessageInfo->Node_own,
									R2CP_COMMANDS_ENTRY,
									R2CP_PROTOCOL_VERSION,
									sizeof(pDataCp),
									(pDataCp));
	}
	
	void CaDataDicGen::R2CP_BootVersion(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(	MessageInfo == nullptr )
			return;
	
		byte data[sizeof(tDataDicSwVersion)];
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
	
		data[0] = m_p_instance_->mBootVersion.Version;
		data[1] = m_p_instance_->mBootVersion.Revision;
		data[2] = m_p_instance_->mBootVersion.Subrevision;
	
		(void)m_Type_->Event_Answer(	(eEthPriorities)MessageInfo->Priority,
										MessageInfo->Node_dest,
										MessageInfo->Node_own,
										R2CP_COMMANDS_ENTRY,
										R2CP_BOOT_VERSION,
										sizeof(data),
										reinterpret_cast<byte *>(data));
					
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									MessageInfo->Node_own,
									R2CP_COMMANDS_ENTRY,
									R2CP_BOOT_VERSION,
									sizeof(pDataCp),
									pDataCp);
	}
		
		
	
	void CaDataDicGen::R2CP_PackageVersion(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(	MessageInfo == nullptr )
			return;		
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
				
#if 1
		const word MAX_STRING_SERIALNUMBER = 1000;
		m_p_instance_->m_pPackageVersion = reinterpret_cast<char *>( /*BOARD_A3687_H_RAM_ALLOCATOR_MEMORY*/0x14000000 );
		memset( m_p_instance_->m_pPackageVersion , 0 , MAX_STRING_SERIALNUMBER );
#else
		const word MAX_STRING_SERIALNUMBER = 1000;
		if(m_p_instance_->m_pPackageVersion)
		{
			delete []  m_p_instance_->m_pPackageVersion ;
			m_p_instance_->m_pPackageVersion = nullptr;
		}
			
		m_p_instance_->m_pPackageVersion = new char[ MAX_STRING_SERIALNUMBER ];
		memset( m_p_instance_->m_pPackageVersion , 0 , MAX_STRING_SERIALNUMBER );
#endif
		
		if(	m_p_R2CPInterface_ )
		{
			m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
			pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_PackageVersion(&m_p_instance_->m_pPackageVersion[HEADER_SIZEOF]);
		}
#if 0
		delete [] m_p_instance_->m_pPackageVersion;
		m_p_instance_->m_pPackageVersion = nullptr;
#endif
		
					
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY,
									m_p_instance_->GetNodeEvent(true),
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_PACKAGE_VERSION,
									sizeof(pDataCp),
									(byte *)pDataCp);
	}
	
	void CaDataDicGen::R2CP_PackageVersion_Event(char *pStringPackageVersion)
	{
		m_Type_->SendRawData(	ETH_LOWEST_PRIORITY, 
								GetNodeEvent(), 
								mNodeId , 
								R2CP_COMMANDS_ENTRY , 
								R2CP_PACKAGE_VERSION,
								ETH_EVENT_ANSWER_FNC,
								strlen(pStringPackageVersion),
								(byte *)(pStringPackageVersion - HEADER_SIZEOF));
					
	}
	
	void CaDataDicGen::R2CP_SerialNumber(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(	MessageInfo == nullptr )
			return;		
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
		
		byte Node = MessageInfo->Node_dest;
		
		switch (Access)
		{
		case DATADIC_ACCESS_SET:
			if(m_p_R2CPInterface_)
			{
				Node = 0;
				pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_SerialNumber(pData,nData);
			}
			break;
		}

			
		(void)m_Type_->Event_Answer(	(eEthPriorities)MessageInfo->Priority,
										Node,
										m_p_instance_->mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_SERIAL_NUMBER,
										strlen(m_p_instance_->m_pSerialNumber) + 1,
										(byte *)m_p_instance_->m_pSerialNumber);
					
		(void)m_Type_->Processed(	(eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_SERIAL_NUMBER,
									sizeof(pDataCp),
									(pDataCp));
	}
	
	void CaDataDicGen::R2CP_Description( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(	MessageInfo == nullptr )
			return;		
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
	
	
		(void)m_Type_->Event_Answer(	(eEthPriorities)MessageInfo->Priority,
										MessageInfo->Node_dest,
										m_p_instance_->mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_DESCRIPTION,
										strlen(m_p_instance_->m_pDescription),
										(byte *)m_p_instance_->m_pDescription);
				
		(void)m_Type_->Processed( (eEthPriorities)MessageInfo->Priority,
									MessageInfo->Node_dest,
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_DESCRIPTION,
									sizeof(pDataCp),
									(pDataCp));
	}
	
	void CaDataDicGen::R2CP_SystemVersion( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(	MessageInfo == nullptr )
			return;		
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(	m_p_R2CPInterface_ )
		{
			m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );
			pDataCp[1] = m_p_R2CPInterface_->II_R2CP_SS_SystemVersion( );
		}
					
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY,
									m_p_instance_->GetNodeEvent(true),
									m_p_instance_->mNodeId,
									R2CP_COMMANDS_ENTRY,
									R2CP_SYSTEM_VERSION,
									sizeof(pDataCp),
									(byte *)pDataCp);
	}
	
	void CaDataDicGen::R2CP_SystemVersion_Event( tSystemVersion *pSystemVersion )
	{
		byte Data[] = {
				pSystemVersion->Index,
				(pSystemVersion->Hardware_Model & 0xFF00) >> 8,
				(pSystemVersion->Hardware_Model & 0x00FF),
				pSystemVersion->Hardware_Version,
				pSystemVersion->Hardware_Revision,
				pSystemVersion->SwVersion,
				pSystemVersion->SwRevision,
				pSystemVersion->SwSubrevision,
				pSystemVersion->BootVersion,
				pSystemVersion->BootRevision,
				pSystemVersion->BootSubrevision,
				pSystemVersion->HdlVersion,
				pSystemVersion->HdlRevision,
				pSystemVersion->HdlSubrevision,
				pSystemVersion->GoldenVersion,
				pSystemVersion->GoldenRevision,
				pSystemVersion->GoldenSubrevision,
				pSystemVersion->FieldToApply.value
		};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),
										mNodeId,
										R2CP_COMMANDS_ENTRY,
										R2CP_SYSTEM_VERSION,
										sizeof(Data),
										(byte *)Data);
	}
	
	bool CaDataDicGen::Check_NotAvailable_Message_vs_Status( word index, word subindex, tInfoMessage *MessageInfo )
	{
		if(m_p_DataDicInterface_)
			return m_p_DataDicInterface_->CheckNotAvailableMessages_vs_Status( index , subindex );
		return true;	
	}
	
	//Por defecto lo enviamos al powerControl
	bool CaDataDicGen::ReservedGot( byte Priority, byte Node , byte Index , byte SubIndex , byte *pData , word Size )
	{
		if(m_p_R2CPInterface_)
			m_p_R2CPInterface_->ReservedGot( Priority , Node , Index , SubIndex , pData , Size );
	}
	

}//namespace R2CP
