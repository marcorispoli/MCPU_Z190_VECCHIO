#ifndef _ADATADICTEST_H__
	#define _ADATADICTEST_H__

#include "Typedef.h"

namespace R2CP
{		
	class CaDataDicTestInterface
	{
	public:
				
		/*!
		 * \brief Default Destructor
		 */
		virtual ~CaDataDicTestInterface(){}
	
		/*!
		 * \brief Initialitation
		 */
		virtual void Initialitation(void){}
		
		/*!
		 * \brief
		 */
		virtual byte II_Test_SS_HandSwitch_FootSwitch( bool PrepOrder , bool ExpOrder , bool FlOrder , bool DynMode_1 , bool DynMode_2 ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief
		 */
		virtual byte II_Test_SS_HandSwitch_FootSwitch( void );
				
		/*!
		 * \brief
		 */
		virtual byte II_Test_SS_Error( dword Id , bool Status , byte Type ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief
		 */
		virtual byte II_Test_SS_PowerControl( byte *pData , byte nData ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief
		 */
		virtual byte II_Test_SS_Uarc( byte *pData , byte nData ){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief
		 */
		virtual byte II_Test_SS_Enable_Log( byte LogId , bool Status ){ return Cp_MessageNotAvailable; }
		
	};
};
#endif
	
