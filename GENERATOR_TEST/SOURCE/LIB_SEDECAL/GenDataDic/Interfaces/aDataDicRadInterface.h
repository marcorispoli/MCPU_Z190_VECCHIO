#ifndef __ADADICRADINTERFACE_H__
	#define __ADADICRADINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{	
	typedef struct
	{
			byte DatabankId;
			byte ImagingSystemProtocolId;
			union RadDB_Tech_{
				struct{
					byte TechniqueSel:		6;
					byte AutoTechMode:		1;
					byte AdjustParamAuto:	1; 
				}Fields;
				byte value;
			}TechMode;
			union RadDB_kV_{
				struct{
					word data_0_7	 : 8;
					word data_8_15	 : 8;
				}Fields;
				word value;
			}kV10;
			union RadDB_mAs1000_{
				struct{
					dword data_0_7	 : 8;
					dword data_8_15	 : 8;
					dword data_16_23 : 8;
					dword data_24_31 : 8;
				}Fields;
				dword value;
			}mAs1000;
			union Rad_mA100_{
				struct{
					dword data_0_7	 : 8;
					dword data_8_15	 : 8;
					dword data_16_23 : 8;
					dword data_24_31 : 8;
				}Fields;
				dword value;
			}mA100;
			union Rad_ms100_{
				struct{
					dword data_0_7	 : 8;
					dword data_8_15	 : 8;
					dword data_16_23 : 8;
					dword data_24_31 : 8;
				}Fields;
				dword value;
			}ms100;
			union Rad_MinIntegrationTime_{
				struct{
					word data_0_7	 : 8;
					word data_8_15	 : 8;
				}Fields;
				word value;
			} MinIntegrationTime;
			union Rad_MaxIntegrationTime_{
				struct{
					word data_0_7	 : 8;
					word data_8_15	 : 8;
				}Fields;
				word value;
			}MaxIntegrationTime;
			byte FocalSpot;
			union Rad_TargetDose
			{
				struct
				{
					word AECSensitivity_DoseTarget 	: 8;
					word AECDensity					: 8;
				}Fields;
				word value;
			}TargetDose;
			union Rad_IonChamber_{
				struct{
					byte LeftAECStatus		: 1;
					byte CenterAECStatus	: 1;
					byte RightAECStatus		: 1;
					byte AECNotUsed			: 4;
					byte Internal_External	: 1;
				}Fields;
				byte value;
			}IonChamber;
			byte Spare_1;
			union Rad_FPS10_{
				struct{
					word data_0_7	 : 8;
					word data_8_15	 : 8;
				}Fields;
				word value;
			}FPS10;
			byte TrakingId;
			byte Spare_2;
	}tRadDb;
	
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
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		} MinmAs1000;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		} MaxmAs1000;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		} MinmA100;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}  MaxmA100;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}  Minms100;
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}  Maxms100;
		byte     mAScale;
	}tRadExpParamRange;

	typedef struct{
		uword ExposureNumber;
		byte  ProcedureId;
		byte  ExpSeqNumber;
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
		union{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		} mAs1000;
		byte	FocalSpot;
		union Rad_TargetDose
		{
			struct
			{
				word AECSensitivity_DoseTarget 	: 8;
				word AECDensity					: 8;
			}Fields;
			word value;
		}TargetDose;
		union{
			struct{
				byte LeftAECStatus		: 1;
				byte CenterAECStatus	: 1;
				byte RightAECStatus		: 1;
				byte Spare				: 4;
				byte Internal_External	: 1;
			}Fields;
			byte value;
		}IonChamber;
		byte	RadEndOfExposure;
		byte IonChamberOrientation;
	}tRadPostExpPostCondition;

	typedef struct 
	{
		byte GeneratorStatus;
		union
		{
			struct
			{
				byte Active		:4;
				byte RxDisabled	:4;				
			}Fields;
			byte value;
		}SystemMessage;
		byte ProcedureId;
		byte ProcedureStatus;
		byte ProcedureType;
		byte ExposureDatabankSeqNumber;
	    union
	    {
	        struct
	        {
	    		byte data_0_7;
	    		byte data_15_8;
	        } Fields;
	        word value;
	    } FluoroTime;
	    union
	    {
	        struct
	        {
	        	byte Alarm_5minute      	: 1;
	        	byte Warning_5minute	  	: 1;
	        	byte Spare_1			  	: 1;
	        	byte LockInFrameReached 	: 1;
	        	byte Alarm_10minute	  		: 1;
	        	byte Warning_10minute	  	: 1;
	        	byte Spare_2			  	: 1;
	        	byte HCF_ON			  		: 1;
	        } Fields;
	        byte value;
	    } FluoroStatus;
		byte AccumulatedAnodeHU;
		byte AccumultatedhousingHU;
		byte AccumulatedGenHU;
		union
		{
			struct
			{
				byte PrepPedalStatus		  :1;
				byte ExpPedalStatus			  :1;
				byte FlPedalStatus			  :1;
				byte Spare					  :4;
				byte FilStatus				  :1;
			} Fields;
			byte value;
		}ExposureSwitches;
	    union
	    {
	        struct
	        {
	        	byte Mode_1   : 1;
	        	byte Mode_2	  : 1;
	        	byte Spare	  : 6;
	        } Fields;
	        byte value;
	    }DynModeStatus;
		byte CurrentRotorSpeed;
	}tGeneratorStatus;
	
	typedef enum
	{
		Aec_Rotation_Unknown	= 0,
		Aec_Rotation_0Degree,
		Aec_Rotation_90Degree,
		Aec_Rotation_180Degree,
		Aec_Rotation_270Degree
	}tAecRotation;
	
	typedef struct
	{
		word PreparationStatus_HARDWIRED 	: 1;
		word ExposureStatus_HARDWIRED		: 1;
		word FluoroStatus_HARDWIRED			: 1;
		word PreparationStatus_BLUETOOTH 	: 1;
		word ExposureStatus_BLUETOOTH		: 1;
		word FluoroStatus_BLUETOOTH			: 1;
		word PreparationStatus_IR 			: 1;
		word ExposureStatus_IR				: 1;
		word FluoroStatus_IR				: 1;
		word ServiceOnlyExposureDeadman 	: 1;
	}tExposureSwitchesActivation;
	
	typedef enum{
		INJECTOR_STOP = 0,
		INJECTOR_PREPARE,
		INJECTOR_INJECT
	}tInjectorAcction;
	
	typedef enum{
		INJECTOR_STOPPED = 0,
		INJECTOR_READY,
		INJECTOR_RUNNING
	}tInjectorStatus;
	
	typedef enum{
		R2CP_Procedure_Status_None = 0,
		R2CP_Procedure_Status_SingelShot,
		R2CP_Procedure_Status_SerialRAD,
		R2CP_Procedure_Status_ContinousFL,
		R2CP_Procedure_Status_ContinousFL_HLC,
		R2CP_Procedure_Status_Pulsed,
		R2CP_Procedure_Status_PulsedFL_HLC
	}tProcedureExposureType;
	
	
	const word 	MAX_KVP10_ALLOWED 		= 150 * 10;
	const word 	MIN_KVP10_ALLOWED 		= 20  * 10;
	const dword MAX_MAS1000_ALLOWED		= 32000 * 1000;
	const dword	MIN_MAS1000_ALLOWED		= 0.01 * 1000;
	const dword MAX_MA100_ALLOWED		= 1000 * 100;
	const dword MIN_MA100_ALLOWED		= 1 * 100;
	const dword MAX_MS100_ALLOWED		= 20000 * 100;
	const dword MIN_MS100_ALLOWED		= 1 * 100;
	const word  MAX_MIT_ALLOWED			= 20000;
	const word  MIN_MIT_ALLOWED			= 1;
	const word  MAX_MAT_ALLOWED			= 20000;
	const word  MIN_MAT_ALLOWED			= 1;
	const word  MAX_FPS10_ALLOWED		= 120 * 10;
	
	typedef enum
	{
		AecInternalReferenceBySensitivity = 0, 	//AEC internal reference. Sensitivity can be Low/Medium/High values (0, 1, 2)
		AecInternalReference,					//AEC internal reference. Sensitivity is calculated with a correction factor (%) that multiplies the calibrated reference value.
		AecExternalReference					//AEC external reference Generator console or imaging system calculate the AEC reference
	}tTypeAecReference;
	
	typedef struct
	{
		union MinRadDB_kV_{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}MinkV10;
		union MaxRadDB_kV_{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}MaxkV10;
		union MinRadDB_mAs1000_{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}MinmAs1000;
		union MaxRadDB_mAs1000_{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}MaxmAs1000;
		union MinRadDB_mA100_{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}MinmA100;
		dword MinmA100SmallFocus;
		dword MinmA100LargeFocus;
		union MaxRadDB_mA100_{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}MaxmA100;
		dword MaxmA100SmallFocus;
		dword MaxmA100LargeFocus;
		union MinRadDB_ms100_{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}Minms100;
		union MaxRadDB_ms100_{
			struct{
				dword data_0_7	 : 8;
				dword data_8_15	 : 8;
				dword data_16_23 : 8;
				dword data_24_31 : 8;
			}Fields;
			dword value;
		}Maxms100;
		union MinRadDB_PPS10_{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}MinPPS10;
		union MaxRadDB_PPS10_{
			struct{
				word data_0_7	 : 8;
				word data_8_15	 : 8;
			}Fields;
			word value;
		}MaxPPS10;
		byte FocalSpot;
		byte DatabankId;
	}tRadParameterRanges;
	
	typedef enum
	{
		GEN_SCALE_R10	= 0,
		GEN_SCALE_R20,
		GEN_SCALE_LINEAL
	}tScale;
	
	typedef struct
	{
		word SmallFocus_mA_vs_Demand_Status : 1;
		word LargeFocus_mA_vs_Demand_Status	: 1;
		word kVpGainStatus					: 1;
		word FluoroGainStatus				: 1;
		word LowGainStatus					: 1;
		word HighGainStatus					: 1;
		word SmallFocusFilamentDemandStatus : 1;
		word LargeFocusFilamentDemandStatus : 1;
	}tCalibrationStatus;
		
	class CaDataDicRadInterface
	{
	public:
		
		enum{
			Cp_II_GEN_SS_Databank_IdOutOfRange			= 1,
			Cp_II_GEN_SS_Databank_NotDefined			= 2,
			Cp_II_GEN_SS_ProcedureIdNotDefined			= 1,
			Cp_II_GEN_SS_ProcedureIdOutOfRange			= 2,
			Cp_II_GEN_SS_IncorrectCommandType			= 10,
		};
		
		/*!
		 *	\brief Once the CaDataDicGen is instanced this method is called
		 */
		void Initialitation(void) {};

		/************************************ Generator Index ***************************************/
		enum{
			//
			Cp_II_GEN_SS_DBAE_ProcedureIdAlreadyActiveAssignNotPossible = 3,
			Cp_II_GEN_SS_DBAE_ExposureSequenceNumberOutOfRange			= 4,
			Cp_II_GEN_SS_DBAE_ExposureDbIdNotDefined					= 5,
			Cp_II_GEN_SS_DBAE_ExposureDbIdOutOfRange					= 6,
			Cp_II_GEN_SS_DBAE_ExposureDbIncompatibleWithProcedureType	= 7,
			Cp_II_GEN_SS_DBAE_ExposureSequenceNumberAlreadyAssigned		= 8,
			//Get
			Cp_II_GEN_SS_DBAE_ProcedureExposureSequenceNumberOutOfRange = 3,
			Cp_II_GEN_SS_DBAE_ProcedureNoExposureDbAssigned				= 4,
		};
	
		/*!
		 *	\brief Sets Databank Assing Exposure
		 */
		virtual byte II_Generator_SS_DBAssignExposure( byte ProcedureId , byte ExposureSeqNumber , byte ExposureDatabankId) { return Cp_MessageNotAvailable; }

		/*!
		 *	\brief Gets Databank Assing Exposure
		 */
		virtual byte II_Generator_SS_DBAssignExposure( byte ProcedureId , byte ExposureSeqNumber) { return Cp_MessageNotAvailable; }


		enum{
			Cp_II_GEN_SS_DBEA_ExposureSequenceNumberOutOfRange	= 3,
			Cp_II_GEN_SS_DBEA_NoExposureDbAssigned				= 4,
			Cp_II_GEN_SS_DBEA_ExposureDbIdNotDefined			= 5
		};

		/*!
		 * \brief	Databank Exposure Acceptance.
		 *			This message indicates if parameters on the data bank are allowed.
		 *			If not allowed, generator will respond also with a number of system messages indicating reason 
		 *			for no acceptance and system messages (warning) to instruct the user how to select correct exposure parameters.
		 */
		virtual byte II_Generator_SS_DBExposureAcceptance (byte ProcedureId , byte ExposureSeqNumber ) { return Cp_MessageNotAvailable; }

		enum{
			//p_II_GEN_SS_RDBL_PatientSizeCodeOutOfRange		= 3,
			//Cp_II_GEN_SS_RDBL_PediatricCodeOutOfRange		= 4,
			Cp_II_GEN_SS_RDBL_TechniqueModeCodeOutOfRange	= 5,
			Cp_II_GEN_SS_RDBL_TechniqueModeNotSupported		= 6,
			Cp_II_GEN_SS_RDBL_KVpValueOutOfRange			= 7,
			Cp_II_GEN_SS_RDBL_MasValueOutOfRange			= 8,
			Cp_II_GEN_SS_RDBL_MaValueOutOfRange				= 9,
			Cp_II_GEN_SS_RDBL_MsValueOutOfRange				= 10,
			Cp_II_GEN_SS_RDBL_MaximumIntegrationTimeValueOutOfRange = 11,
			Cp_II_GEN_SS_RDBL_FocalSpotCodeOutOfRange		= 12,
			Cp_II_GEN_SS_RDBL_FocalSpotNotSupported			= 13,
			Cp_II_GEN_SS_RDBL_AECSensitivityCodeOutOfRange	= 14,
			Cp_II_GEN_SS_RDBL_AECDensityCodeOutOfRange		= 15,
			//Cp_II_GEN_SS_RDBL_TubePowerLimitOutOfRange		= 17,
			Cp_II_GEN_SS_RDBL_FPSValueOutOfRange			= 18,
			Cp_II_GEN_SS_RDBL_MinimumIntegrationTimeValueOutOfRange = 19,
			Cp_II_GEN_SS_RDBL_TrackingNotSupported			= 21,
			//Cp_II_GEN_SS_RDBL_KvScanNotSupported			= 22,
			//Cp_II_GEN_SS_RDBL_MaScanNotSupported			= 23,
			Cp_II_GEN_SS_RDBL_DataBankTypeNotMatch			= 24,
			Cp_II_GEN_SS_RDBL_KvScanAndMaScanSelected		= 25
			
		};


		/*!
		 * \brief Sets Load RAD Exposure Data Bank. Used to load a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBLoad(tRadDb *pRadDB) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets Load RAD Exposure Data Bank.
		 */
		virtual byte II_Generator_SS_RDBLoad(byte Databank_Id) { return Cp_MessageNotAvailable; }
		
		enum {
			Cp_II_GEN_SS_RDBPACCEPT_ProcedureNotRad = 3
		};

		/*!
		 *	\brief	Procedure Acceptance. 
		 *			Message used to determine if the exposures defined by the RAD Data Banks assigned 
		 *			to a procedure can be made given the current generator and tube heat status.
		 *			If Procedure Acceptance is requested (GET) with Check Number of Exposures Allowed = 1, 
		 *			generator will calculate the number of exposures that can be made with the RAD Data Banks defined for the procedure. 
		 *			This is particularly useful for MultiRAD applications that requires a large number of exposures to complete an acquisition run. 
		 *			It is important to know if all the exposures needed can be made prior to starting the procedure, 
		 *			stopping the run will result in dose received by the patient and no image.
		 */
		
		virtual byte II_Generator_SS_RDBProcedureAcceptance(byte ProcedureId , byte CheckNumberOfExposures) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_GEN_SS_RDBTM_TechniqueModeCodeOutOfRange 	= 3,
			Cp_II_GEN_SS_RDBTM_TechniqueModeNotSupported	= 4,
		};

		/*!
		 * \brief Sets RAD Technique Mode in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBExpParamTechniqueMode(byte DatabankId, byte Technique_Mode, byte Auto_Tech_Mode , byte Adjust_Param_Auto , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets RAD Technique Mode in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBExpParamTechniqueMode(byte DatabankId) { return Cp_MessageNotAvailable; }

		enum{
			II_Generator_SS_RDBKV_KvpValueOutOfRange	= 3,
		};

		/*!
		 * \brief Sets kVp value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBKVP(byte DatabankId , short int kVp10orStep , byte CommandType) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets kVp value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBKVP(byte DatabankId) { return Cp_MessageNotAvailable; }


		enum{
			Cp_II_GEN_SS_RDBMAS_MasValueOutOfRange		= 3,
			CP_II_GEN_SS_RDBMAS_AutotechniqueMode_No_Selected,
		};

		/*!
		 * \brief Sets mAs value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDB_MAS(byte DatabankId , long int mAs1000orStep , byte ComandType)	{ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets mAs value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDB_MAS(byte DatabankId)	{ return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBMAMaValueOutOfRange		= 3,
		};
		/*!
		 * \brief Sets mA value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMA(byte DatabankId , long int mA100orStep, byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets mA value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMA(byte DatabankId) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBMSMsValueOutOfRange		= 3,
		};

		/*!
		 * \brief Sets ms value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMS(byte DatabankId , long int ms100orStep, byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets ms value in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMS(byte DatabankId ) { return Cp_MessageNotAvailable; }
	
		enum{
			Cp_II_Generator_SS_RDBMITMinIntegrationTimeValueOutOfRange = 3,
		};
		/*!
		 *	\brief Sets minimum integration time in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMinIntTime(byte DatabankId , word MinIntTime , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 *	\brief Gets minimum integration time in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMinIntTime(byte DatabankId) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBMATMaxIntegrationTimeValueOutOfRange	= 3,
		};
		/*!
		 *	\brief 	Sets maximum integration time in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMaxIntTime(byte DatabankId , word MaxIntTime , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 *	\brief 	Gets maximum integration time in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBMaxIntTime(byte DatabankId) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBFFocalSpotCodeOutOfRange = 3,
			Cp_II_Generator_SS_RDBFFocalSpotNotSupported = 4,
		};
		/*!
		 *	\brief 	Sets focal spot in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBFocalSpot(byte DatabankId , byte FocalSpot , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 *	\brief 	Gets focal spot in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBFocalSpot( byte DatabankId ) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBAECSSensitivityCodeOutOfRange	= 3,
		};
		/*!
		 * \brief Sets AEC Sensitivity / Target Dose in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBAECsensitivity(byte DatabankId , byte AECSensitivity_TargetDose , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets AEC Sensitivity / Target Dose in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBAECsensitivity(byte DatabankId) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBAECDDensityValueOutOfRange	= 3,
		};
		/*!
		 * \brief	Sets AEC Density in a RAD Data Bank. 
		 *			Generator uses calibrated value for no density correction (N). 
		 *			Correction factor is applied according to the following formula:
		 *			
		 *			AEC DAC Reference = (Calibrated Value * Correction Factor) / 100
		 */
		virtual byte II_Generator_SS_RDBAECdensity(byte DatabankId , byte AECDensity , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief	Gets AEC Density in a RAD Data Bank. 
		 */
		virtual byte II_Generator_SS_RDBAECdensity( byte DatabankId ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Sets AEC reference in a RAD Data Bank
		 */
		virtual byte II_Generator_SS_RDBAECReference( byte DatabankId , word AECReference ) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets Aec External Reference
		 */
		virtual byte II_Generator_SS_RDBAECReference( byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_Generator_SS_RDBAECCMisMatch				= 3,
		};

		/*!
		 * \brief Gets AEC Chambers selection in a RAD Data Bank
		 */
		virtual byte II_Generator_SS_RDBAECchambers( byte DatabankId , bool LeftStatus , bool CenterStatus , bool RigthStatus ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Sets AEC Chambers selection in a RAD Data Bank
		 */
		virtual byte II_Generator_SS_RDBAECchambers(byte DatabankId) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBTTurboPowerLimitOutOfRange 	= 3,
		};

		/*!
		 * \brief Sets Tube Power Limit percentage in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBTubePowerLimit(byte DatabankId , byte TubePowerLimit , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets Tube Power Limit percentage in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBTubePowerLimit(byte DatabankId) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_Generator_SS_RDBFPSFpsValueOutOfRange		= 3,
		};
		/*!
		 * \brief Sets FPS in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBFPS(byte DatabankId , word FPS10 , byte CommandType ) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets FPS in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBFPS(byte DatabankId ) { return Cp_MessageNotAvailable; }
		
		enum{
			Cp_II_Generator_SS_RDBPAPatientSizeOutOfRange	= 3,
			Cp_II_Generator_SS_RDBPAPedriaticsSelOutOfRange = 4,
		};
		/*!
		 * \brief Sets Patient Size and Pediatric study in a Rad Data bank
		 */
		virtual byte II_Generator_SS_RDBPatientSize(byte DatabankId , byte PatientSize , byte Pediatric , byte CommandType) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets Patient Size and Pediatric study in a Rad Data bank
		 */
		virtual byte II_Generator_SS_RDBPatientSize(byte DatabankId ) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_RDBTrackingNotSupported	= 4,
		};
		/*!
		 *	\brief Sets Tracking ID in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBTrackingId(byte DatabankId , bool TrankingIdEnable , byte CommandType) { return Cp_MessageNotAvailable; }
		
		/*!
		 *	\brief Gets Tracking ID in a RAD Data Bank.
		 */
		virtual byte II_Generator_SS_RDBTrackingId( byte DatabankId ) { return Cp_MessageNotAvailable; }


		/*************************************** CURRENT RAD EXPOSURE DATA BANK ***************************************/

		enum {
			Cp_II_Generator_SS_RDBCURRIncorrectType = 1
		};

		/*!
		 * \brief Current RAD Exposure Data Bank
		 */
		virtual byte II_Generator_SS_RDBCurrentRadDataBank(void) { return Cp_MessageNotAvailable; }

		/*!
		 * \brief Current RAD Data Bank Technique Mode.
		 */
		virtual byte II_Generator_SS_RDBCurrentTechniqueMode(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank kVp value.
		*/
		virtual byte II_Generator_SS_RDBCurrentkVp(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank mAs value.
		*/
		virtual byte II_Generator_SS_RDBCurrentmAs(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank mA value.
		*/
		virtual byte II_Generator_SS_RDBCurrentmA(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank ms value.
		*/
		virtual byte II_Generator_SS_RDBCurrentms(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank MinIntegrationTime value.
		*/
		virtual byte II_Generator_SS_RDBCurrentMinIntegrationTime(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank MinIntegrationTime value.
		*/
		virtual byte II_Generator_SS_RDBCurrentMaxIntegrationTime(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank FocalSpot value.
		*/
		virtual byte II_Generator_SS_RDBCurrentFocalSpot(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank AEC Sensitivity / Target Dose value.
		*/
		virtual byte II_Generator_SS_RDBCurrentAECSensitivityTargeDose(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank AEC Density correction factor.
		*/
		virtual byte II_Generator_SS_RDBCurrentAECDensity(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank AEC Chambers selection..
		*/
		virtual byte II_Generator_SS_RDBCurrentAECChambers(void){ return Cp_MessageNotAvailable; }
	   /*!
		* \brief Current RAD Data Bank FPS.
		*/
		virtual byte II_Generator_SS_RDBCurrentFPS(void){ return Cp_MessageNotAvailable; }

	   /*!
		* \brief Current RAD Data Bank Tracking Id.
		*/
		virtual byte II_Generator_SS_RDBCurrentTrackingId(void){ return Cp_MessageNotAvailable; }

		/************************************* OTHER GENERATOR FUNCTIONS ***************************/

		/*!
		 * \brief Resets fluoro time.
		 */
		virtual byte II_Generator_SS_FDBTimeReset(void){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Resets 5-minute fluoro alarm.
		 */
		virtual byte II_Generator_SS_FDB5MinuteFluoroAlarmReset(void){ return Cp_MessageNotAvailable; }

		enum{
	       Cp_II_Generator_SS_GenPowerLimitValueOutOfRange  = 1,
		};
		/*!
		 * \brief Sets Generator Power Limit.
		 */
		virtual byte II_Generator_SS_GenPowerLimit(signed char GenPowerLimitPercentageOrStep , byte CommandType ){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Gets Generator Power Limit.
		 */
		virtual byte II_Generator_SS_GenPowerLimit( void ){ return Cp_MessageNotAvailable; }

		/*!
		 * \brief Sets disabled/enabled filaments
		 */
		virtual byte II_Generator_SS_FilamentEnabled(bool status) { return Cp_MessageNotAvailable; }
	
		/*!
		 * \brief Gets Filament state
		 */
		virtual byte II_Generator_SS_FilamentEnabled(void) { return Cp_MessageNotAvailable; }
		
		enum{
	       Cp_II_Generator_SS_TubePowerLimitValueOutOfRange  = 1,
	       Cp_II_Generator_SS_TubePowerLimitTubeOutOfRange,
	       CP_II_Generator_SS_TubePowerLimitTubeNotSupported,
		};
		
		/*!
		 * \brief Sets Tube Power Limit 
		 */
		virtual byte II_Generator_SS_TubePowerLimit( byte TubeId , signed char TubePowerLimit ,  signed char CommandType ) { return Cp_MessageNotAvailable; };
		
		/*!
		 * \brief Gets Tube Power Limit 
		 */
		virtual byte II_Generator_SS_TubePowerLimit( void ) { return Cp_MessageNotAvailable; };

		/******************************* STATUS AND EXPOSURE MANAGEMENT *********************************************/
		/*!
		 * \brief   Generator provides information about its current internal status and if a transition to a different status is in progress.
		 *			If Status=2, Status transition from=2 and Status transition to=3, 
		 *			it means that generator is preparing to make an exposure after prep signal has been pressed.
		 */
		virtual byte II_Generator_SS_Status(void) { return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_PENoexposureMade = 1,
		};

		/*!
		 * \brief Actual exposure parameters. For standard RAD applications Exposure Number = 0 always refers to last exposure.
		 */
		virtual byte II_Generator_SS_RadPostExposure(void){ return Cp_MessageNotAvailable; }

		enum {
			Cp_II_Generator_SS_SSFeatureNotSupported = 1,
			Cp_II_Generator_SS_SSRequestOutOfRange	= 2,
		};

		/*!
		 * \brief  Message to initiate or to stop an exposure. 
		 *	Exposure will start if handswitch is pressed and all conditions to start exposure are met: 
		 *	no inhibits and anode and filament ready.
		 *  param[in] 	Request => 0 Stop Exposure	 1 Start Exposure
		 */
		virtual byte II_Generator_SS_StartStop(bool Request ){ return Cp_MessageNotAvailable; }

		enum{
			Cp_II_Generator_SS_CI_Source_Node_Already_Given = 1,
		};
		
		/*!
		 * \brief	This message is used to set and manage a communications timeout on the generator.
		 *			It is used to make sure that exposures are enabled and disabled when main generator console determines, 
		 *			which is usually when a patient is open and a procedure is selected for flat panel or when a direct workstation is selected.
		 *			This message works as a keep alive message at application level.
		 *			Generator console will start sending this message at specified intervals, 
		 *			setting communications timeout with a longer time. 
		 *			Generator will send an answer message to let generator console know that communications are ok.
		 *			If this message is not received by the generator within the timeout defined, 
		 *			exposures will be disabled and it will send answer with Link status = Connection lost with the same timeout interval.
		 *			When communications are reestablished, generator console can start sending this message again and enable exposures if needed.
		 *param[in] CommTimeout => 0 Not Active 1-255 Active, Timeout in seconds
		 */
		virtual  byte  II_Generator_SS_CommunicationsInhibitTimeout( byte CommTimeout , byte SourceNodeId ) { return Cp_MessageNotAvailable; }
	 
		 /*!
		  *	\brief Gets Current Status of Communications Inhibit Timeout
		  */
		 virtual byte  II_Generator_SS_CommunicationsInhibitTimeout( void ) { return Cp_MessageNotAvailable; };

		 /*!
		  * \brief Resets exposure counter, typically when a patient is open.
		  */
		 virtual  byte  II_Generator_SS_ResetExposureCounter( void ) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  * \brief Generator provides information about the status of exposure switches and where they are activated. 
		  */
		 virtual byte II_Generator_SS_ExposureSwitchesActivationDevice( void ) { return Cp_MessageNotAvailable; }
		 
		 /***************************************************  MISCELLANEOUS ****************************************/
		 /*!
		  *	\brief Information about the maximum number of procedures available.
		  */
		 virtual byte II_Generator_SS_MaxProcedure(void) { return Cp_MessageNotAvailable; }

		 /*!
		  *\brief Information about the maximum number of data banks available.
		  */
		 virtual byte II_Generator_SS_MaxDatabank(void) { return Cp_MessageNotAvailable; }

		 /*!
		  *	\brief This message contains the information about the RAD exposure parameter ranges
		  */
		 virtual byte II_Generator_SS_RadExpParamRanges(void) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  *	\brief 	This message contains the information required by a generator to configure a workstation. 
		  *			It is available for generators that do not support XML configuration files.
		  */
		 virtual byte II_Generator_SS_Workstation(byte WSId, byte GenId , byte TubeId , byte PosId , byte SynchInterfaceType , byte ImageReceptorIndex , byte IOChamberConnTo , byte AECInput) { return Cp_MessageNotAvailable; } 

		 enum {
			Cp_Generator_SS_HFSwitchIDoutofrange = 1,
		 };
		 /*!
		  *	\brief This message contains the information required by a generator to configure handswitches/footswitches. 
		  *	It is available for generators that do not support XML configuration files.
		  */
		 virtual byte II_Generator_SS_HorFSwitch(	byte HFId , bool PrepRad , bool ExpRad , bool Fl , 
													bool DynMode1Mask , bool DynMode2Mask , bool DynMode3Mask , bool DynMode4Mask,
													bool DynMode1Status , bool DynMode2Status , bool DynMode3Status, bool DynMode4Status) { return Cp_MessageNotAvailable; }
		 /*!
		  *	\brief This message contains the information required by a generator to know what software version is running in Ebox Device when exists.
		  */
		 virtual byte II_Generator_SS_EboxVersion(byte Version,byte Revision,byte SubRevision) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  * \brief This message configures the way the AEC is handled by the generator. 
		  * \param[ in ] Sensitivity correction factor 
		  * \param[ in ] External, AEC reference in counts comes from an external device
		  */
		 virtual byte II_Generator_SS_AecReferenceMode( tTypeAecReference AecReference  ){ return Cp_MessageNotAvailable; }
		 
		 /*!
		  * \brief This message configures the way the AEC is handled by the generator. 
		  */
		 virtual byte II_Generator_SS_AecReferenceMode( void ) { return Cp_MessageNotAvailable; }
		 
		 enum{
			 CP_Generator_SS_ExposureCounters_TubeNotSupported = 1,
			 Cp_Generator_SS_ExposureCounters_TubeOutOfRange,
			 Cp_Generator_SS_ExposureCounters_CommandOutOfRange
		 };
		 /*!
		  * \brief Exposure Counters
		  * \param [ in ] TubeId
		  * \param [ in ] Command:  0 Rad Exposures
		  * 						1 RAD energy (joules)
		  * 						2 RAD mAs
		  * 						3 Filament excited Time (seconds)
		  * 						4 Fluoro time (seconds)
		  * 						5 Fluoro energy (joules)
		  */
		 virtual byte II_Generator_SS_CountersData( byte TubeId ,  byte CommandId ) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  * This message is used to request the value of the digital inputs (GET), 
		  * to receive the response and also to receive an event when digital input is configured to inform about transitions.
		  */
		 virtual byte II_Generator_SS_DigitalInputsOutputs( word DigitalInputsMask ) { return Cp_MessageNotAvailable; }
		 
		 enum {
			 CP_Generator_SS_LockInTimeOutOfRange	= 10 
		 };
		 /*!
		  * \brief 	Message to read (GET) or write (SET) parameters for a Lock-In Data Bank.
		  * 		Auto Lock-in Enabled is related to ABC in Roadmapping fluoro and to AEC in DSA. It can also be uses for non image subtraction procedures.
		  *			For Roadmapping fluoro lock-in is reached when lock-in frame is met or when ABC regulation is done if this occurs before lock-in frame.
		  * 		If roadmapping fluoro is performed with continuous fluoroscopy, frame number is defined for 30 PPS.
		  */
		 virtual byte II_Generator_SS_LockInDatabank( byte LockInDb , bool AutoLockInEnabled , byte AutoLockInTime_Sec10 ) { return Cp_MessageNotAvailable; };
		 
		 /*!
		  * \brief Get LockIn Databank configuration
		  */
		 virtual byte II_Generator_SS_LockInDatabank( byte LockInDb ){ return Cp_MessageNotAvailable; }
		 
		 enum{
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ProcedureIdNotDefined = 1,
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ProcedureIdOutOfRange,
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ProcedureIdAlreadyActive,
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ImageSubtractionDbNumberOutOfRange,
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ImageSubtractionBankIdNotDefined,
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ImageSubtractionDataBankIdOutOfRange,
			 Cp_Generator_SS_LockInAssignToProcedureProcedure_ImageSubractionDbIncompatiblewithProcedureType
		 };
		 /*!
		  * \brief Set LockIn Assign To Procedure
		  */
		 virtual byte II_Generator_SS_LockInAssignToProcedure( byte ProcedureId , byte LockInDb ){ return Cp_MessageNotAvailable; }
		 
		 /*!
		  * \brief Get LockIn Assign To Procedure
		  */
		 virtual byte II_Generator_SS_LockInAssignToProcedure( byte ProcedureId ){ return Cp_MessageNotAvailable; }
		 
		 enum{
			 Cp_Generator_SS_LockInFrameReached_ImageSubtractionDbNotloaded	= 101,
			 Cp_Generator_SS_LockInFrameReached_NoDSAOrRoadmappingProcedureDefined, 
		 };
		 /*!
		  * \brief Get Message sent by the generator when the generator reaches lock-in frame defined by the procedure.
		  */
		 virtual byte II_Generator_SS_LockInFrameReached( byte LockInDb ){ return Cp_MessageNotAvailable; }
		 
		 /*
		  * \brief Imaging SW provides information about its current internal status.
		  */
		 virtual byte II_Generator_SS_Injector( tInjectorAcction InjectorStatus ) { return Cp_MessageNotAvailable; }
		 
		 /*
		  * \brief Imaging SW provides information about its current internal status.
		  */
		 virtual byte II_Generator_SS_Injector( void ){ return Cp_MessageNotAvailable; }
		 
		 /*!
		  * Sets RAD exposure parameter ranges to be assigned to a procedure.
		  */
		 virtual byte II_Generator_SS_RadProcedureParameterRanges( tRadParameterRanges *pRadParameterRanges ) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  * Gets RAD exposure parameter ranges to be assigned to a procedure.
		  */
		 virtual byte II_Generator_SS_RadProcedureParameterRanges( byte DatabankId ) { return Cp_MessageNotAvailable; } 
		 
		 /*!
		  * Command used to assign a Dynamic Sequence Data Bank to a Procedure. This set of ranges supersedes the default ones.
		  */
		 virtual byte II_Generator_SS_AssignRadProcedureParameterRanges( byte ProcedureId ,  byte DatabankId ) { return Cp_MessageNotAvailable; }

		 /*!
		  * Command used to assign a Dynamic Sequence Data Bank to a Procedure. This set of ranges supersedes the default ones.
		  */
		 virtual byte II_Generator_SS_AssignRadProcedureParameterRanges( byte ProcedureId ) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  * This message contains the information about the RAD exposure parameter scales
		  */
		 virtual byte II_Generator_SS_RadParameterScales( void ) { return Cp_MessageNotAvailable; }
		 
		 /*!
		  * This message contains the information of current calibration status
		  */
		 virtual byte II_Generator_SS_CalibrationStatus(void) { return Cp_MessageNotAvailable; }
		 //////////////////////////////////// ION CHAMBER ROTATION ////////////////////////////////////////////////////
			
		/*!
		 * \brief Positioner Ion Chamber Rotation Event
		 */
		virtual void II_Positioner_SS_IonChamberRotation( tAecRotation AecRotation ){}
	};//class
};//namesapce
#endif
