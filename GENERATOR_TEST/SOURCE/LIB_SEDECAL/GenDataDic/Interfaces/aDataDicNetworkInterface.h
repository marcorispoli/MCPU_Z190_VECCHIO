#ifndef __ADATADICNETWORKINTERFACE_H__
	#define __ADATADICNETWORKINTERFACE_H__

#include "aDataDicInterface.h"


namespace R2CP
{
	typedef struct
	{		
		union BACKSNAPSHOT_Log_{
			struct{
				byte FlagEventLog		: 1;
				byte FlagExposureLog	: 1;
				byte FlagErrorLog		: 1;
				byte Spare				: 5;
			}Fields;
			byte Value;
		}Log;
		
		union BACKSNAPSHOT_CNGCAL_{
			struct{
				byte Configuration				: 1;
				byte SystemCalibration			: 1;
				byte FilamentCalibration_Tube1	: 1;
				byte Factory					: 1;
				byte License					: 1;
				byte ExposureCounter			: 1;
				byte FilamentCalibration_Tube2	: 1;
				byte Spare_2					: 1;
			}Fields;
			byte Value;	
		}ConfigAndCalibration;
	}tBackupAndSnapShot;
	
	typedef enum{
		BackupSnapStatus_Start = 1,
		BackupSnapStatus_InProgress,
		BackupSnapStatus_Finished,
		BackupSnapStatus_Error
	}tBackupSnapShotStatus;
	
	typedef	union RESTORE_
	{
		struct{
			byte Configuration 				: 1;
			byte GeneratorCalibration		: 1;
			byte PositionerCalibration		: 1;
			byte FilamentCalibration_Tube1	: 1;
			byte FilamentCalibration_Tube2  : 1;
		}Fields;
		byte Value;
	}tRestore;
	
	typedef enum{
		Restore_Start = 1,
		Restore_InProgress,
		Restore_Finished,
		Restore_Error
	}tRestoreStatus;
	

	class CaDataDicNetworkInterface
	{
	public:
		/*!
		 * \brief Default Constructor
		 */
		CaDataDicNetworkInterface(){};
			
		/*!
		 * \brief Destructor
		 */
		virtual ~CaDataDicNetworkInterface(){};
		
		/*!
		 * \brief Connection Changed
		 */
		virtual byte II_Network_SS_ConnectionChanged(byte NodeId, byte ConnectionStatus) { return Cp_MessageNotAvailable;}
		/*!
		 * \brief Connection Status 
		 */
		virtual byte II_Network_SS_NodeStatus(byte NodeId , byte ConnectionStatus) { return Cp_MessageNotAvailable;}
		/*!
		 * \brief Sends new Connection Request
		 */
		virtual byte II_Network_SS_NetworkConnectionRequest(byte NodeId) { return Cp_MessageNotAvailable; }
		
		enum{
			AddressOutOfRange = 2
		};
		
		/*!
		 * \brief Command to modify and check ip configuration of the node.
		 */
		virtual byte II_Network_SS_IpConfig(udword IpAddress, udword MaskSubNetwork , udword Gateway , udword ShIpAdress) { return Cp_MessageNotAvailable; }
		/*!
		 * \brief Command to gets network configuration.
		 */
		virtual byte II_Network_SS_IpConfig(void) { return Cp_MessageNotAvailable; }
		
		/*
		 * \brief 	The device that modifies any file of the share configuration folder, 
		 * 			should notify with this message to the rest of system. After the reception of this EVENT, every component can:
		 *					-	check what file has changed, download it, update information if possible
		 *					-	download are files, and update information if possible
		 */
		virtual byte II_Network_SS_FileConfigUpdate(void){ return Cp_MessageNotAvailable; }
				
		/*!
		 * \brief	The intended use of this command is to archive the current system logs and configuration and calibration files when the operator requests it.
		 * 			The workflow is as follows:
		 *				1.	The operator uses the Generate Snapshot option in one console
		 *				2.	That console checks the nodes of type “Console”, “Hub”, “Positioner”, “Generator” and “Image system” 
		 *					connected to the system and sends them the SnapShotSETMessage indicating the action to be performed
		 *				3.	Each node which receives the SnapShotSETMessage initiates their snapshot sequence and notifies it sending a broadcast SnapShotEVENTMessage with the “Status=InProgress”
		 *				4.	Each note notifies the result of their snapshot sequence sending a broadcast SnapShotEVENTMessage with the “Status=Finished” or “Status=Error”. 
		 *					In case of Error the node must send a SystemMessage indicating the error 
		 * 			
		 */
		virtual  byte II_Network_SS_BackupSnapShot(tBackupAndSnapShot BackupAndSnapShot){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief 
		 */
		virtual byte II_Network_SS_Restore( tRestore Restore ) { return Cp_MessageNotAvailable; }
	};
};
#endif
