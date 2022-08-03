#ifndef __ADATADICSYSTEMMESSAGEINTERFACE__H_
 #define __ADATADICSYSTEMMESSAGEINTERFACE__H_

#include "aDataDicInterface.h"
#include <QList>

namespace R2CP
{
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
            if(pItem->Active){
                for(int i=0; i< messageList.size(); i++){
                    if(messageList[i].Id == pItem->Id) return false; // Already active
                }
                messageList.append(*pItem);
                return true;
            }else{
                if(messageList.size() == 0) return false;
                for(int i=0; i< messageList.size(); i++){
                    if(messageList[i].Id == pItem->Id){
                        messageList.remove(i);
                        return true;
                    }
                }
                return false;
            }
            return false;
        }

        tSystemMessage* getMessage(uint ID){
            for(int i=0; i< messageList.size(); i++){
                if(messageList[i].Id == ID) return &messageList[i]; // Already active
            }
            return nullptr;
        }

        QList<tSystemMessage> messageList;

	};



};
#endif
