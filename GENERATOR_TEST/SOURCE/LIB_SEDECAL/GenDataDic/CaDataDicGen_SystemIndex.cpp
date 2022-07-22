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
	//												System
	/******************************************************************************************************************/

	void CaDataDicGen::System_SystemMessage( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(MessageInfo == nullptr)
			return;

		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };

		if(m_p_SystemInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_SET:
				{

					tSystemMessage SystemMessage;
					//Se cambia el Issuing Node por el del nodo que hace el set
					SystemMessage.Message_Info.IssuingNode				= /*pData[0]*/MessageInfo->Node_dest;
					
					SystemMessage.Id									= (dword)pData[1]<<24 | (dword)pData[2]<<16 | (dword)pData[3]<<8 | pData[4];
					SystemMessage.Message_Info.InhibitRX				= pData[6];
					SystemMessage.Message_Info.InhibitGeneratorPhase 	= pData[7];
					SystemMessage.Message_Info.InhibitMovement			= pData[8];
					SystemMessage.Message_Info.InhibitPositionerPhase	= pData[9];
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_p_instance_->m_fcb_log_( 2 ,"[ R2CP DATADIC ] --> SYSTEM MESSAGE SET => ID [ %d ] ISS [ %d ] RxEnabled [ %s ] Status [ %s ]", SystemMessage.Id, SystemMessage.Message_Info.IssuingNode , SystemMessage.Message_Info.InhibitRX ? "TRUE" : "FALSE" , pData[5] ? "ENABLED" : "DISABLED");
					}

					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest);
					pDataCp[1] = m_p_SystemInterface_->II_System_SS_SystemMessage( &SystemMessage , static_cast<byte>(pData[5]));
				}
					break;
				case DATADIC_ACCESS_GET:
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_fcb_log_( 2 ,"[ R2CP DATADIC ] --> SYSTEM MESSAGE GET");
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_SystemInterface_->II_System_SS_SystemMessage( (dword)pData[0]<<24 | (dword)pData[1]<<16 | (dword)pData[2]<<8 | pData[3] );
					break;
			}
		}

		(void)m_Type_->Processed(	   ETH_LOWEST_PRIORITY ,
									   m_p_instance_->GetNodeEvent(true), 
									   m_p_instance_->mNodeId, 
									   SYSTEM_COMMANDS_ENTRY,
									   SYSTEM_SYSTEM_MESSAGE,
									   sizeof(pDataCp), 
									   (pDataCp));
	}

	void CaDataDicGen::System_AllSystemMessages( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{

		if(MessageInfo == nullptr)
			return;

		byte pDataCp[] = { MessageInfo->Sequence , Cp_MessageNotAvailable };

		if(m_p_SystemInterface_) 
		{
			switch( Access )
			{
				case DATADIC_ACCESS_GET:
					
					if(m_EnableLogInfo && m_fcb_log_)
					{
						m_fcb_log_( 2 ,"[ R2CP DATADIC ] --> SYSTEM MESSAGE ALL GET");
					}
					
					m_p_instance_->SetNodeEvent(MessageInfo->Node_dest , true);
					pDataCp[1] = m_p_SystemInterface_->II_System_SS_RequestAllSystemMessages( );
					break;
			}
		}

		(void)m_Type_->Processed(  ETH_LOWEST_PRIORITY ,
								   m_p_instance_->GetNodeEvent(true), 
								   m_p_instance_->mNodeId, 
								   SYSTEM_COMMANDS_ENTRY,
								   SYSTEM_ALL_SYSTEM_MESSAGES,
								   sizeof(pDataCp), 
								   (pDataCp));
	}

	void CaDataDicGen::System_SystemMessage_Event(tSystemMessage *pSystemMessage, bool status)
	{
		byte Data[] = {	 pSystemMessage->Message_Info.IssuingNode,
						(pSystemMessage->Id>>24)&0x000000FFL,
						(pSystemMessage->Id>>16)&0x000000FFL,
						(pSystemMessage->Id>>8)&0x000000FFL,
						pSystemMessage->Id & 0x000000FFL,
						status,
						pSystemMessage->Message_Info.InhibitRX,
						pSystemMessage->Message_Info.InhibitGeneratorPhase,
						pSystemMessage->Message_Info.InhibitMovement,
						pSystemMessage->Message_Info.InhibitPositionerPhase
						};
		
		if(m_EnableLogInfo && m_fcb_log_)
		{
			m_fcb_log_(2,"[ R2CP DATADIC EVENT] --> SYSTEM MESSAGE ID [ %d ] ISS [ %d ] RxEnabled [ %s ] Status [ %s ]", pSystemMessage->Id, pSystemMessage->Message_Info.IssuingNode , pSystemMessage->Message_Info.InhibitRX ? "TRUE" : "FALSE" , status? "ENABLED" : "DISABLED");
		}

		(void)m_Type_->Event_Answer(	ETH_LOWEST_PRIORITY,
										GetNodeEvent(),
										mNodeId,
										SYSTEM_COMMANDS_ENTRY,
										SYSTEM_SYSTEM_MESSAGE,
										sizeof(Data),
										(Data));
	}

	/************************************************************************************************************************/
	/*********************************** Service Mode************************************************************************/
	/************************************************************************************************************************/
	void CaDataDicGen::System_LoadDesktop ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo )
	{
		if(m_p_SystemInterface_) 
			m_p_SystemInterface_->II_System_SS_VirtualDesktopEntry( SYSTEM_VD_LOAD_DESKTOP , pData,  nData );
	}
	void CaDataDicGen::System_DesktopDisplay ( tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(m_p_SystemInterface_) 
			m_p_SystemInterface_->II_System_SS_VirtualDesktopEntry( SYSTEM_VD_DESKTOP_DISPLAY , pData,  nData );
	}
	void CaDataDicGen::System_NumericPropertySetup(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_NumericPropertyValue(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(m_p_SystemInterface_) 
			m_p_SystemInterface_->II_System_SS_VirtualDesktopEntry( SYSTEM_VD_NUMERIC_PROPERTY_VALUE , pData,  nData );
	}
	void CaDataDicGen::System_StringProperty(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
	}
	void CaDataDicGen::Sytem_NumericValueTable(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
	}
	void CaDataDicGen::Sytem_NumericValueTableItem(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	
	}
	void CaDataDicGen::System_StringTable(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
	}
	void CaDataDicGen::System_SingleUserRequest(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(m_p_SystemInterface_) 
			m_p_SystemInterface_->II_System_SS_VirtualDesktopEntry( SYSTEM_VD_SINGLE_USER_REQUEST , pData,  nData );
	}
	void CaDataDicGen::System_UserRequestTable(tDataDicAccess Access, byte *pData, word nData,  tInfoMessage *MessageInfo)
	{
		if(m_p_SystemInterface_) 
			m_p_SystemInterface_->II_System_SS_VirtualDesktopEntry( SYSTEM_VD_USER_REQUEST_TABLE , pData,  nData );
	}
}//namespace
