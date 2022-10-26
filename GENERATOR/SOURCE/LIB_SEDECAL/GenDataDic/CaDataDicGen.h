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
        static void R2CP_ProtocolVersion	(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo = nullptr);

		
        ////////////////////////////////////GENERATOR INDEX////////////////////////////////////////////////////////////
        static void Generator_DataBank_ExposureAcceptance				( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_DataBank_DefineProcedure                  ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_RadDataBank_Load							( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_ExposureManagement_GeneratorStatus		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);
        static void Generator_ExposureManagement_RadPostExposure		( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);


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
        //static void ImageSystem_CurrentLsb			( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo = nullptr);

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
        void SystemMessages_SetDisableRx(bool state);

        bool SystemMessages_Clear_Message(uint ID);


        void Patient_SetupProcedureV5(byte num);
        void Patient_SetupProcedureV6(byte num);
        void Patient_ClearAllProcedures(void);
        void Generator_Set_2D_Databank(uchar i, uchar focus, float kV, uint mAs);

        void Patient_Activate2DProcedurePulse(bool detector, bool grid);

        void Patient_Activate2DAecProcedurePulse(void);
        void Patient_Activate2DAecProcedurePre(void);

        void Generator_AssignDbToProc(uint8_t db, uint8_t proc, uint8_t index);
        void Generator_verifyDbToProc(uint8_t proc, uint8_t index);
        void Generator_startExposure(void);
        void Generator_stopExposure(void);



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
