#ifndef _ADATADICINTERFACE_H__
	#define _ADATADICINTERFACE_H__

#include "Typedef.h"

namespace R2CP
{
	enum{
		Cp_Ok = 0,
		Cp_Nok,
		Cp_MessageNotAvailable = 202
	};
	
	typedef union
	{
		struct{
			dword data_0_7	 : 8;
			dword data_8_15	 : 8;
			dword data_16_23 : 8;
			dword data_24_31 : 8;
		}Fields;
		dword value;
	}udword;
	
	typedef union
	{
		struct{
			word data_0_7	 : 8;
			word data_8_15	 : 8;
		}Fields;
		word value;
	}uword;
	
	typedef union
	{
		struct{
			byte data_0_3	: 4;
			byte data_4_7	: 4;
		}Fields;
		byte value;
	}ubyte;
		
	class CaDataDicInterface
	{
	public:
				
		/*!
		 * \brief Default Destructor
		 */
		virtual ~CaDataDicInterface(){ }
	
		/*!
		 * \brief Initialitation
		 */
		virtual void Initialitation(void){ }

		/*!
		 * \brief According to the generator status some messages cannot be replied.
		 */
		virtual bool CheckNotAvailableMessages_vs_Status( word Index , word SubIndex ) { return true; }

	};
};
#endif
	
