#ifndef __ADADICRFINTERFACE_H__
	#define __ADADICRFINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{	
	typedef struct
	{
			udword 	mA100;
			udword 	ms100;
			uword  	kV10;
			uword 	MaxIntegrationTime;
			uword   AbcUpdateTime;
			uword 	TargeteLsb;
			uword 	PPS10;
			uword 	DoseLevelId;
			union{
				struct{
					byte 	ABCEnabled		: 1;
					byte	Fl_LockIn		: 1;
					byte 	HighDoseEnabled : 1;
					byte	Reserved		: 5;
				}Fields;
				byte value;
			} DoseCtrl;
			
			byte 	FocalSpot;
			byte 	DatabankId;
			byte 	ImagingSystemProtocol;
			byte 	CurveId;
			byte	FilterId;
			byte	QbyPSS;
	}tFlDb;
	
	typedef struct{
		union{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		} MinkV10;
		union{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}MaxKv10;
		union{
		struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}  MinmA100;
		union{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}  MaxmA100;
	}tFlExpParamRange;
	
	typedef struct
	{
		uword ExposureNumber;
		byte  ProcedureId;
		byte  ExpDbSeqNumber;
		union{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		} kV10;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}mA100;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		} ms100;
		byte	FocalSpot;
		byte	FlEndOfExposure;
	}tFlPostExpPostCondition;
	
	const word 	MAX_FLKVP10_ALLOWED	= 125 * 10;
	const word 	MIN_FLKVP10_ALLOWED	= 40  * 10;
	const dword	MAX_FLMA100_ALLOWED	= 1000 * 100;
	const word 	MIN_FLMA100_ALLOWED	= 1;//0.01 * 100
	const dword	MAX_FLMS100_ALLOWED	= 1000 * 100;
	const dword MIN_FLMS100_ALLOWED = 1 * 100;
	const word  MAX_FLMAT_ALLOWED	= 1000;
	const word  MIN_FLMAT_ALLOWED	= 1;
	const word  MAX_PPS10_ALLOWED	= 120 * 10;
	const word  MIN_PPS10_ALLOWED	= 0;
	
	
	class CaDataDicRfInterface
	{
	public:
		/*!
		 * 
		 */
		CaDataDicRfInterface() {}
		
		/*!
		 * \brief Destructor
		 */
		virtual ~CaDataDicRfInterface() {}
		
		/*!
		 *	\brief Once the CaDataDicGen is instanced this method is called
		 */
		void Initialitation(void) {};

		enum{
			Cp_II_GEN_SS_Databank_IdOutOfRange			= 1,
			Cp_II_GEN_SS_Databank_NotDefined			= 2,
			Cp_II_GEN_SS_IncorrectCommandType			= 10,
		};
		
		/************************************ Generator Index ***************************************/
		enum{
			Cp_II_GEN_SS_FDBkVpValueoutOfRange	= 6,
			Cp_II_GEN_SS_FDBmAValueoutofRange 	= 8,
			Cp_II_GEN_SS_FDBmsValueoutofRange,
			Cp_II_GEN_SS_FDBMaxIntTime,
			Cp_II_GEN_SS_FDBPPSValueoutOfRange,
			Cp_II_GEN_SS_FDBSettingPPSNotSupported,
			Cp_II_GEN_SS_FDBContinuousFluoroscopyNotSupported,
			Cp_II_GEN_SS_FDBCurveIDoutofRange,
			Cp_II_GEN_SS_FDBCurveNotSupported,
			Cp_II_GEN_SS_FDBDataBankTypeNoMatch,
			Cp_II_GEN_SS_FDFocalSpotCodeOutofRange,
			Cp_II_GEN_SS_FDFocalSpotNotSupported,
			Cp_II_GEN_SS_FDFDataBankTypeNotMatch			= 24,
		};
		
		/*!
		 * \brief Sets a FL Data Bank.
		 */
		virtual byte II_Generator_SS_RFDLoad( tFlDb *pFlDb )  { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets a FL Data Bank.
		 */
		virtual byte II_Generator_SS_RFDLoad(byte Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBACCProcedureNoFluoro				= 3,
			Cp_II_GEN_SS_FDBACCNoExposureDbAssigned				= 4,
			Cp_II_GEN_SS_FDBACCExposureDbIdNotDefined			= 5
		};
		
		/*!
		 * \brief Message defined to keep RAD and FL procedure messages consistent.
		 */
		virtual byte II_Generator_SS_RFDAcceptance(byte Procedure_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBKVPkVpValueoutofRange		= 3,
		};

		/*!
		 * \brief Sets FL kV value to a FL Data Bank.
		 */
		virtual byte II_GEN_SS_FDBKVP( byte Databank_Id, short int kVp10orStep , bool CommandType) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets FL kV value to a FL Data Bank.
		 */
		virtual byte II_GEN_SS_FDBKVP(byte Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBMAmAValueOutofRange		= 3,
		};

		/*!
		 * \brief Sets FL mA value to a FL Data Bank.
		 */			 
		virtual byte II_GEN_SS_FDBMA( byte Databank_Id, long int mA100orStep , bool CommandType ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets FL mA value to a FL Data Bank.
		 */
		virtual byte II_GEN_SS_FDBMA( byte Databank_Id ) { return Cp_MessageNotAvailable; } 
		
		enum{
			Cp_II_GEN_SS_FDBMSmsValueoutofRange		= 3,
		};

		/*!
		 * \brief Sets FL ms value (pulse width) to a FL Data Bank.
		 */
		virtual byte II_GEN_SS_FDBMS(byte Databank_Id, int ms100orStep , bool CommandType) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets FL ms value (pulse width) to a FL Data Bank.
		 */
		virtual byte II_GEN_SS_FDBMS(byte Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDBMaxIntTimeValue = 3,
		};
		/*!
		 * \brief Sets maximum integration time in a FL Data Bank.
		 */
		virtual byte II_Generator_SS_FDBMaxIntTime( byte DatabankId , word MaxIntTime ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets maximum integration time in a FL Data Bank.
		 */
		virtual byte II_Generator_SS_FDBMaxIntTime( byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_GEN_SS_FDB_PPSValueOutofRange = 3,
			CP_II_GEN_SS_FDB_SettingPPSNotSupported,
			CP_II_GEN_SS_FDB_ContinuousFluoroscopyNotSupported
		};

		/*!
		 * \brief Sets PPS in a FL Data Bank.
		 */
		virtual byte II_Generator_SS_FDBPPS(byte Databank_Id, word PPS10  ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets PPS in a FL Data Bank.
		 */
		virtual byte II_Generator_SS_FDBPPS(byte Databank_Id) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Sets Abc , Lock In and High Dose 
		 */
		virtual byte II_Generator_SS_FDAbcHighDoseLockIn( byte DatabankId , bool IsAbc , bool IsFlLocking , bool HighDose ){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets Abc , Lock In and High Dose 
		 */
		virtual byte II_Generator_SS_FDAbcHighDoseLockIn( byte DatabankId ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Set QbyPPS
		 */
		virtual byte II_Generator_SS_FDQbyPPS( byte DatabankId , bool QbyPPS ) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Get QbyPPS 
		 */
		virtual byte II_Generator_SS_FDQbyPPS( byte DatabankId ) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Sets Target Lsb 
		 */
		virtual byte II_Generator_SS_FDTargetLsb( byte DatabankId , word TargetLsb ) { return Cp_MessageNotAvailable; } 
		/*!
		 * \brief Gets Target Lsb
		 */
		virtual byte II_Generator_SS_FDTargetLsb( byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum
		{
			CP_II_GEN_SS_FSSFocalSpotCodeOutOfRange = 3,
			CP_II_GEN_SS_FSSFocalSpotNotSupported
		};
		/*!
		 * \brief Sets Target Lsb 
		 */
		virtual byte II_Generator_SS_FDFocalSpot( byte DatabankId , byte FocalSpot ) { return Cp_MessageNotAvailable; }
		/*!
		 * \brief Gets Target Lsb
		 */
		virtual byte II_Generator_SS_FDFocalSpot( byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		
		/*!
		 * \brief Sets Abc Update Time
		 */
		virtual byte II_Generator_SS_FDAbcUpdateTime( byte DatabankId , word AbcUpdateTime ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Sets Abc Update Time
		 */
		virtual byte II_Generator_SS_FDAbcUpdateTime( byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Sets Dose Level ID in a FL Data Bank. 
		 */
		virtual byte II_Generator_SS_FDBDoseLevelID( byte DatabankId, word DoseLevelId ) { return Cp_MessageNotAvailable; }	
		
		/*!
		 * \brief Gets Dose Level ID in Fl Databank
		 */
		virtual byte II_Generator_SS_FDBDoseLevelID( byte DatabankId ) { return Cp_MessageNotAvailable; }	
		
		/*!
		 * \brief Sets New Filter Id
		 */
		virtual byte II_Generator_SS_FDBFilterId( byte DatabankId, byte DoseLevelId ) { return Cp_MessageNotAvailable; }	
		
		/*!
		 * \brief Gets Filter Id
		 */
		virtual byte II_Generator_SS_FDBFilterId( byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum
		{
			CP_II_GEN_SS_FDCCurveIDOutOfRange = 3,
			CP_II_GEN_SS_FDCurveNotSupported
		};
		/*!
		*	\brief 	Sets Curve ID in a FL Data Bank.
		*/
		virtual byte II_Generator_SS_FDBCurveId (byte DatabankId, byte CurveId ) { return Cp_MessageNotAvailable; }	

		/*!
		*	\brief 	Gets Curve ID in a FL Data Bank.
		*/
		virtual byte II_Generator_SS_FDBCurveId (byte DatabankId ) { return Cp_MessageNotAvailable; }	
		
		/****************************** CURRENT FLUORO EXPOSURE DATA BANK *********************************/
		/*!
		 * \brief Current Fluoro Exposure Data Bank
		 */
		virtual byte II_Generator_SS_FDBCurrentFlDataBank(void) { return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank kVp value.
		*/
		virtual byte II_Generator_SS_FDBCurrentkVp(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank mA value.
		*/
		virtual byte II_Generator_SS_FDBCurrentmA(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank ms value.
		*/
		virtual byte II_Generator_SS_FDBCurrentms(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank MinIntegrationTime value.
		*/
		virtual byte II_Generator_SS_FDBCurrentMaxIntegrationTime(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank FPS.
		*/
		virtual byte II_Generator_SS_FDBCurrentPPS(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank ABC setting.
		*/
		virtual byte II_Generator_SS_FDBCurrentABC(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank High Dose value.
		*/
		virtual byte II_Generator_SS_FDBCurrentHighDose(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Current Fluoro Data Bank kV Scan
		 */
		virtual byte II_Generator_SS_FDBCurrentkVScan(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Current Fluoro Data Bank QbyPPS setting.
		 */
		virtual byte II_Generator_SS_FDBCurrentQbyPPS(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank Dose Level ID.
		*/
		virtual byte II_Generator_SS_FDBCurrentDoseLevelId(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current Fluoro Data Bank Curve ID.
		*/
		virtual byte II_Generator_SS_FDBCurrentCurveId(void){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Resets fluoro time.
		 */
		virtual byte II_Generator_SS_FDBFluoroTimeReset(void){ return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Resets 5-minute fluoro alarm.
		 */
		virtual byte II_Generator_SS_FDBFiveMinuteFluoroAlarmReset(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief FlPost Exposure Fl Post Condition
		 */
		virtual byte II_Generator_SS_FlPostExposure(void){ return Cp_MessageNotAvailable; }
		
		 /*!
		  *	\brief This message contains the information about the FLUORO exposure parameter ranges.
		  */
		 virtual byte II_Generator_SS_FlExpParamRanges(void) { return Cp_MessageNotAvailable; }
		 /*!
		  * \brief This message constains the information about current Lsb measured by Image System
		  */
		virtual byte II_ImageSystem_CurrentLsb( word ExpNumber , word CurrentLsb ) { return Cp_MessageNotAvailable; }
			
	};//class
};//namesapce
#endif
