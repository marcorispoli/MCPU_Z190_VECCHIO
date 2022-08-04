/*!
 * \file      R2CPslaveInterface.cpp
 * \brief     Implementations for the class R2CP.
 * \author    Francisco Javier Sanchez Rabadán <francisco.sanchez@sedecal.com>.
 * \bug       Unknown bugs
 * \version
 *  \htmlonly
 *		<A HREF="https://svn.sedecal.com/software/Embedded Drivers/HCS12/R2CP/tags/V1R01.1">V1R01.1</A>
 *	\endhtmlonly
 * \date      01/08/2012
 * \copyright SEDECAL S.A.
 * \ingroup   R2CPModule
*/

#include "R2CP_Eth.h"
#include "string.h"
#include "application.h"

//#include <stdio.h>

//using namespace Util;

/*!
 * 
 */
byte CR2CP_Eth::m_token_granted;


///
///TIME TOKEN & HEARTBEAT
///
#define TIME_RELEASE_TOKEN		10000//token_release

///
///WORKFLOW SUBINDEX
///
#define II_WORKFLOW_SS_TOKEN	30

///
///TOKEN WORKFLOW SUBINDEX
///
#define TOKEN_REQUEST_ID	0	
#define	TOKEN_RELEASE_ID	1
#define TOKEN_REFRESH_ID	2


CR2CP_Eth::CR2CP_Eth(	int16_t (*cbSendMsgEth)( byte *pMessage , word datalength ), 
						CaDataDic* ( *cbSearchDatadicSlave )(byte Node),
						CaDataDic* pEthDataDic,
						void (*cbJumpToBootloader)(void) ,
						word targetId)
{
	mCbSendMsgEth 				= 	cbSendMsgEth;
	mCbJumpToBootloader 		= 	cbJumpToBootloader; 
	mCbSearchDatadicSlave		= 	cbSearchDatadicSlave;
	m_token_granted 			= 	R2CP_ETH_BROADCAST_NODE_ID;
	mTargetId 					= 	targetId;
	mCbAuxDataDicToDownloader 	= 	nullptr;
	mpEthDatadic				= 	pEthDataDic;
			
	mBlockManager.nBlockDataExpected = 0;
	mBlockManager.nCurrentBlockData  = 0;
	mBlockManager.BlockDataSequenceExpected = 0;
	mBlockManager.pBlockData = nullptr;
	mBlockManager.BlockAccess = 0;
	mBlockManager.infomessage.Node_dest=0;
	mBlockManager.infomessage.Node_own=0;
	mBlockManager.infomessage.Sequence=0;
	mBlockManager.infomessage.Function=0;
	mBlockManager.infomessage.Priority=0;
}

