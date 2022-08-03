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
#include "CaDataDicGen.h"

namespace R2CP
{
	
	/******************************************************************************************************************/
	//												PATIENT
	/******************************************************************************************************************/
	void CaDataDicGen::Patient_Procedure_Definition(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_instance_->m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET PROCEDURE DEFINITION [ %d ] --> PROCEDURE_ID [ %d ] TYPE [ %d ] WS [ %d ]", nData , pData[0] , pData[1] , pData[5] );
					}

					tProcedure Procedure[] = { pData[0], //Procedure ID
											   pData[1], //Generator Procedure Type
											   pData[3], //Handswitch / Footswitch Id
											   pData[4], //Activate When Handswitch/Footswitch is pressed
											   pData[5], //Workstation Id
											   pData[6], //Total Number of Exposure Data Banks in Procedure
											   pData[10], //Generator Data Bank Sequencing
											   0,
											   0,
											   PATIENT_PROCEDURE_DEFINITION
											  };
	
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_ProcedureDefinition(  reinterpret_cast<tProcedure *>(&Procedure)  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET PROCEDURE DEFINITION [ %d ] --> PROCEDURE_ID [ %d ]", nData , pData[0] );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_ProcedureDefinition( static_cast<byte>(pData[0]) );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_PROCEDURE_DEFINITION, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	void CaDataDicGen::Patient_Procedure_NExposures_Definition( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_instance_->m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET PROCEDURE NEXPOSURES DEFINITION [ %d ] --> PROCEDURE_ID [ %d ] TYPE [ %d ] WS [ %d ]", nData , pData[0] , pData[1] , pData[5] );
					}

					tProcedure Procedure[] = { pData[0], //Procedure ID
											   pData[1], //Generator Procedure Type
											   pData[3], //Handswitch / Footswitch Id
											   pData[4], //Activate When Handswitch/Footswitch is pressed
											   pData[5], //Workstation Id
											   pData[6], //Total Number of Exposure Data Banks in Procedure
											   pData[10], //Generator Data Bank Sequencing
											   pData[15],
											   pData[14],
											   PATIENT_PROCEDURE_NEXPOSURES_DEFINITION
											  };
	
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_ProcedureDefinition(  reinterpret_cast<tProcedure *>(&Procedure)  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET PROCEDURE DEFINITION NEXPOSURES [ %d ] --> PROCEDURE_ID [ %d ]", nData , pData[0] );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_ProcedureDefinition( static_cast<byte>(pData[0]) );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_PROCEDURE_NEXPOSURES_DEFINITION, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Procedure_Activate( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{		
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET PROCEDURE ACTIVATE [ %d ] --> PROCEDURE_ID [ %d ] COMMAND [ %d ] EXP_NUMBER [ %d ]", nData , pData[0] , pData[1] , pData[2] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_ProcedureActivateDatabank( 	pData[0]/*ProcedureID*/, 
																									pData[1] /*Command*/ ,  
																									pData[2] /*Exposure Data Bank Sequence Number*/);
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET PROCEDURE ACTIVATE [ %d ] --> PROCEDURE_ID [ %d ] ", nData , pData[0] );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_ProcedureActivateDatabank( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
												m_p_instance_->GetNodeEvent(true), 
												m_p_instance_->mNodeId, 
												PATIENT_COMMANDS_ENTRY, 
												PATIENT_PROCEDURE_ACTIVATE, 
												sizeof(pDataCp), 
												(pDataCp));
	}
	
	void CaDataDicGen::Patient_Procedure_Default( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET PROCEDURE DEFAULT [ %d ] --> PROCEDURE_ID [ %d ] COMMAND [ %d ] EXP_NUMBER [ %d ]", nData , pData[0] , pData[1] , pData[2] );
					}
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_Procedure_Default( 	pData[0]/*ProcedureID*/, 
																							pData[1] /*Command*/ ,  
																							pData[2] /*Exposure Data Bank Sequence Number*/);
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET PROCEDURE DEFAULT [ %d ]", nData );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_Procedure_Default();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_PROCEDURE_DEFAULT, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Patient_Procedure_ClearAll( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET CLEARALL [ %d ]", nData);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_Procedure_ClearAll();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_PROCEDURE_CLEAR_ALL, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Patient_Redefine_HandFootswitchActivation( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET HANDSWITCH ACTIVATION [ %d ] --> PROCEDURE_ID [ %d ] HANDSWITCH_ID [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_Patient_Redefine_HandFootswitchActivation( 	pData[0]/*ProcedureID*/, 
																													pData[1] /*Handswitch/Footswitch is pressed*/);
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_REDEFINE_HANDFOOTSWITCH_ACTIVATION, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Redefine_Workstation( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET REDEFINE WS [ %d ] --> PROCEDURE_ID [ %d ] WS_ID [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_Redefine_Workstation(    pData[0]/*ProcedureID*/, 
																				   	   	   	   pData[1] /*Workstation*/);
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_REDEFINE_WORKSTATION, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Miscellaneous_Sync_Up( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET SYNC_UP [ %d ]", nData);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_SyncUp();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
												m_p_instance_->GetNodeEvent(true), 
												m_p_instance_->mNodeId, 
												PATIENT_COMMANDS_ENTRY, 
												PATIENT_MISCELLANEOUS_SYNC_UP, 
												sizeof(pDataCp), 
												(pDataCp));
	}
	
	void  CaDataDicGen::Patient_Miscellaneous_WorkingMode(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET APP MODE [ %d ] --> MODE [ %d ] NODE [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_WorkingMode(pData[0] , pData[1]);
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET APP MODE [ %d ]", nData );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_WorkingMode();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_MISCELLANEOUS_WORKINGMODE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Miscellaneous_Pos_Cal_Number(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET POSITIONER NUMBER [ %d ] ", pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Positioner_Calibration_Number( pData[0] , MessageInfo->Sequence );
					if( pDataCp[1] != Cp_Ok )
						return;
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET POSITIONER CAL NUMBER" );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_PatientInterface_->II_Positioner_Calibration_Number();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_MISCELLANEOUS_POSITIONER_CALIBRATION_NUMBER, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Miscellaneous_PowerOff(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET POWEROFF [ %d ] --> MS [ %d ] ", nData , pData[0] << 8 | pData[1] );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_PowerOff( static_cast< unsigned short >( pData[0] <<8 | pData[1] ));
				}
				break;
				case DATADIC_ACCESS_ANSWER_EVENT:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] EVENT POWEROFF [ %d ] --> MS [ %d ] ", nData , pData[0] << 8 | pData[1] );
					}
					
					m_p_PatientInterface_->II_Patient_SS_PowerOff();
					return;
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_MISCELLANEOUS_POWEROFF, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Miscellaneous_PowerOffRequest( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_ANSWER_EVENT:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] EVENT POWEROFF_REQUEST [ %d ] --> TYPE [ %d ] ", nData , pData[0] );
					}
					
					(void)m_p_PatientInterface_->II_Patient_SS_PowerOffRequest( static_cast< tPowerOffType >( pData[0] ));
				}
				break;
			}
		}
	}
	
	void CaDataDicGen::Patient_Miscellaneous_PowerOffCancel(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET POWEROFF_CANCEL [ %d ]", nData );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_PowerOffCancel();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_MISCELLANEOUS_POWEROFF_CANCEL, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Patient_Miscellaneous_PowerOffConfirm(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_PatientInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET POWEROFF_CONFIRM [ %d ] --> TYPE [ %d ]", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_PatientInterface_->II_Patient_SS_PowerOffConfirm( static_cast< tPowerOffType >( pData[0] ));
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									PATIENT_COMMANDS_ENTRY, 
									PATIENT_MISCELLANEOUS_POWEROFF_CONFIRM, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	/*********************************************************EVENTS******************************************************************/
	void CaDataDicGen::Patient_Miscellaneous_WorkingMode_Event(byte WorkingMode , byte Node)
	{
		byte Data[] = { WorkingMode , Node };
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId, 
										PATIENT_COMMANDS_ENTRY, 
										PATIENT_MISCELLANEOUS_WORKINGMODE, 
										sizeof(Data), 
										Data);
	}
	
	void CaDataDicGen::Patient_Miscellaneous_Positioner_Calibration_Number_Event( byte Pos_Id , bool SendCp , byte CommandProcessedCode , byte Sequence )
	{
		byte Data[] = { Pos_Id };
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId, 
										PATIENT_COMMANDS_ENTRY, 
										PATIENT_MISCELLANEOUS_POSITIONER_CALIBRATION_NUMBER, 
										sizeof(Data), 
										Data);
		
		if( SendCp )
		{			
			byte pDataCp[] = { Sequence , CommandProcessedCode };
			
			(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
										m_p_instance_->GetNodeEvent(true), 
										m_p_instance_->mNodeId, 
										PATIENT_COMMANDS_ENTRY, 
										PATIENT_MISCELLANEOUS_POSITIONER_CALIBRATION_NUMBER, 
										sizeof(pDataCp), 
										(pDataCp));
		}
	}
	

	
	void CaDataDicGen::Patient_Miscellaneous_PowerOffCancel_Event(void)
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId, 
										PATIENT_COMMANDS_ENTRY, 
										PATIENT_MISCELLANEOUS_POWEROFF_CANCEL, 
										0, 
										nullptr);
	}
	
	void CaDataDicGen::Patient_Miscellaneous_PowerOff_Request_Event( tPowerOffType Type )
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										R2CP_ETH_BROADCAST_NODE_ID, 
										mNodeId, 
										PATIENT_COMMANDS_ENTRY, 
										PATIENT_MISCELLANEOUS_POWEROFF_REQUEST, 
										sizeof(byte), 
										reinterpret_cast<byte *>(&Type));
		
	}

    void CaDataDicGen::Patient_SetupProcedure(byte num){

        if((num < 1) || (num > 4)) return;

        byte pData[14];

        if(num == 1){ // Standard 2D
            pData[0] = num; // Proc Id
            pData[1] = 1;   // Proc Type = standard Rad
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = 1;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 2;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq

        }else if(num == 2){ // AEC 2D
            pData[0] = num; // Proc Id
            pData[1] = 1;   // Proc Type = standard Rad
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = 1;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 2;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 2;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq

        }else if(num == 3){ // Tomo
            pData[0] = num; // Proc Id
            pData[1] = 3;   // Proc Type = Tomo
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = 1;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 1;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 2;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq

        }else if(num == 4){ // Tomo AEC
            pData[0] = num; // Proc Id
            pData[1] = 3;   // Proc Type = Tomo
            pData[2] = 0;   // Positioner = not defined
            pData[3] = 1;   // Handswitch ID = not defined
            pData[4] = 1;   // Activation Mode: 1 = Push, 0 = Software
            pData[5] = 1;   // Workstation: 1 = Detector, 5 = Direct
            pData[6] = 2;   // Num Databank
            pData[7] = 1;   // Positioning Databank
            pData[8] = 1;   // Collimator Databank
            pData[9] = 1;   // Filter Databank
            pData[10] = 2;  // Gen Databank sequencing: 0-NA, 1-Activate Next, 2-Software
            pData[11] = 0;  // Posi. Seq
            pData[12] = 0;  // Colli. Seq
            pData[13] = 0;  // Filter. Seq
        }

        (void)m_Type_-> Set(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                PATIENT_COMMANDS_ENTRY,
                                PATIENT_PROCEDURE_DEFINITION,
                                14,
                                pData);

    }



}//namesapce R2Cp	
