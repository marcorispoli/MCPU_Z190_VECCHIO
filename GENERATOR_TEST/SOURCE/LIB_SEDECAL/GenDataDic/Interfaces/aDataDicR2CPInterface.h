#ifndef __ADATADICR2CPINTERFACE_H__
	#define	__ADATADICR2CPINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{
	typedef struct
	{
		word Hardware_Model;	/*!< Hw model version. */
		byte Hardware_Version;	/*!< Hw version. */
		char Hardware_Revision;	/*!< Hw revision. */
		byte SwVersion;			/*!< Sw version. */
		byte SwRevision;		/*!< Sw revision. */
		byte SwSubrevision;		/*!< Sw subrevision. */
		byte BootVersion;		/*!< Boot version. */
		byte BootRevision;		/*!< Boot revision. */
		byte BootSubrevision;	/*!< Boot subrevision. */
		byte HdlVersion;		/*!< Hdl version. */
		byte HdlRevision;		/*!< Hdl revision. */
		byte HdlSubrevision;	/*!< Hdl subrevision. */
		byte GoldenVersion;		/*!< Golden version. */
		byte GoldenRevision;	/*!< Golden revision. */
		byte GoldenSubrevision;	/*!< Golden subrevision. */
		byte Index;
		union 
		{
			struct
			{
				byte HwVersion		:1;
				byte SwVersion		:1;
				byte BootVersion	:1;
				byte HdlVersion		:1;
				byte GolderVersion	:1;
			} Fields;
			byte value;
		} FieldToApply;
	}tSystemVersion;
	
	class CaDataDicR2CPInterface
	{
	public:
		/*!
		 *\brief Constructor 
		 */
		CaDataDicR2CPInterface() { }
		
		/*!
		 * \brief Destructor
		 */
		virtual ~CaDataDicR2CPInterface() { }
		
		/*!
		 * \brief Class Initialitation
		 */
		virtual void Initialitation(void) { }

		/*!
		 * \brief Cleans Last Error 
		 */
		virtual byte II_R2CP_SS_Clean_Error(void) {return Cp_MessageNotAvailable;}

		/*!
		 * \brief Gets all Error
		 */
		virtual byte II_R2CP_SS_Get_Error(void) { return Cp_MessageNotAvailable;}

		/*!
		 * \brief Sets a Board Reset
		 */
		virtual byte II_R2CP_SS_Reset(void) { return Cp_MessageNotAvailable;}

		/*!
		 * \brief Sets MasterLiveTime
		 */
		virtual byte II_R2CP_SS_MasterLiveTime(word Time2Configure) { return Cp_MessageNotAvailable;}

		/*!
		 * \brief Send Package Version
		 */
		virtual byte II_R2CP_SS_PackageVersion(char* pPackageVersion) { return Cp_MessageNotAvailable;}
		
		/*!
		 * \brief Sets SerialNumber
		 */
		virtual byte II_R2CP_SS_SerialNumber( byte *pSerialNumber, word SizeSerialNumber) { return Cp_MessageNotAvailable;}
		
		/*!
		 * \brief Gets System Versions
		 */
		virtual byte II_R2CP_SS_SystemVersion(  void  ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Reverved Got
		 */
		virtual bool ReservedGot( byte Priority, byte Node , byte Index , byte SubIndex , byte *pData , word Size ){ return Cp_MessageNotAvailable;}
	};
};

#endif