CR2CP_Eth::~CR2CP_Eth()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: ProcessMessage
// Decode a message
//
void CR2CP_Eth::ProcessMessage(void *pMessage)
{
	if( mpEthDatadic == nullptr )
		return;
		
	tSocket_Msg_ptr pEthMsg = reinterpret_cast< tSocket_Msg_ptr >( pMessage );
	
	//printf("\n\r Index [ %02X ] SubIndex [ %02X ] Funtion[ %d ] Node [ %d ]",pEthMsg->header.index , pEthMsg->header.subindex , pEthMsg->header.function , pEthMsg->header.dest_node );
	///
	///
	///
	switch(pEthMsg->header.function)
	{
	case ETH_RESERVED_FNC:
		if( mCbAuxDataDicToDownloader )
		{
			mCbAuxDataDicToDownloader->ReservedGot(		pEthMsg->header.priority, 
														pEthMsg->header.iss_node , 
														pEthMsg->header.index , 
														pEthMsg->header.subindex , 
														pEthMsg->data, 
														htons( pEthMsg->header.len ));
		}
		
		break;
	case ETH_DOWNLOAD_START:
	{
		word targetId = pEthMsg->header.index << 8 | pEthMsg->header.subindex;
		///
		///Check if the target Id is for us
		///
		if( targetId == mTargetId && mCbJumpToBootloader)
		{
			mCbJumpToBootloader();
		}
		else
		{
			//
			//GET THE DICTIONARY ACCORDING TO THE NEXT SENDER
			//
			mCbAuxDataDicToDownloader = mCbSearchDatadicSlave( pEthMsg->data[ htons( pEthMsg->header.len ) - 1 ] ); //Obtenemos el diccionario
			if( mCbAuxDataDicToDownloader != nullptr )
			{
				mCbAuxDataDicToDownloader->DownloadGot( targetId , pEthMsg->data , htons( pEthMsg->header.len ) - 1);
			}
		}
		return;
	}
		break;
    case ETH_MSG_PROCESSED_FNC:
        if(pEthMsg->header.len != 2) return;
           if(COMMUNICATION) COMMUNICATION->handleCommandProcess(pEthMsg->data[0], pEthMsg->data[1]);
           return;
        break;
	default:
		break;
	}
	
	switch(pEthMsg->header.index)
	{
		case II_Workflow:
			this->ProcessWorkflow(static_cast<eEthPriorities>(	pEthMsg->header.priority ),
																pEthMsg->header.iss_node,
																pEthMsg->header.dest_node,
																pEthMsg->header.index,
																pEthMsg->header.subindex,
																pEthMsg->header.function,
																pEthMsg->header.len,
																pEthMsg->data);
			return;
			break;
		default:
			break;
	}
	
	if( m_token_granted == R2CP_ETH_BROADCAST_NODE_ID ||  m_token_granted == pEthMsg->header.iss_node )//ELSE The token is given to another node
	{		
		if( pEthMsg->header.dest_node == R2CP_ETH_BROADCAST_NODE_ID )
			pEthMsg->header.dest_node = mpEthDatadic->mNodeId;
		
		// CHECKING FUNCTION   
		switch(pEthMsg->header.function)
		{
			case ETH_SET_FNC:
			case ETH_GET_FNC:
			case ETH_EVENT_ANSWER_FNC:
			{	//crosses initialization
				///
				///Give an id in case a thread is launched by the dictionary
				///This id is placed in the last argument of the data message
				///
				tInfoMessage infomessage={ pEthMsg->header.iss_node , pEthMsg->header.dest_node , pEthMsg->header.sequence , pEthMsg->header.function , pEthMsg->header.priority , pEthMsg->header.subindex , nullptr };	
				// Checking right access and calling dictionary
				byte rval = mpEthDatadic->DataDicCheckAndCall(	pEthMsg->header.index,
																pEthMsg->header.subindex,
																pEthMsg->header.dest_node,
																(tDataDicAccess)(0x01<<(pEthMsg->header.function - 1)),
																pEthMsg->data,
																htons( pEthMsg->header.len ),// NEED TO EXCHANGE THE BYTES	
																&infomessage);
				if( rval == 0)
				{
					return; // Right access
				}
				//Si recibimos un evento, no enviar nunca un cp wrong access
				if( pEthMsg->header.function == ETH_EVENT_ANSWER_FNC )
					break;
				
				byte data[]={ pEthMsg->header.sequence , rval};
				(void)Processed(	static_cast<eEthPriorities>(pEthMsg->header.priority),
									pEthMsg->header.iss_node,
									pEthMsg->header.dest_node,
									pEthMsg->header.index,
									pEthMsg->header.subindex,
									0x02,
									data);
			}
			
			break;
			case ETH_BLOCK_FNC:
			{
				tInfoMessage infomessage;

		    	infomessage.Node_dest	= pEthMsg->header.dest_node;
		    	infomessage.Node_own	= pEthMsg->header.iss_node;
		    	infomessage.Sequence	= pEthMsg->header.sequence;
		    	infomessage.Function	= pEthMsg->header.function;
		    	infomessage.Priority	= pEthMsg->header.priority;
			    
			    // Making the block
			    if( ProcessBlock( mpEthDatadic , pEthMsg->header.index, pEthMsg->header.subindex, htons(pEthMsg->header.len), pEthMsg->data, &infomessage) )
			    {
					byte data[]={pEthMsg->header.sequence,ERROR_MESSAGE_NOT_AVAILABLE};
					(void)Processed(static_cast<eEthPriorities>(pEthMsg->header.priority),
									pEthMsg->header.iss_node,
									pEthMsg->header.dest_node,
									pEthMsg->header.index,
									pEthMsg->header.subindex,
									0x02,
									data);
			    }
			}
			break;
			default:
			break;
		}
	}
	else{
		Processed(	static_cast<eEthPriorities>(pEthMsg->header.priority), 
					pEthMsg->header.iss_node, 
					pEthMsg->header.dest_node,
					pEthMsg->header.index, 
					pEthMsg->header.subindex, 
					0x01, 
					reinterpret_cast<byte *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));
		return;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Reception of blocks. 
//
bool CR2CP_Eth::ProcessBlock(CaDataDic * pDataDic, byte index, byte subindex, byte dataLength, byte *pdata, tInfoMessage * pInfoMessage)
{
	bool result = false;

	if( dataLength == 0 || pdata == nullptr )
	{
        mBlockManager.nBlockDataExpected = 0;
        mBlockManager.nCurrentBlockData = 0;
        mBlockManager.BlockDataSequenceExpected = 0;
        mBlockManager.BlockAccess = 0;
    	mBlockManager.infomessage.Node_dest=0;
    	mBlockManager.infomessage.Node_own=0;
    	mBlockManager.infomessage.Sequence=0;
    	mBlockManager.infomessage.Function=0;
    	mBlockManager.infomessage.Priority=0;
    	
        if( mBlockManager.pBlockData != nullptr )
        {
#if 0
          delete [] mBlockManager.pBlockData;
          mBlockManager.pBlockData = nullptr;
#endif
          memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
        }
        return true;
	}
		
	switch( pdata[0] )
	{
		case 0xFE:
			
			result = true;	
			if( dataLength == 4 )
			{
				mBlockManager.nBlockDataExpected = (word)pdata[1]<<8 | (word)pdata[2];
				
				// Checking function command   
				switch(	pdata[3] )
				{
					case ETH_SET_FNC:
					case ETH_GET_FNC:
					case ETH_EVENT_ANSWER_FNC:
				    	mBlockManager.infomessage.Node_dest=0;
				    	mBlockManager.infomessage.Node_own=0;
				    	mBlockManager.infomessage.Sequence=0;
				    	mBlockManager.infomessage.Function=0;
				    	mBlockManager.infomessage.Priority=0;
				    	
						mBlockManager.BlockAccess = (tDataDicAccess)(0x01<<(pdata[3]-1));
						if( mBlockManager.pBlockData != nullptr )
						{
#if 0
							delete [] mBlockManager.pBlockData;
							mBlockManager.pBlockData = nullptr;
#endif
							memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
						}
						if( mBlockManager.pBlockData == nullptr )
						{
							mBlockManager.pBlockData = new byte[ MAX_BLOCK_ETH_SIZE ];
						}
						
						if( mBlockManager.pBlockData != nullptr )
						{
							result = false;
						}
						else
						{
							//TODO: Gpio.AddError( ERROR_MEMORY);
						}
						break;
				}
			}
			
			if( result )
			{         
				//(void)WrongAccess(pDataDic->mNodeId,index,subindex,0, (byte *)pNULL);
			}
			mBlockManager.nCurrentBlockData = 0;
			mBlockManager.BlockDataSequenceExpected = 0;
			break;
			
		case 0xFF:
			if( dataLength == 1 && mBlockManager.nBlockDataExpected == mBlockManager.nCurrentBlockData )
			{
				mBlockManager.infomessage = *pInfoMessage;
				// Checking right access and calling dictionary
				if( pDataDic )
				{
					bool result = pDataDic->DataDicCheckAndCall(		index,
							subindex,
							mBlockManager.infomessage.Node_dest,
							(tDataDicAccess)mBlockManager.BlockAccess,
							mBlockManager.pBlockData,
							mBlockManager.nCurrentBlockData,
							&mBlockManager.infomessage);
					if( !result )
					{
						mBlockManager.nBlockDataExpected = 0;
						mBlockManager.nCurrentBlockData = 0;
						mBlockManager.BlockDataSequenceExpected = 0;
						mBlockManager.BlockAccess = 0;
						if( mBlockManager.pBlockData != nullptr )
						{
#if 0
						  delete [] mBlockManager.pBlockData;
						  mBlockManager.pBlockData = nullptr;
#endif
						  memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
						}
						mBlockManager.infomessage.Node_dest=0;
						mBlockManager.infomessage.Node_own=0;
						mBlockManager.infomessage.Sequence=0;
						mBlockManager.infomessage.Function=0;
						mBlockManager.infomessage.Priority=0;
						return false;
					}
				}
			}
	        //(void)WrongAccess(pDataDic->mNodeId,index,subindex,0, (byte *)pNULL);
			result = true;
			break;
			
		default:
			if( dataLength > 1 &&
				pdata[0] == mBlockManager.BlockDataSequenceExpected &&
				mBlockManager.nCurrentBlockData < mBlockManager.nBlockDataExpected )
			{
				(void)memcpy(&mBlockManager.pBlockData[mBlockManager.nCurrentBlockData],&pdata[1],dataLength - 1);
				mBlockManager.nCurrentBlockData += dataLength - 1;
				mBlockManager.BlockDataSequenceExpected++;
			}
			else{
				result = true;
			} 
			break;
	}
    
	if( result )
	{
        mBlockManager.nBlockDataExpected = 0;
        mBlockManager.nCurrentBlockData = 0;
        mBlockManager.BlockDataSequenceExpected = 0;
        mBlockManager.BlockAccess = 0;
        if( mBlockManager.pBlockData != nullptr )
        {
#if 0
          delete [] mBlockManager.pBlockData;
          mBlockManager.pBlockData = nullptr;
#endif
          memset( mBlockManager.pBlockData , 0 , MAX_BLOCK_ETH_SIZE );
        }
    	mBlockManager.infomessage.Node_dest=0;
    	mBlockManager.infomessage.Node_own=0;
    	mBlockManager.infomessage.Sequence=0;
    	mBlockManager.infomessage.Function=0;
    	mBlockManager.infomessage.Priority=0;
	}
  
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: ProtFunction
// Makes and sends CAN message
//
bool CR2CP_Eth::ProtFunction( eEthPriorities prio ,byte node_dest ,byte node_iss , byte index, byte subindex, tEthFunction fnc, word dataLength, byte *pdata)
{
    /*
	if( mCbSendMsgEth == nullptr )
	{
		return true;
	} 
    */
	if( dataLength > MAX_LENGTH_BUFFER_ETH )
	{
#ifdef _DEBUG
		_io_printf("\n ___ ETH PROTFUNCION MAX_LENGTH_BUFFER_ETH II [ %d ] SS [ %d ] [ %d ] [ %s ]" , index , subindex , dataLength , pdata );
#endif
		return true;
	}
	
	tSocket_Msg Eth_Msg = { 0 };
	
	Eth_Msg.header.sequence 	= Get_Sequence();
	Eth_Msg.header.priority 	= prio;
	Eth_Msg.header.dest_node 	= node_dest;
	Eth_Msg.header.iss_node		= node_iss;
	Eth_Msg.header.index		= index;
	Eth_Msg.header.subindex		= subindex;
	Eth_Msg.header.function		= fnc;
	Eth_Msg.header.len			= htons( dataLength );
 	
 	if( pdata )
 	{
 		memcpy( Eth_Msg.data , pdata , dataLength );
 	}

 	return mCbSendMsgEth( reinterpret_cast<byte *>(&Eth_Msg), HEADER_SIZEOF + dataLength );
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Set
// Sends SET message
//
bool CR2CP_Eth::Set(eEthPriorities prio , byte node_dest, byte node_iss, byte index, byte subindex, byte dataLength, byte *pdata)
{  
	return ProtFunction(prio, node_dest, node_iss , index, subindex, ETH_SET_FNC , dataLength, pdata);  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Get
// Sends GET message
//
bool CR2CP_Eth::Get(eEthPriorities prio , byte node_dest, byte node_iss, byte index, byte subindex, byte dataLength, byte *pdata)
{   
  return ProtFunction(prio, node_dest, node_iss , index, subindex, ETH_GET_FNC , dataLength, pdata);  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Event_Answer
// Sends EVENT and Answer message
//
bool CR2CP_Eth::Event_Answer(eEthPriorities prio , byte node_dest, byte node_iss, byte index ,byte subindex, word dataLength, byte *pdata)
{
	return ProtFunction(prio, node_dest , node_iss , index , subindex , ETH_EVENT_ANSWER_FNC , dataLength , pdata);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Processed
// SEND PROCESSED MESSAGE
//
bool CR2CP_Eth::Processed(eEthPriorities prio , byte node_dest, byte node_iss, byte index, byte subindex, byte dataLength, byte *pdata)
{
	return ProtFunction( prio, node_dest , node_iss , index , subindex , ETH_MSG_PROCESSED_FNC , dataLength , pdata );	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: WrongAccess
//
bool CR2CP_Eth::WrongAccess(eEthPriorities prio , byte node_dest, byte node_iss, byte index, byte subindex, byte dataLength, byte *pdata){
	byte data[] = {pdata[0],ERROR_MESSAGE_NOT_AVAILABLE};
	return Processed(prio , node_dest, node_iss, index, subindex, 0x02, data);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Reserved
//
bool CR2CP_Eth::Reserved(eEthPriorities prio , byte node_dest, byte node_iss, byte index, byte subindex, byte dataLength, byte *pdata)
{
	return ProtFunction( prio, node_dest , node_iss , index , subindex , ETH_RESERVED_FNC , dataLength , pdata );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: SendRawData
//
bool CR2CP_Eth::SendRawData(eEthPriorities prio , byte node_dest, byte node_iss, byte index ,byte subindex, byte function ,word dataLength, byte *pdata)
{
	reinterpret_cast<tSocket_Header_ptr>(pdata)->sequence   = this->Get_Sequence();
	reinterpret_cast<tSocket_Header_ptr>(pdata)->priority	= prio;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->dest_node	= node_dest;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->iss_node	= node_iss;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->index	    = index;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->subindex  	= subindex;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->function  	= function;
	reinterpret_cast<tSocket_Header_ptr>(pdata)->len       	= htons( dataLength );
	
	if( mCbSendMsgEth == nullptr )
	{
		return true;
	} 
	
 	return mCbSendMsgEth( pdata, dataLength + HEADER_SIZEOF);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: ProcessWorkflow
//
bool CR2CP_Eth::ProcessWorkflow(byte priority, byte node_dest, byte node_iss, byte index, byte subindex, byte function, byte dataLength, byte *pdata){
	switch(function){
	case ETH_SET_FNC:
		switch(pdata[1]){
		case TOKEN_REQUEST_ID:
			if(m_token_granted == R2CP_ETH_BROADCAST_NODE_ID || m_token_granted == pdata[0]){
				m_token_granted = pdata[0];
				
				ProtFunction( 	static_cast<eEthPriorities>(priority) , 
								R2CP_ETH_BROADCAST_NODE_ID , 
								node_iss , 
								II_Workflow , 
								II_WORKFLOW_SS_TOKEN , 
								ETH_EVENT_ANSWER_FNC, 
								0x01, 
								&m_token_granted);
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<byte *>(TOKEN_IS_GRANTED));
				
				//m_p_timer_token_request->Load(m_TimerNodeToken_assigned,TIME_RELEASE_TOKEN);
			}
			else
				Processed(	static_cast<eEthPriorities>(priority),
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<byte *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));
			break;
		case TOKEN_RELEASE_ID:
			if( m_token_granted == pdata[0]){
				m_token_granted = R2CP_ETH_BROADCAST_NODE_ID;
				ProtFunction( 	static_cast<eEthPriorities>(priority) , 
								R2CP_ETH_BROADCAST_NODE_ID , 
								node_iss , 
								II_Workflow , 
								II_WORKFLOW_SS_TOKEN , 
								ETH_EVENT_ANSWER_FNC, 
								0x01, 
								&m_token_granted);
				
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<byte *>(TOKEN_IS_GRANTED));
			}
			else
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<byte *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));
			break;
		case TOKEN_REFRESH_ID:
			if( m_token_granted == pdata[0]){
				ProtFunction( 	static_cast<eEthPriorities>(priority) , 
								R2CP_ETH_BROADCAST_NODE_ID , 
								node_iss , 
								II_Workflow , 
								II_WORKFLOW_SS_TOKEN , 
								ETH_EVENT_ANSWER_FNC, 
								0x01, 
								&m_token_granted);
				
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<byte *>(TOKEN_IS_GRANTED));
				//m_p_timer_token_request->Load(m_TimerNodeToken_assigned,TIME_RELEASE_TOKEN);
			}
			else					
				Processed(	static_cast<eEthPriorities>(priority), 
							node_iss,
							node_dest, 
							index, 
							subindex, 
							0x01, 
							reinterpret_cast<byte *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));			
			break;
		default:
			break;
		}
		break;
		case ETH_GET_FNC:
			ProtFunction( 	static_cast<eEthPriorities>(priority) , 
							R2CP_ETH_BROADCAST_NODE_ID , 
							node_iss , 
							II_Workflow , 
							II_WORKFLOW_SS_TOKEN , 
							ETH_EVENT_ANSWER_FNC, 
							0x01, 
							&m_token_granted);
			
			Processed(	static_cast<eEthPriorities>(priority), 
						node_iss,
						node_dest, 
						index, 
						subindex, 
						0x01, 
						(m_token_granted != R2CP_ETH_BROADCAST_NODE_ID) ? 
								reinterpret_cast<byte *>(TOKEN_IS_GRANTED) : 
								reinterpret_cast<byte *>(ERROR_CODE_TOKEN_ALREADY_GRANTED));						
			break;
		default:
		break;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Get_Sequence 
//
uint8_t CR2CP_Eth::Get_Sequence(void){
	if(m_sequence > 0x3F)
		m_sequence = 1;
	return m_sequence++;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNC: Get_Sequence
// CALLBACK IN CASE SWTIMER EXPIRED
//
void CR2CP_Eth::Callback_token_expired(byte node){
	m_token_granted = R2CP_ETH_BROADCAST_NODE_ID;
}


