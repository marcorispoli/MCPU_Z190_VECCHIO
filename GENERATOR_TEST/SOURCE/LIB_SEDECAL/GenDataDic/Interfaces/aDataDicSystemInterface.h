#ifndef __ADATADICSYSTEMMESSAGEINTERFACE__H_
 #define __ADATADICSYSTEMMESSAGEINTERFACE__H_

#include "aDataDicInterface.h"

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
		tSystemMessageInfo  Message_Info;
	}tSystemMessage;

	class CaDataDicSystemInterface
	{
	public:
		
		/*!
		 * \brief
		 */
		virtual ~CaDataDicSystemInterface() { }
	
		/********************************************* System Message ***********************************/
		/*!
		 *	\brief	Message to indicate when a System Message is active or cleared. It can also request to reset it.
		 *	 		System message additional information can be used to specify the root cause when it is relevant for the user to know. For instance, 
		 *			if we have defined Procedure 1 with two RAD DB’s and DB 2 / Sequence Number 2 settings result in an AEC selection error, 
		 *			it will send the following System Message:
		 */
		virtual byte II_System_SS_SystemMessage(tSystemMessage *pSystemMessage , bool status) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief  Gets System Message Status
		 */
		virtual byte II_System_SS_SystemMessage(dword SystemMessageId) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Requests All System Message
		 */
		virtual byte II_System_SS_RequestAllSystemMessages(void) { return Cp_MessageNotAvailable; }

		/******************************************* Virtual Desktop *******************************/
		/*!
		 * \brief Selection of Virtual Desktop
		 */
		virtual byte II_System_SS_VirtualDesktopEntry( word VirtualType , byte *pData , word nData ) { return Cp_MessageNotAvailable; };

	};
};
#endif
