#ifndef __ADATADICSYSTEMMESSAGEINTERFACE__H_
 #define __ADATADICSYSTEMMESSAGEINTERFACE__H_

#include "aDataDicInterface.h"
#include <QList>

namespace R2CP
{
// #define GENERAL_RX_DISABLE_MSG_ID ((ulong) 500074)
 const ulong  GENERAL_RX_DISABLE_MSG_ID		= 500074;

	typedef struct{
		byte IssuingNode;
		byte InhibitGeneratorPhase;
		byte InhibitPositionerPhase;//Cambiar
		bool InhibitRX;
		bool InhibitMovement;
		bool ClearByIssuingNode;
	}tSystemMessageInfo;
	
	typedef struct{
		dword 				Id;
        bool                Active;
		tSystemMessageInfo  Message_Info;
	}tSystemMessage;

	class CaDataDicSystemInterface
	{
	public:
		
		/*!
		 * \brief
		 */
		virtual ~CaDataDicSystemInterface() { }
	
        bool handleMessage(tSystemMessage* pItem){
            if(pItem->Id == R2CP::GENERAL_RX_DISABLE_MSG_ID){
                if(pItem->Active) rx_disable_msg = true;
                else rx_disable_msg = false;
                return true;
            }

            if(pItem->Active){
                for(int i=0; i< messageList.size(); i++){
                    if(messageList[i] == pItem->Id) return false; // Already active
                }
                messageList.append(pItem->Id);
                return true;
            }else{
                if(messageList.size() == 0) return false;
                for(int i=0; i< messageList.size(); i++){
                    if(messageList[i] == pItem->Id){
                        messageList.remove(i);
                        return true;
                    }
                }
                return false;
            }
            return false;
        }

        QList<ulong> messageList;
        bool rx_disable_msg;

	};



};
#endif
