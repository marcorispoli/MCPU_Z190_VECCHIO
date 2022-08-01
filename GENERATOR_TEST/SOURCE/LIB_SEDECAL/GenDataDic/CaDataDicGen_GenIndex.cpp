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
#include "communication.h"

extern Communication* pComm;

/******************************************************************************************************************/
//												GENERATOR
/******************************************************************************************************************/

namespace R2CP
{
    void CaDataDicGen::Generator_ExposureManagement_GeneratorStatus( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
    {
        if(MessageInfo == nullptr)  return;

        if( MessageInfo->SubIndex == GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5 	&& m_p_instance_->mProtocolVersion.Version != PROTOCOL_VERSION_5 ||
            MessageInfo->SubIndex == GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V6  && m_p_instance_->mProtocolVersion.Version != PROTOCOL_VERSION_6 )
        {
            return;
        }

        if(!m_p_RadInterface_) return;

        switch( Access )
        {
            case DATADIC_ACCESS_ANSWER_EVENT:
            {
                m_p_RadInterface_->II_Generator_SS_Status(pData);
                pComm->emit_generatorStatusSgn();
            }
            break;
        }

    }



    // Get Functions
    void CaDataDicGen::Generator_Get_Status(void){
        (void)m_Type_-> Get(    ETH_LOWEST_PRIORITY,
                                GENERATOR_NODE_ID,
                                mNodeId,
                                GENERATOR_COMMANDS_ENTRY,
                                GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5,
                                0,
                                nullptr);
    }

}//namespace R2CP




/*
namespace R2CP
{
	void CaDataDicGen::Generator_DataBank_AssignExposure( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	

		
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:				
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET ASSIGN LEN [ %d ] --> PROCEDURE_ID [ %d ] EXPSEQNUMBER [ %d ] EXPDBID [ %d ]", nData , pData[0] , pData[1] , pData[2]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_DBAssignExposure( pData[0] , pData[1] , pData[2]);
					break;
				case DATADIC_ACCESS_GET:
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 , "[ R2CP DATADIC ] GET ASSIGN LEN [ %d ]  --> PROCEDURE_ID [ %d ] EXPSEQNUMBER [ %d ]", nData, pData[0] , pData[1] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_DBAssignExposure( pData[0] , pData[1] );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_COMMANDS_ENTRY, 
								   GENERATOR_DATA_BANK_ASSIGN_EXPOSURE, 
								   sizeof(pDataCp), 
								   (pDataCp));
	}
	
	void CaDataDicGen::Generator_DataBank_ExposureAcceptance(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET ACCEPTANCE LEN [ %d ] --> PROCEDURE_ID [ %d ] EXPSEQNUMBER [ %d ]", nData , pData[0] , pData[1] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_DBExposureAcceptance( pData[0] , pData[1] );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_COMMANDS_ENTRY, 
								   GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE, 
								   sizeof(pDataCp), 
								   (pDataCp));
	}
					
	void CaDataDicGen::Generator_RadDataBank_Load(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if( MessageInfo->SubIndex == GENERATOR_RAD_DATA_BANK_LOAD_V5 && m_p_instance_->mProtocolVersion.Version != PROTOCOL_VERSION_5 || 
			MessageInfo->SubIndex == GENERATOR_RAD_DATA_BANK_LOAD_V6 && m_p_instance_->mProtocolVersion.Version != PROTOCOL_VERSION_6 )
		{
			(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
									   m_p_instance_->GetNodeEvent( true ), 
									   m_p_instance_->mNodeId, 
									   GENERATOR_COMMANDS_ENTRY, 
									   MessageInfo->SubIndex, 
									   sizeof(pDataCp), 
									   (pDataCp));
			return;
		}
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{	
					tRadDb RadDatabank;
					RadDatabank.DatabankId										= pData[0];
					RadDatabank.ImagingSystemProtocolId							= pData[1];
					RadDatabank.TechMode.value									= pData[2];
					RadDatabank.kV10.value										= pData[3] << 8 | pData[4];
					RadDatabank.mAs1000.value									= (dword)(pData[5] <<16 | (dword)pData[6] <<8 | pData[7]);
					RadDatabank.mA100.value										= (dword)(pData[8] <<16 | (dword)pData[9] <<8 | pData[10]);
					RadDatabank.ms100.value										= (dword)(pData[11]<<16 | (dword)pData[12]<<8 | pData[13]);
					RadDatabank.MinIntegrationTime.value						= (word)(pData[14]<<8 | pData[15]);
					RadDatabank.MaxIntegrationTime.value						= (word)(pData[16]<<8 | pData[17]);
					RadDatabank.FocalSpot										= pData[18];
					RadDatabank.TargetDose.Fields.AECSensitivity_DoseTarget 	= pData[19];
					RadDatabank.TargetDose.Fields.AECDensity					= pData[20];
					RadDatabank.IonChamber.value								= pData[21];
					RadDatabank.Spare_1											= pData[22];
					RadDatabank.FPS10.value										= (word)( pData[ 23 ] << 8 | pData[ 24 ]);
					RadDatabank.TrakingId										= pData[25];
					RadDatabank.Spare_2											= pData[26];
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET RAD LOAD LEN [ %d ] --> DB_ID [ %d ] KV10 [ %d ] MAS1000 [ %d ] MA100 [ %d ] MS100 [ %d ]"	 , nData 
																																										 , RadDatabank.DatabankId
																																										 , RadDatabank.kV10.value 
																																										 , RadDatabank.mAs1000.value
																																										 , RadDatabank.mA100.value	
																																										 , RadDatabank.ms100.value);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					

					if(RadDatabank.kV10.value < MIN_KVP10_ALLOWED || RadDatabank.kV10.value > MAX_KVP10_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_KVpValueOutOfRange;
						break;		
					}
					
					if(RadDatabank.mAs1000.value < MIN_MAS1000_ALLOWED || RadDatabank.mAs1000.value > MAX_MAS1000_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_MasValueOutOfRange;
						break;	
					}
					
					if(RadDatabank.mA100.value < MIN_MA100_ALLOWED || RadDatabank.mA100.value > MAX_MA100_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_MaValueOutOfRange;
						break;	
					}
					
					if(RadDatabank.ms100.value < MIN_MS100_ALLOWED || RadDatabank.ms100.value > MAX_MS100_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_MsValueOutOfRange;
						break;	
					}
					
					if(RadDatabank.MinIntegrationTime.value < MIN_MIT_ALLOWED || RadDatabank.MinIntegrationTime.value > MAX_MIT_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_MinimumIntegrationTimeValueOutOfRange;
						break;	
					}
					
					if(RadDatabank.MaxIntegrationTime.value < MIN_MAT_ALLOWED || RadDatabank.MaxIntegrationTime.value > MAX_MAT_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_MaximumIntegrationTimeValueOutOfRange;
						break;	
					}
					
					if(RadDatabank.FocalSpot > 0x02)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_FocalSpotCodeOutOfRange;
						break;
					}
					
					
					if(RadDatabank.FPS10.value > MAX_FPS10_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBL_FPSValueOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBLoad(&RadDatabank);
				}
					break;
				case DATADIC_ACCESS_GET:
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET RAD LOAD  LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0] );
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBLoad( pData[0] );
					break;
			}
		}
	
		(void)m_Type_->Processed( ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   GENERATOR_COMMANDS_ENTRY, 
								   MessageInfo->SubIndex, 
								   sizeof(pDataCp), 
								   (pDataCp));
	
	}
	
	void CaDataDicGen::Generator_RadDataBank_ProcedureAcceptance(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET PROCEDURE ACCEPTANCE LEN [ %d ] --> PROCEDURE_ID [ %d ] CHECKNUMEXP [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBProcedureAcceptance( pData[0] , pData[1]);
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_DATA_BANK_ACCEPTANCE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_TechniqueMode	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET TECNIQUE MODE LEN [ %d ] --> DB_ID [ %d ] TECHMODE [ %d ] AUTOTECH [ %d ] ADJUST [ %d ]", nData , pData[0], (pData[1] & 0x3F) , ((pData[1] >> 6 ) & 0x01) , ((pData[1] >> 7 ) & 0x01));
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);		
					
					if(static_cast<byte>(pData[1] & 0x3F) > 0x06)//Technique Mode
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBTM_TechniqueModeCodeOutOfRange;
						break;
					}
					
					if(static_cast<byte>(pData[2]) > 0x01)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;

					}
																									
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBExpParamTechniqueMode( 	pData[0],
																								static_cast<byte>(pData[1] & 0x3F) , 
																								static_cast<byte>((pData[1] >> 6 ) & 0x01) , 
																								static_cast<byte>((pData[1] >> 7 ) & 0x01) , 
																								pData[2] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET TECNIQUE MODE LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBExpParamTechniqueMode( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_TECHNIQUE_MODE, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_KVP(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET KVP LEN [ %d ] --> DB_ID [ %d ] KVP10 [ %d ]", nData , pData[0] , (pData[1] << 8 | pData[2]) );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if(static_cast<byte>(pData[3]) > 0x01)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if(static_cast<byte>(pData[3]) == 0x00)//Set Value
					{
						if(static_cast<word>(pData[1] << 8 | pData[2]) < MIN_KVP10_ALLOWED || static_cast<word>(pData[1] << 8 | pData[2]) > MAX_KVP10_ALLOWED)
						{
							pDataCp[1] = CaDataDicRadInterface::II_Generator_SS_RDBKV_KvpValueOutOfRange;
							break;
						}
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBKVP(  pData[0] , pData[1]<<8 | pData[2] , pData[3]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET KVP LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBKVP( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_KVP, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_MAS(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if(static_cast<byte>(pData[4]) > 0x01)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					long int mAs1000orStep = pData[1] << 16 | pData[2] << 8 | pData[3];
					
					if(static_cast<byte>(pData[4]) == 0)//Set Value
					{
						if(static_cast<dword>( mAs1000orStep ) < MIN_MAS1000_ALLOWED || static_cast<dword>( mAs1000orStep ) > MAX_MAS1000_ALLOWED)
						{
							pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_RDBMAS_MasValueOutOfRange;
							break;
						}
					}
					else
					{
						if( mAs1000orStep == 0xFFFFFF )
						{
							mAs1000orStep = -1;
						}
					}
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET MAS LEN [ %d ] --> DB_ID [ %d ] MAS1000 [ %d ]", nData , pData[0] , mAs1000orStep );
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDB_MAS(  pData[0] , mAs1000orStep , pData[4]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET MAS LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDB_MAS( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_MAS, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_MA(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{	

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if(static_cast<byte>(pData[4]) > 0x01)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					long int mA100orStep = pData[1]<<16 | pData[2]<<8 | pData[3];
					
					if( static_cast<byte>(pData[4]) == 0x00 )//Set Value
					{
						if( static_cast<dword>( mA100orStep ) < MIN_MA100_ALLOWED || static_cast<dword>( mA100orStep ) > MAX_MA100_ALLOWED)
						{
							pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBMAMaValueOutOfRange;
							break;
						}
					}
					else //Step
					{
						if( mA100orStep == 0xFFFFFF )// -1
						{
							mA100orStep = -1;
						}
					}
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET MA LEN [ %d ] --> DB_ID [ %d ] MA100 [ %d ]", nData , pData[0] , mA100orStep );
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMA(  pData[0] , mA100orStep , pData[4]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET MA LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMA( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_MA, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_MS(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if(static_cast<byte>(pData[4]) > 0x01)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					long int ms100orStep = pData[1]<<16 | pData[2]<<8 | pData[3];
					
					if(static_cast<byte>(pData[4]) == 0x00)//Set Value
					{
						if( static_cast<dword>( ms100orStep ) < MIN_MS100_ALLOWED || static_cast<dword>( ms100orStep ) > MAX_MS100_ALLOWED)
						{
							pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBMSMsValueOutOfRange;
							break;
						}
					}
					else
					{
						if( ms100orStep == 0xFFFFFF )
						{
							ms100orStep = -1;
						}
					}
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET MS LEN [ %d ] --> DB_ID [ %d ] MS100 [ %d ]", nData , pData[0] , ms100orStep );
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMS(  pData[0] , ms100orStep , pData[4]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET MS LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMS( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_MS, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_MinIntegrationTime(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if(static_cast<byte>(pData[3]) != 0x00)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if(static_cast<word>(pData[1]<<8 | pData[2]) < MIN_MIT_ALLOWED|| static_cast<word>(pData[1]<<8 | pData[2]) > MAX_MIT_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBMITMinIntegrationTimeValueOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMinIntTime(  pData[0] , pData[1]<<8 | pData[2] , pData[3]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMinIntTime( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_MIN_INTEGRATION_TIME, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_MaxIntegrationTime(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{		
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[3]) != 0x00)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if(static_cast<word>(pData[1]<<8 | pData[2]) < MIN_MAT_ALLOWED|| static_cast<word>(pData[1]<<8 | pData[2]) > MAX_MAT_ALLOWED)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBMATMaxIntegrationTimeValueOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMaxIntTime(  pData[0] , pData[1]<<8 | pData[2] , pData[3]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBMaxIntTime( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_MAX_INTEGRATION_TIME, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_FocalSpot(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{		
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET FOCALSPOT LEN [ %d ] --> DB_ID [ %d ] FOCALSPOT [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[2]) != 0x00 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if( static_cast<byte>(pData[1]) > 0x02)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBFFocalSpotCodeOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBFocalSpot(  pData[0] , pData[1] , pData[2]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET FOCALSPOT LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBFocalSpot( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_FOCAL_SPOT, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_AECsensitivity(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{	
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET AECSENSITIVITY LEN [ %d ] --> DB_ID [ %d ] SENSITIVITY [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[2]) != 0x00 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if( static_cast<byte>(pData[1]) > 0x02 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBAECSSensitivityCodeOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECsensitivity(  pData[0] , pData[1] , pData[2]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET AECSENSITIVITY LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECsensitivity( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_SENSITIVITY, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_AECdensity ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET AECDENSITY LEN [ %d ] --> DB_ID [ %d ] DENSITY [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[2]) != 0 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECdensity(  pData[0] , pData[1] , pData[2]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET AECDENSITY LEN [ %d ] --> DB_ID [ %d ] ", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECdensity( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
												m_p_instance_->GetNodeEvent(true), 
												m_p_instance_->mNodeId, 
												GENERATOR_COMMANDS_ENTRY, 
												GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_DENSITY, 
												sizeof(pDataCp), 
												(pDataCp));
	}
	void CaDataDicGen::Generator_RadExposureParameter_AECReference( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET AEC REFERENCE LEN [ %d ] --> DB_ID [ %d ] REFERENCE [ %d ]", nData , pData[0] , pData[1] << 8 | pData[2] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECReference(  pData[0] , pData[ 1 ] << 8 | pData[ 2 ] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET AECHAMBERS LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECReference( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_AEC_REFERENCE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_AECchambers_V5( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		Generator_RadExposureParameter_AECchambers( Access, pData, nData, MessageInfo );
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_Ok };
		
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_CHAMBERS_V5, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_TubePowerLimit ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET TUBEPOWERLIMIT LEN [ %d ] --> DB_ID [ %d ] POWER [ %d ]", nData , pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[2]) != 0 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if(static_cast<byte>(pData[1]) > 100)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBTTurboPowerLimitOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBTubePowerLimit(  pData[0]	, pData[1] ,  pData[2]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET TUBEPOWERLIMIT LEN [ %d ] --> DB_ID [ %d ] ", nData , pData[0]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBTubePowerLimit( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_TUBEPOWERLIMIT, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_RadExposureParameter_AECchambers	( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET AECHAMBERS LEN [ %d ] --> DB_ID [ %d ] CHAMBERS [ %d ]", nData , pData[0] , pData[1] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[2]) != 0 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECchambers(  	pData[0]								,  //DatabankId
																						static_cast<bool>(pData[1] & 0x01)		,  //aecLeftStatus
																						static_cast<bool>((pData[1]>>1)&0x01)	,  //aecCenterStatus
																						static_cast<bool>((pData[1]>>2)&0x01));	   //aecRightStatus
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET AECHAMBERS LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBAECchambers( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_CHAMBERS_V6, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	

	
	void CaDataDicGen::Generator_RadExposureParameter_FPS(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{		
	
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET FPS LEN [ %d ] --> DB_ID [ %d ] FPS [ %d ]", nData , pData[0] , (pData[1] << 8 | pData[2]) );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[3]) != 0 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if( static_cast<word>(pData[1] << 8 | pData[2]) > MAX_FPS10_ALLOWED )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_RDBFPSFpsValueOutOfRange;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBFPS(  pData[0]	, static_cast<word>(pData[1]<<8 | pData[2]) ,  pData[3]  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET FPS LEN [ %d ] --> DB_ID [ %d ]", nData , pData[0] );
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBFPS( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_FPS, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
		void CaDataDicGen::Generator_RadExposureParameter_PatientSize( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[3]) != 0 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}

					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBPatientSize(  pData[0] , pData[1] , pData[2] , pData[3] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBPatientSize( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_PATIENT_SIZE, 
									sizeof(pDataCp), 
									(pDataCp));
	}

	void CaDataDicGen::Generator_RadExposureParameter_TrackingId(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
				switch( Access )
				{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if(pData[2] != 0)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBTrackingId(  pData[0] , pData[1] , pData[2] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBTrackingId(pData[0]);
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_RAD_EXPOSURE_PARAMETER_TRACKINGID, 
									sizeof(pDataCp), 
									(pDataCp));

	}
		
	void CaDataDicGen::Generator_CurrentRadDataBank_RADparams(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if( MessageInfo->SubIndex == GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V5 	&& m_p_instance_->mProtocolVersion.Version != PROTOCOL_VERSION_5 || 
			MessageInfo->SubIndex == GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V6  && m_p_instance_->mProtocolVersion.Version != PROTOCOL_VERSION_6 )
		{
			(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
										m_p_instance_->GetNodeEvent(true), 
										m_p_instance_->mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										MessageInfo->SubIndex, 
										sizeof(pDataCp), 
										(pDataCp));
			return;
		}
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET CURRENTRADPARAM LEN [ %d ]", nData );
					}
					
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RDBCurrentRadDataBank();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent( true ), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									MessageInfo->SubIndex, 
									sizeof( pDataCp ), 
									( pDataCp ));
	}


	
	void CaDataDicGen::Generator_OtherFunctions_GenPowerLimit(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{		
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[1]) > 0x01 )
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_GEN_SS_IncorrectCommandType;
						break;
					}
					
					if( static_cast<byte>(pData[1]) == 0x00 )
					{
						if( static_cast<byte>(pData[0]) < 10 || static_cast<byte>(pData[0]) > 100 )
						{
							pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_GenPowerLimitValueOutOfRange;
							break;
						}
					}
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_GenPowerLimit( pData[0] , pData[1]);
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_GenPowerLimit();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_OTHER_FUNCTIONS_GENPOWERLIMIT, 
									sizeof(pDataCp), 
									(pDataCp));
	}

	
	void CaDataDicGen::Generator_OtherFunctions_Filmanents_Enable(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_FilamentEnabled( pData[0] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_FilamentEnabled();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_OTHER_FUNCTIONS_FILAMENTS_ENABLED, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_OtherFunctions_TubePowerLimit ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] SET TUBEPOWERLIMIT TUBE ID[ %d ] --> POWER [ %d ]", pData[0] , pData[1]);
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
															
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_TubePowerLimit(  pData[ 0 ] , static_cast<signed char>( pData[1] )  ,  static_cast< signed char >( pData[2] ) );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] GET TUBEPOWERLIMIT  ");
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_TubePowerLimit();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_OTHER_FUNCTIONS_TUBEPOWERLIMIT, 
									sizeof(pDataCp), 
									(pDataCp));
	}
		

	
	void CaDataDicGen::Generator_Miscellaneous_RadRanges( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RadExpParamRanges();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_RAD_EXPOSURE_PARAMETER_RANGES, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	

	
	void CaDataDicGen::Generator_Miscellaneous_DigitalInputsOutputs( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_DigitalInputsOutputs( static_cast<word>( pData[0] << 8 | pData[1] ) );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_DIGITAL_INPUTS_OUTPUTS, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_LockIn_Datatbank( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if( m_p_RadInterface_ ) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , false );
                    pDataCp[1] = m_p_RadInterface_->II_Generator_SS_LockInDatabank( pData[ 0 ] , static_cast<bool>( pData[ 1 ] ) , pData[ 2 ] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );
                    pDataCp[1] = m_p_RadInterface_->II_Generator_SS_LockInDatabank( pData[ 0 ] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent( true ), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_LOCKIN_DATABANK, 
									sizeof(pDataCp), 
									(pDataCp));
		
	}
	
	void CaDataDicGen::Generator_LockIn_AssignToProcedure( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , false );
                    pDataCp[1] = m_p_RadInterface_->II_Generator_SS_LockInAssignToProcedure( pData[ 0 ] , pData[ 1 ]);
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true );
                    pDataCp[1] = m_p_RadInterface_->II_Generator_SS_LockInAssignToProcedure( pData[ 0 ] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_ASSIGN_LOCKINDB_TO_PROCEDURE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_LockIn_FrameReached(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
                    pDataCp[1] = m_p_RadInterface_->II_Generator_SS_LockInFrameReached( pData[ 0 ]  );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_LOCKIN_FRAME_REACHED, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_Injector(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , false);
                    pDataCp[1] = m_p_RadInterface_->II_Generator_SS_Injector( static_cast<tInjectorAcction>( pData[ 0 ] )  );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_Injector();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_INJECTOR, 
									sizeof(pDataCp), 
									(pDataCp));
	}
		
	void CaDataDicGen::Generator_ExposureManagement_RadPostExposure( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RadPostExposure();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_EXPOSURE_MANAGEMENT_RAD_POST_EXPOSURE, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Generator_Miscellaneous_RadProcedureParameterRanges( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:	
				{
					tRadParameterRanges RadParameterRanges = { 0 };
					
					RadParameterRanges.DatabankId 		= pData[0];
					RadParameterRanges.MinkV10.value	= pData[ 1 ] << 8 | pData[ 2 ];
					RadParameterRanges.MaxkV10.value	= pData[ 3 ]  << 8  | pData[ 4 ];
					RadParameterRanges.MinmAs1000.value	= pData[ 5 ]  << 16 | pData[ 6 ]  << 8 | pData[ 7 ];
					RadParameterRanges.MaxmAs1000.value	= pData[ 8 ]  << 16 | pData[ 9 ]  << 8 | pData[ 10 ];
					RadParameterRanges.MinmA100.value	= pData[ 11 ] << 16 | pData[ 12 ] << 8 | pData[ 13 ];
					RadParameterRanges.MaxmA100.value	= pData[ 14 ] << 16 | pData[ 15 ] << 8 | pData[ 16 ];
					RadParameterRanges.Minms100.value	= pData[ 17 ] << 16 | pData[ 18 ] << 8 | pData[ 19 ];
					RadParameterRanges.Maxms100.value	= pData[ 20 ] << 16 | pData[ 21 ] << 8 | pData[ 22 ];
					RadParameterRanges.FocalSpot		= pData[ 23 ];
					RadParameterRanges.MinPPS10.value	= pData[ 24 ] << 8 | pData[ 25 ];
					RadParameterRanges.MaxPPS10.value	= pData[ 26 ] << 8 | pData[ 27 ];
#if 0
					m_p_instance_->m_fcb_log_( 1 ,"[ RadProcedureParameterRanges ]" );
					m_p_instance_->m_fcb_log_( 1 ," Id [ %d ]"			,RadParameterRanges.DatabankId );
					m_p_instance_->m_fcb_log_( 1 ," MinkV10 [ %d ]"		,RadParameterRanges.MinkV10.value );
					m_p_instance_->m_fcb_log_( 1 ," MaxkV10 [ %d ]"		,RadParameterRanges.MaxkV10.value );
					m_p_instance_->m_fcb_log_( 1 ," MinmAs1000 [ %d ]"	,RadParameterRanges.MinmAs1000.value );
					m_p_instance_->m_fcb_log_( 1 ," MaxmAs1000 [ %d ]"	,RadParameterRanges.MaxmAs1000.value );
					m_p_instance_->m_fcb_log_( 1 ," MinmA100 [ %d ]"	,RadParameterRanges.MinmA100.value );
					m_p_instance_->m_fcb_log_( 1 ," MaxmA100 [ %d ]"	,RadParameterRanges.MaxmA100.value );
					m_p_instance_->m_fcb_log_( 1 ," Minms100 [ %d ]"	,RadParameterRanges.Minms100.value );
					m_p_instance_->m_fcb_log_( 1 ," Maxms100 [ %d ]"	,RadParameterRanges.Maxms100.value );
					m_p_instance_->m_fcb_log_( 1 ," FocalSpot [ %d ]"	,RadParameterRanges.FocalSpot );
					m_p_instance_->m_fcb_log_( 1 ," MinPPS10 [ %d ]"	,RadParameterRanges.MinPPS10.value );
					m_p_instance_->m_fcb_log_( 1 ," MaxPPS10 [ %d ]"	,RadParameterRanges.MaxPPS10.value );
#endif
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest );
					
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RadProcedureParameterRanges( &RadParameterRanges );
					break;
				}
				case DATADIC_ACCESS_GET:
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );	
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RadProcedureParameterRanges( pData[ 0 ] );
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_AssignRadProcedureParameterRanges( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:	
				{					
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest );	
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_AssignRadProcedureParameterRanges( pData[ 0 ] , pData[ 1 ] );
					break;
				}
				case DATADIC_ACCESS_GET:
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );	
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_AssignRadProcedureParameterRanges( pData[ 0 ] );
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES_ASSIGN, 
									sizeof(pDataCp), 
									(pDataCp));

	}
	
	void CaDataDicGen::Generator_Miscellaneous_RadParameterScales( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );	
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_RadParameterScales();
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_RAD_PARAMETER_SCALES, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_CalibrationStatus( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );	
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_CalibrationStatus();
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_CALIBRATION_STATUS, 
									sizeof(pDataCp), 
									(pDataCp));	
	}
	

	void CaDataDicGen::Generator_ExposureManagement_StartStopExposure(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] START_STOP_EXPOSURE [ %s ]", pData[0] ? "START COMMAND" : "STOP COMMAND");
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					
					if( static_cast<byte>(pData[0]) > 0x01)
					{
						pDataCp[1] = CaDataDicRadInterface::Cp_II_Generator_SS_SSRequestOutOfRange;
						break;
					}
				
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_StartStop( pData[0] );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_CommunicationsInhibitTimeout(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_CommunicationsInhibitTimeout( pData[0] , pData[1] );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_CommunicationsInhibitTimeout( );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_EXPOSURE_MANAGEMENT_COMMUNICATIONS_INHIBIT_TIMEOUT, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_ExposureManagement_ResetExposureCounter( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest );
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_ResetExposureCounter();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_EXPOSURE_MANAGEMENT_RESET_EXPOSURE_COUNTER, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_ExposureSwitchesActivationDevice( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent( MessageInfo->Node_dest , true );
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_ExposureSwitchesActivationDevice();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_EXPOSURE_SWITCHES_ACTIVATION_DEVICE, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_MaxProcedure(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_MaxProcedure( );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_MAXPROCEDURE, 
									sizeof(pDataCp), 
									(pDataCp));
	
	}
	
	void CaDataDicGen::Generator_Miscellaneous_MaxDatabank(tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo)
	{
		if(MessageInfo == nullptr)
			return;
	
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
	
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_MaxDatabank( );
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
												m_p_instance_->GetNodeEvent(true), 
												m_p_instance_->mNodeId, 
												GENERATOR_COMMANDS_ENTRY, 
												GENERATOR_MISCELLANEOUS_MAXDATABANK, 
												sizeof(pDataCp), 
												(pDataCp));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_AecReferenceMode( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;
		
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{
					tTypeAecReference AecReference = AecInternalReferenceBySensitivity;
					
					if( pData[0] & 0x02 )
						AecReference = AecExternalReference;
					else if( pData[0] & 0x01 )
						AecReference = AecInternalReference;
						
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_AecReferenceMode( AecReference );
				}
				break;
				case DATADIC_ACCESS_GET:
				{
					pDataCp[1] = m_p_RadInterface_->II_Generator_SS_AecReferenceMode();
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_AEC_EXTERNAL_REFERENCE, 
									sizeof(pDataCp), 
									(pDataCp));
		
	}
	
	void CaDataDicGen::Generator_Miscellaneous_CountersData( tDataDicAccess Access, byte *pData, word nData, tInfoMessage *MessageInfo )
	{
		if( MessageInfo == nullptr )
			return;
		
		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };
		
		if(m_p_RadInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
				{
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 1 ,"[ R2CP DATADIC ] COUNTERS TubeId [ %d ] Cmd [ %d ]", pData[0] , pData[1]);
					}

					
                    if( pData[ 0 ] > 2 ||  pData[ 0 ] == 0 )
						pDataCp[ 1 ] = CaDataDicRadInterface::Cp_Generator_SS_ExposureCounters_TubeOutOfRange; 
                    else if( pData[ 1 ] > 5  )
						pDataCp[ 1 ] = CaDataDicRadInterface::Cp_Generator_SS_ExposureCounters_CommandOutOfRange;
					else
                        pDataCp[1] = m_p_RadInterface_->II_Generator_SS_CountersData( pData[ 0 ] , pData [ 1 ] );
					
				}
				break;
			}
		}
	
		(void)m_Type_->Processed(	ETH_LOWEST_PRIORITY ,
									m_p_instance_->GetNodeEvent(true), 
									m_p_instance_->mNodeId, 
									GENERATOR_COMMANDS_ENTRY, 
									GENERATOR_MISCELLANEOUS_COUNTERS_DATA, 
									sizeof(pDataCp), 
									(pDataCp));
	}
	
	//////////////////////////////////////////EVENTS//////////////////////////////////////////
	void CaDataDicGen::Generator_DataBank_DefineProcedure_Event(tProcedure *pProcedure)
	{
		if( pProcedure->SubIndex == PATIENT_PROCEDURE_DEFINITION )
		{
			byte Data[] = {  pProcedure->ProcedureId,
							 pProcedure->GeneratorProcedureType,
							 pProcedure->HorFSwitchId,
							 pProcedure->ActiveWhenHorFSwichIsPressed,
							 pProcedure->WorkstationId,
							 pProcedure->TotalNumOfExpDBinProcedure,
							 pProcedure->GenDataBankSequencing };
		
			(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
											GetNodeEvent(),
											mNodeId, 
											GENERATOR_COMMANDS_ENTRY, 
											GENERATOR_DATA_BANK_DEFINE_PROCEDURE, 
											sizeof(Data), 
											(Data));
		}
		else
		{
			Generator_DataBank_Define_NExposures_Procedure_Event( pProcedure );
		}
	}
	
	void CaDataDicGen::Generator_DataBank_Define_NExposures_Procedure_Event(tProcedure *pProcedure)
	{
			byte Data[] = {  pProcedure->ProcedureId,
							 pProcedure->GeneratorProcedureType,
							 pProcedure->HorFSwitchId,
							 pProcedure->ActiveWhenHorFSwichIsPressed,
							 pProcedure->WorkstationId,
							 pProcedure->TotalNumOfExpDBinProcedure,
							 pProcedure->GenDataBankSequencing,
							 static_cast<byte>( pProcedure->TotalNumOfExpInProcedure.Fields.data_8_15 ),
							 static_cast<byte>( pProcedure->TotalNumOfExpInProcedure.Fields.data_0_7 )
						};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_DATA_BANK_DEFINE_PROCEDURE_NEXPOSURES, 
										sizeof(Data), 
										(Data));
	}
		
	void CaDataDicGen::Generator_DataBank_ProcedureDatabankActivate_Event( byte procedureID , byte Procedure_Status , byte ExpDBSequenceNumber )
	{
	
		byte Data[] = {	 procedureID,
						 Procedure_Status,
						 ExpDBSequenceNumber };
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_DATA_BANK_PROCEDURE_DATABANK_ACTIVATE, 
										sizeof(Data), 
										(Data));
	}
	
	void CaDataDicGen::Generator_ExposureManagement_GeneratorStatus_Event(tGeneratorStatus *LastGeneratorStatus)
	{
		if( m_EnableLogInfo && m_fcb_log_ )
		{
			m_fcb_log_(1,"[ R2CP DATADIC ] --> STATUS [ %d ] SM_ENABLED [ %d ] RX_ENABLED [ %d ] PROC_ID [ %d ] PROC_STATUS [ %d ] EXP_DB [ %d ] HU_ANODE [ %d ] HU_GEN [ %d ] PREP [ %d ] EXP [ %d ] FIL [ %d ]",
					LastGeneratorStatus->GeneratorStatus,
					LastGeneratorStatus->SystemMessage.Fields.Active,
					LastGeneratorStatus->SystemMessage.Fields.RxDisabled,
					LastGeneratorStatus->ProcedureId,
					LastGeneratorStatus->ProcedureStatus,
					LastGeneratorStatus->ExposureDatabankSeqNumber,
					LastGeneratorStatus->AccumulatedAnodeHU,
					LastGeneratorStatus->AccumulatedGenHU,
					LastGeneratorStatus->ExposureSwitches.Fields.PrepPedalStatus,
					LastGeneratorStatus->ExposureSwitches.Fields.ExpPedalStatus,
					LastGeneratorStatus->ExposureSwitches.Fields.FilStatus);
		}
		
		byte Data[ 14 ] = { 0 };
		byte SubIndex	= GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5;
		
		if( mProtocolVersion.Version == PROTOCOL_VERSION_5 )
		{
			Data[ 0 ]  = LastGeneratorStatus->GeneratorStatus;
			Data[ 1 ]  = LastGeneratorStatus->SystemMessage.value;
			Data[ 2 ]  = LastGeneratorStatus->ProcedureId;
			Data[ 3 ]  = LastGeneratorStatus->ProcedureStatus;
			Data[ 4 ]  = LastGeneratorStatus->ExposureDatabankSeqNumber;
			Data[ 5 ]  = LastGeneratorStatus->FluoroTime.Fields.data_15_8;
			Data[ 6 ]  = LastGeneratorStatus->FluoroTime.Fields.data_0_7;
			Data[ 7 ]  = LastGeneratorStatus->FluoroStatus.value & 0x11;
			Data[ 8 ]  = LastGeneratorStatus->AccumulatedAnodeHU;
			Data[ 9 ]  = LastGeneratorStatus->AccumultatedhousingHU;
			Data[ 10 ] = LastGeneratorStatus->AccumulatedGenHU;
			Data[ 11 ] = LastGeneratorStatus->ExposureSwitches.value;
			Data[ 12 ] = LastGeneratorStatus->DynModeStatus.value;
			Data[ 13 ] = LastGeneratorStatus->CurrentRotorSpeed;
		}
		else
		{
			SubIndex = GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V6;
					
			Data[ 0 ]  = LastGeneratorStatus->GeneratorStatus;
			Data[ 1 ]  = LastGeneratorStatus->SystemMessage.value;
			Data[ 2 ]  = LastGeneratorStatus->ProcedureId;
			Data[ 3 ]  = LastGeneratorStatus->ProcedureType;
			Data[ 4 ]  = LastGeneratorStatus->ExposureDatabankSeqNumber;
			Data[ 5 ]  = LastGeneratorStatus->FluoroTime.Fields.data_15_8;
			Data[ 6 ]  = LastGeneratorStatus->FluoroTime.Fields.data_0_7;
			Data[ 7 ]  = LastGeneratorStatus->FluoroStatus.value;
			Data[ 8 ]  = LastGeneratorStatus->AccumulatedAnodeHU;
			Data[ 9 ]  = LastGeneratorStatus->AccumultatedhousingHU;
			Data[ 10 ] = LastGeneratorStatus->AccumulatedGenHU;
			Data[ 11 ] = LastGeneratorStatus->ExposureSwitches.value;
			Data[ 12 ] = LastGeneratorStatus->DynModeStatus.value;
			Data[ 13 ] = LastGeneratorStatus->CurrentRotorSpeed;
		}
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										SubIndex, 
										sizeof( Data ), 
										Data );
	
	
	}
	
	void CaDataDicGen::Generator_DataBank_AssignExposure_Event(byte procedureID, byte ExpSequenceNumber, byte ExpDBId , bool Allowed)
	{
		byte Data[] = {	 procedureID,
						 ExpSequenceNumber,
						 ExpDBId,
						 Allowed,
						};
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_DATA_BANK_ASSIGN_EXPOSURE,
										sizeof(Data),
										Data);
	}
	
	void CaDataDicGen::Generator_DataBank_ExposureAcceptance_Event( byte procedureID, byte sequenceIndex , bool Allowed)
	{
	
		byte Data[] =  {	procedureID,
							sequenceIndex,
							Allowed
						};
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
										GetNodeEvent(),  
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE, 
										sizeof(Data), 
										Data);
	}
	
	void CaDataDicGen::Generator_RadDataBankLoad_Deprecated_Event(tRadDb *RadDatabank)
	{

	}

	void CaDataDicGen::Generator_RadDataBankLoad_Event(tRadDb *RadDatabank)
	{	
		if( RadDatabank == nullptr )
			return;
		
		if( mProtocolVersion.Version == PROTOCOL_VERSION_5 )
		{
			byte Data[] = { RadDatabank->DatabankId,
							RadDatabank->ImagingSystemProtocolId,
							RadDatabank->TechMode.value,
							RadDatabank->kV10.Fields.data_8_15,
							RadDatabank->kV10.Fields.data_0_7,
							RadDatabank->mAs1000.Fields.data_16_23,
							RadDatabank->mAs1000.Fields.data_8_15,
							RadDatabank->mAs1000.Fields.data_0_7,
							RadDatabank->mA100.Fields.data_16_23,
							RadDatabank->mA100.Fields.data_8_15,
							RadDatabank->mA100.Fields.data_0_7,
							RadDatabank->ms100.Fields.data_16_23,
							RadDatabank->ms100.Fields.data_8_15,
							RadDatabank->ms100.Fields.data_0_7,
							RadDatabank->MinIntegrationTime.Fields.data_8_15,
							RadDatabank->MinIntegrationTime.Fields.data_0_7,
							RadDatabank->MaxIntegrationTime.Fields.data_8_15,
							RadDatabank->MaxIntegrationTime.Fields.data_0_7,
							RadDatabank->FocalSpot,
							RadDatabank->TargetDose.Fields.AECSensitivity_DoseTarget,
							RadDatabank->TargetDose.Fields.AECDensity,
							RadDatabank->IonChamber.value,
							RadDatabank->Spare_1,
							RadDatabank->FPS10.Fields.data_8_15,
							RadDatabank->FPS10.Fields.data_0_7,
							RadDatabank->TrakingId,
							RadDatabank->Spare_2
			};
			
			(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
											GetNodeEvent(),  
											mNodeId, 
											GENERATOR_COMMANDS_ENTRY, 
											GENERATOR_RAD_DATA_BANK_LOAD_V5, 
											sizeof(Data), 
											Data);
		}
		else
		{
			byte Data[] = { RadDatabank->DatabankId,
							RadDatabank->ImagingSystemProtocolId,
							RadDatabank->TechMode.value,
							RadDatabank->kV10.Fields.data_8_15,
							RadDatabank->kV10.Fields.data_0_7,
							RadDatabank->mAs1000.Fields.data_16_23,
							RadDatabank->mAs1000.Fields.data_8_15,
							RadDatabank->mAs1000.Fields.data_0_7,
							RadDatabank->mA100.Fields.data_16_23,
							RadDatabank->mA100.Fields.data_8_15,
							RadDatabank->mA100.Fields.data_0_7,
							RadDatabank->ms100.Fields.data_16_23,
							RadDatabank->ms100.Fields.data_8_15,
							RadDatabank->ms100.Fields.data_0_7,
							RadDatabank->MinIntegrationTime.Fields.data_8_15,
							RadDatabank->MinIntegrationTime.Fields.data_0_7,
							RadDatabank->MaxIntegrationTime.Fields.data_8_15,
							RadDatabank->MaxIntegrationTime.Fields.data_0_7,
							RadDatabank->FocalSpot,
							RadDatabank->TargetDose.Fields.AECSensitivity_DoseTarget,
							RadDatabank->TargetDose.Fields.AECDensity,
							RadDatabank->IonChamber.value,
							RadDatabank->Spare_1,
							RadDatabank->FPS10.Fields.data_8_15,
							RadDatabank->FPS10.Fields.data_0_7,
							RadDatabank->TrakingId,
							RadDatabank->Spare_2
			};
			
			(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
											GetNodeEvent(),  
											mNodeId, 
											GENERATOR_COMMANDS_ENTRY, 
											GENERATOR_RAD_DATA_BANK_LOAD_V6, 
											sizeof(Data), 
											Data);
		}
				
	}	
	
	void CaDataDicGen::Generator_RadDataBank_ProcedureAcceptance_Event(byte ProcedureID, bool Allowed, word NumberExposuresAllowed)
	{
		byte Data[] = { ProcedureID,
						static_cast<byte>(Allowed),
						static_cast<byte>((NumberExposuresAllowed & 0xFF00) >> 8),
						static_cast<byte>(NumberExposuresAllowed & 0x00FF)
		};
	
		(void)m_Type_->Event_Answer(   ETH_LOWEST_PRIORITY, 
										GetNodeEvent(),
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_RAD_DATA_BANK_ACCEPTANCE, 
										sizeof(Data), 
										Data);
	
	
	}
	
	void CaDataDicGen::Generator_CurrentRadDataBank_RADparams_Event(tRadDb *RadDatabank)
	{
		if(RadDatabank == nullptr)
			return;
		
		if( mProtocolVersion.Version == PROTOCOL_VERSION_5 )
		{
			byte Data[] = { RadDatabank->ImagingSystemProtocolId,
							RadDatabank->TechMode.value,
							RadDatabank->kV10.Fields.data_8_15,
							RadDatabank->kV10.Fields.data_0_7,
							RadDatabank->mAs1000.Fields.data_16_23,
							RadDatabank->mAs1000.Fields.data_8_15,
							RadDatabank->mAs1000.Fields.data_0_7,
							RadDatabank->mA100.Fields.data_16_23,
							RadDatabank->mA100.Fields.data_8_15,
							RadDatabank->mA100.Fields.data_0_7,
							RadDatabank->ms100.Fields.data_16_23,
							RadDatabank->ms100.Fields.data_8_15,
							RadDatabank->ms100.Fields.data_0_7,
							RadDatabank->MinIntegrationTime.Fields.data_8_15,
							RadDatabank->MinIntegrationTime.Fields.data_0_7,
							RadDatabank->MaxIntegrationTime.Fields.data_8_15,
							RadDatabank->MaxIntegrationTime.Fields.data_0_7,
							RadDatabank->FocalSpot,
							RadDatabank->TargetDose.Fields.AECDensity,
							RadDatabank->TargetDose.Fields.AECSensitivity_DoseTarget,
							RadDatabank->IonChamber.value,
							RadDatabank->Spare_1,
							RadDatabank->FPS10.Fields.data_8_15,
							RadDatabank->FPS10.Fields.data_0_7,
							RadDatabank->TrakingId,
							100//Dumy Esto es GeneratorPowerLimit -> En la siguiente version del protocolo se quitara.
			};
			
			(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
											GetNodeEvent() ,  
											mNodeId, 
											GENERATOR_COMMANDS_ENTRY, 
											GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V5, 
											sizeof(Data), 
											Data);
		}
		else
		{
			byte Data[] = { RadDatabank->ImagingSystemProtocolId,
							RadDatabank->TechMode.value,
							RadDatabank->kV10.Fields.data_8_15,
							RadDatabank->kV10.Fields.data_0_7,
							RadDatabank->mAs1000.Fields.data_16_23,
							RadDatabank->mAs1000.Fields.data_8_15,
							RadDatabank->mAs1000.Fields.data_0_7,
							RadDatabank->mA100.Fields.data_16_23,
							RadDatabank->mA100.Fields.data_8_15,
							RadDatabank->mA100.Fields.data_0_7,
							RadDatabank->ms100.Fields.data_16_23,
							RadDatabank->ms100.Fields.data_8_15,
							RadDatabank->ms100.Fields.data_0_7,
							RadDatabank->MinIntegrationTime.Fields.data_8_15,
							RadDatabank->MinIntegrationTime.Fields.data_0_7,
							RadDatabank->MaxIntegrationTime.Fields.data_8_15,
							RadDatabank->MaxIntegrationTime.Fields.data_0_7,
							RadDatabank->FocalSpot,
							RadDatabank->TargetDose.Fields.AECDensity,
							RadDatabank->TargetDose.Fields.AECSensitivity_DoseTarget,
							RadDatabank->IonChamber.value,
							RadDatabank->Spare_1,
							RadDatabank->FPS10.Fields.data_8_15,
							RadDatabank->FPS10.Fields.data_0_7,
							RadDatabank->TrakingId,
							RadDatabank->Spare_2
			};
		
			(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
											GetNodeEvent() ,  
											mNodeId, 
											GENERATOR_COMMANDS_ENTRY, 
											GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V6, 
											sizeof(Data), 
											Data);
		}		
	}	
	
	void CaDataDicGen::Generator_Miscellaneous_RadExposureParameterScale_Event( tScale kVpScale , tScale mAsScale , tScale mAScale , tScale msScale  )
	{
		byte Data[] = {
				kVpScale,
				mAsScale,
				mAScale,
				msScale
		};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
										GetNodeEvent() ,  
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_MISCELLANEOUS_RAD_PARAMETER_SCALES, 
										sizeof(Data), 
										Data);
	}
	
	void CaDataDicGen::Generator_Miscellaneous_RadProcedureParameterRanges_Event( R2CP::tRadParameterRanges  *pRadParameterRanges )
	{
		byte Data[] = {	pRadParameterRanges->DatabankId,
						pRadParameterRanges->MinkV10.Fields.data_8_15,
						pRadParameterRanges->MinkV10.Fields.data_0_7,
						pRadParameterRanges->MaxkV10.Fields.data_8_15,
						pRadParameterRanges->MaxkV10.Fields.data_0_7,
						pRadParameterRanges->MinmAs1000.Fields.data_16_23,
						pRadParameterRanges->MinmAs1000.Fields.data_8_15,
						pRadParameterRanges->MinmAs1000.Fields.data_0_7,
						pRadParameterRanges->MaxmAs1000.Fields.data_16_23,
						pRadParameterRanges->MaxmAs1000.Fields.data_8_15,
						pRadParameterRanges->MaxmAs1000.Fields.data_0_7,
						pRadParameterRanges->MinmA100.Fields.data_16_23,
						pRadParameterRanges->MinmA100.Fields.data_8_15,
						pRadParameterRanges->MinmA100.Fields.data_0_7,
						pRadParameterRanges->MaxmA100.Fields.data_16_23,
						pRadParameterRanges->MaxmA100.Fields.data_8_15,
						pRadParameterRanges->MaxmA100.Fields.data_0_7,
						pRadParameterRanges->Minms100.Fields.data_16_23,
						pRadParameterRanges->Minms100.Fields.data_8_15,
						pRadParameterRanges->Minms100.Fields.data_0_7,
						pRadParameterRanges->Maxms100.Fields.data_16_23,
						pRadParameterRanges->Maxms100.Fields.data_8_15,
						pRadParameterRanges->Maxms100.Fields.data_0_7,
						pRadParameterRanges->FocalSpot,
						pRadParameterRanges->MinPPS10.Fields.data_8_15,
						pRadParameterRanges->MinPPS10.Fields.data_0_7,
						pRadParameterRanges->MaxPPS10.Fields.data_8_15,
						pRadParameterRanges->MaxPPS10.Fields.data_0_7 };


					(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY ,
													m_p_instance_->GetNodeEvent(true), 
													m_p_instance_->mNodeId, 
													GENERATOR_COMMANDS_ENTRY, 
													GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES, 
													sizeof(Data), 
													(Data));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_AssignRadProcedureParameterRanges_Event( byte ProcedureId , byte DatabankId , bool Allowed )
	{
		byte Data[] = { ProcedureId , DatabankId , Allowed };
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY ,
										m_p_instance_->GetNodeEvent(true), 
										m_p_instance_->mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES_ASSIGN, 
										sizeof(Data), 
										(Data));
		
	}
	



	
	void CaDataDicGen::Generator_DoseLevel_Event( byte DatabankId , uword DoseLevelId )
	{
		byte Data[]={ 	
				DatabankId ,  
				true,
				DoseLevelId.Fields.data_8_15,
				DoseLevelId.Fields.data_0_7,
		};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent() , 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_FL_EXPOSURE_PARAMETER_DOSELEVELID, 
										sizeof(Data), 
										(byte *)&Data);
				
	}
	
	void CaDataDicGen::Generator_CommunicationsInhibitTimeout_Event( byte CommInhibit_sec , bool status , byte SourceNode)
	{
		byte Data[]={ 	CommInhibit_sec, status, SourceNode  };
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent() , 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_EXPOSURE_MANAGEMENT_COMMUNICATIONS_INHIBIT_TIMEOUT, 
										sizeof(Data), 
										(byte *)&Data);
	}
	
	void CaDataDicGen::Generator_OtherFunctions_GenPowerLimit_Event( bool Allowed , byte GenPowerLimit )
	{
	
		byte Data[] = {	 Allowed,
						 GenPowerLimit
					  };
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
										GetNodeEvent() ,  
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_OTHER_FUNCTIONS_GENPOWERLIMIT, 
										sizeof(Data), 
										Data);
	
	}
	
	void CaDataDicGen::Generator_OtherFunctions_FilmanentsEnable_Event(bool Status)
	{
		byte fil_status = Status;
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,  
										GetNodeEvent() , 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_OTHER_FUNCTIONS_FILAMENTS_ENABLED, 
										sizeof(byte), 
										&fil_status);
	}
	
	void CaDataDicGen::Generator_Miscellaneous_RadRanges_Event(tRadExpParamRange *pRadRanges)
	{
		byte Data[]={ pRadRanges->MinkV10.Fields.data_8_15,
					  pRadRanges->MinkV10.Fields.data_0_7,
					  pRadRanges->MaxKv10.Fields.data_8_15,
					  pRadRanges->MaxKv10.Fields.data_0_7,
					  pRadRanges->MinmAs1000.Fields.data_16_23,
					  pRadRanges->MinmAs1000.Fields.data_8_15,
					  pRadRanges->MinmAs1000.Fields.data_0_7,
					  pRadRanges->MaxmAs1000.Fields.data_16_23,
					  pRadRanges->MaxmAs1000.Fields.data_8_15,
					  pRadRanges->MaxmAs1000.Fields.data_0_7,
					  pRadRanges->MinmA100.Fields.data_16_23,
					  pRadRanges->MinmA100.Fields.data_8_15,
					  pRadRanges->MinmA100.Fields.data_0_7,
					  pRadRanges->MaxmA100.Fields.data_16_23,
					  pRadRanges->MaxmA100.Fields.data_8_15,
					  pRadRanges->MaxmA100.Fields.data_0_7,
					  pRadRanges->Minms100.Fields.data_16_23,
					  pRadRanges->Minms100.Fields.data_8_15,
					  pRadRanges->Minms100.Fields.data_0_7,
					  pRadRanges->Maxms100.Fields.data_16_23,
					  pRadRanges->Maxms100.Fields.data_8_15,
					  pRadRanges->Maxms100.Fields.data_0_7,
					  pRadRanges->mAScale
					};
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent() , 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_RAD_EXPOSURE_PARAMETER_RANGES, 
										sizeof(Data), 
										(Data));
	}
	

	
	void CaDataDicGen::Generator_Miscellaneous_MaxProcedure_Event(byte MaxNumProcedureAllowed)
	{
	   byte MNPrAll = MaxNumProcedureAllowed;
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent() , 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_MAXPROCEDURE, 
										sizeof(byte), 
										(&MNPrAll));
	
	}
	
	void CaDataDicGen::Generator_Miscellaneous_MaxDatabank_Event(byte MaxNumDBAllowed)
	{
		 byte MNDBAll = MaxNumDBAllowed;
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent() , 
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_MAXDATABANK, 
										sizeof(byte), 
										(&MNDBAll));
	}
	
	void CaDataDicGen::Generator_ExposureManagement_RadPostExposure_Event( tRadPostExpPostCondition *pPostExpPostCondition )
	{
		if( mProtocolVersion.Version == 5 )
		{
			byte Data[] = {	 pPostExpPostCondition->kV10.Fields.data_8_15,
							 pPostExpPostCondition->kV10.Fields.data_0_7,
							 pPostExpPostCondition->mA100.Fields.data_16_23,
							 pPostExpPostCondition->mA100.Fields.data_8_15,
							 pPostExpPostCondition->mA100.Fields.data_0_7,
							 pPostExpPostCondition->ms100.Fields.data_16_23,
							 pPostExpPostCondition->ms100.Fields.data_8_15,
							 pPostExpPostCondition->ms100.Fields.data_0_7,
							 pPostExpPostCondition->mAs1000.Fields.data_16_23,
							 pPostExpPostCondition->mAs1000.Fields.data_8_15,
							 pPostExpPostCondition->mAs1000.Fields.data_0_7,
							 pPostExpPostCondition->FocalSpot,
							 pPostExpPostCondition->TargetDose.Fields.AECSensitivity_DoseTarget,
							 pPostExpPostCondition->IonChamber.value,
							 pPostExpPostCondition->TargetDose.Fields.AECDensity,
							 pPostExpPostCondition->RadEndOfExposure
						 };
		
				(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
												GetNodeEvent() , 
												mNodeId , 
												GENERATOR_COMMANDS_ENTRY, 
												GENERATOR_EXPOSURE_MANAGEMENT_POST_CONDITION_V5, 
												sizeof(Data), 
												(Data));
		}
		else
		{
			byte Data[] = {	 pPostExpPostCondition->ExposureNumber.Fields.data_8_15,
							 pPostExpPostCondition->ExposureNumber.Fields.data_0_7,
							 pPostExpPostCondition->ProcedureId,
							 pPostExpPostCondition->ExpSeqNumber,
							 pPostExpPostCondition->kV10.Fields.data_8_15,
							 pPostExpPostCondition->kV10.Fields.data_0_7,
							 pPostExpPostCondition->mA100.Fields.data_16_23,
							 pPostExpPostCondition->mA100.Fields.data_8_15,
							 pPostExpPostCondition->mA100.Fields.data_0_7,
							 pPostExpPostCondition->ms100.Fields.data_16_23,
							 pPostExpPostCondition->ms100.Fields.data_8_15,
							 pPostExpPostCondition->ms100.Fields.data_0_7,
							 pPostExpPostCondition->mAs1000.Fields.data_16_23,
							 pPostExpPostCondition->mAs1000.Fields.data_8_15,
							 pPostExpPostCondition->mAs1000.Fields.data_0_7,
							 pPostExpPostCondition->FocalSpot,
							 pPostExpPostCondition->TargetDose.Fields.AECSensitivity_DoseTarget,
							 pPostExpPostCondition->TargetDose.Fields.AECDensity,
							 pPostExpPostCondition->IonChamber.value,
							 pPostExpPostCondition->RadEndOfExposure,
							 pPostExpPostCondition->IonChamberOrientation
						 };
		
				(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
												GetNodeEvent() , 
												mNodeId , 
												GENERATOR_COMMANDS_ENTRY, 
												GENERATOR_EXPOSURE_MANAGEMENT_RAD_POST_EXPOSURE, 
												sizeof(Data), 
												(Data));
		}

	}


	
	void CaDataDicGen::Generator_ClearAllProcedure_Event(void)
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),  
										mNodeId, 
										GENERATOR_COMMANDS_ENTRY, 
										GENERATOR_DATA_BANK_CLEAR_ALL_PROCEDURE, 
										0 , 
										nullptr);
	}
	
	void CaDataDicGen::Generator_DataBank_ProcedureDatabankDefault_Event(byte ProcedureDefaultId , byte ExpSeqId )
	{
		byte Data[]={ ProcedureDefaultId,
					  ExpSeqId
					};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_DATA_BANK_PROCEDURE_DATABANK_DEFAULT,
										sizeof(Data),
										Data);
	}
	
	void CaDataDicGen::Generator_Miscellaneous_AecReferenceMode_Event( tTypeAecReference AecReference )
	{
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_AEC_EXTERNAL_REFERENCE,
										sizeof(byte),
										reinterpret_cast<byte *>(&AecReference));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_CountersData_Event( byte TubeId , byte CommandId , dword SmallFocusData , dword LargeFocusData )
	{
		byte Data[ 10 ]= { 0 };
		udword Dummy;
		
		Data[ 0 ] = TubeId;
		Data[ 1 ] = CommandId;
		
		Dummy.value = SmallFocusData;
		Data[ 2 ] = Dummy.Fields.data_24_31;
		Data[ 3 ] = Dummy.Fields.data_16_23;
		Data[ 4 ] = Dummy.Fields.data_8_15;
		Data[ 5 ] = Dummy.Fields.data_0_7;
		
		Dummy.value = LargeFocusData;
		Data[ 6 ] = Dummy.Fields.data_24_31;
		Data[ 7 ] = Dummy.Fields.data_16_23;
		Data[ 8 ] = Dummy.Fields.data_8_15;
		Data[ 9 ] = Dummy.Fields.data_0_7;
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_COUNTERS_DATA,
										sizeof(Data),
										Data);
	}
	
	void CaDataDicGen::Generator_Miscellaneous_DigitalInputsOutputs_Event( word DigitalInputsMask , word DigitalInputsValue )
	{
		
		word Data[]={ htons( DigitalInputsMask ) , htons( DigitalInputsValue ) };

		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_DIGITAL_INPUTS_OUTPUTS,
										sizeof(Data),
										reinterpret_cast<byte *>(Data));
	}
	
	void CaDataDicGen::Generator_ExposureSwitchesActivationDevice_Event( tExposureSwitchesActivation ExposureSwitchesActivation )
	{
		byte Data[]=
		{ 
				ExposureSwitchesActivation.FluoroStatus_HARDWIRED << 2 | ExposureSwitchesActivation.ExposureStatus_HARDWIRED << 1 | ExposureSwitchesActivation.PreparationStatus_HARDWIRED,
				ExposureSwitchesActivation.FluoroStatus_BLUETOOTH << 2 | ExposureSwitchesActivation.ExposureStatus_BLUETOOTH << 1 | ExposureSwitchesActivation.PreparationStatus_BLUETOOTH,
				ExposureSwitchesActivation.FluoroStatus_IR 		  << 2 | ExposureSwitchesActivation.ExposureStatus_IR        << 1 | ExposureSwitchesActivation.PreparationStatus_IR,
				ExposureSwitchesActivation.ServiceOnlyExposureDeadman 
				
		};	
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_EXPOSURE_SWITCHES_ACTIVATION_DEVICE,
										sizeof(Data),
										reinterpret_cast<byte *>(Data));
	}
	
	void CaDataDicGen::Generator_Miscellaneous_CalibrationStatus_Event( tCalibrationStatus CalibrationStatus )
	{
	
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_MISCELLANEOUS_CALIBRATION_STATUS,
										sizeof(byte),
										reinterpret_cast<byte *>(&CalibrationStatus));
	}
	
	void CaDataDicGen::Generator_LockInDatabank_Event(  byte LockInDb , bool AutoLockInEnabled , byte AutoLockInTime_Sec10 )
	{
		byte Data[] = {
				LockInDb,
				AutoLockInEnabled,
				AutoLockInTime_Sec10
		};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_LOCKIN_DATABANK,
										sizeof(Data),
										reinterpret_cast<byte *>(&Data));
	}
	
	void CaDataDicGen::Generator_LockInAssignToProcedure_Event(  byte ProcedureId , byte LockInDb , bool Allowed )
	{
		byte Data[] = {
				ProcedureId,
				LockInDb,
				Allowed
		};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_ASSIGN_LOCKINDB_TO_PROCEDURE,
										sizeof(Data),
										reinterpret_cast<byte *>(&Data));
	}
	
	void CaDataDicGen::Generator_LockInFrameReached_Event( byte LockInDb , byte LockInFrameReached )
	{
		byte Data[] = {
				LockInDb,
				LockInFrameReached,
		};
		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_LOCKIN_FRAME_REACHED,
										sizeof(Data),
										reinterpret_cast<byte *>(&Data));
	}
	
	void CaDataDicGen::Generator_Injector_Event( tInjectorStatus InjectorStatus )
	{		
		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY, 
										GetNodeEvent(), 
										mNodeId,
										GENERATOR_COMMANDS_ENTRY,
										GENERATOR_INJECTOR,
										sizeof(byte),
										reinterpret_cast<byte *>(&InjectorStatus));
	}
	
	

	

}//namespace R2CP
*/
