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

    // __________________________________________ GENERATORE STATUS
    typedef enum{
       Stat_Initialization = 1,
       Stat_Standby,
       Stat_Preparation,
       Stat_Ready,
       Stat_ExpReq,
       Stat_ExpInProgress,
       Stat_WaitFootRelease,
       Stat_GoigToShutdown,
       Stat_Error,
       Stat_Service
    } tGenStatus_Stat;

    typedef enum{
        Stat_SystemMessageActive_NotActive = 0,
        Stat_SystemMessageActive_Active,
    }tGenStatus_SystemMessageActive;

    typedef enum{
        Stat_SystemMessageInhibit_NotActive = 0,
        Stat_SystemMessageInhibit_Active,
    }tGenStatus_SystemMessageInhibit;

    typedef enum{
        Stat_ProcedureStat_NotActive = 0,
        Stat_ProcedureStat_Active,
        Stat_ProcedureStat_Paused,
        Stat_ProcedureStat_Finished
    }tGenStatus_ProcedureStat;

    typedef enum{
        tGenStatus_ExposureType_None=0,
        tGenStatus_ExposureType_SingleShot,
        tGenStatus_ExposureType_SerialRad,
        tGenStatus_ExposureType_ContFl,
        tGenStatus_ExposureType_ContFl_HLC,
        tGenStatus_ExposureType_PulsedFl,
        tGenStatus_ExposureType_PulsedFl_HLC
    }tGenStatus_ExposureType;

    typedef struct
    {
        byte GeneratorStatus;
        union
        {
            struct
            {
                byte Active		:4;
                byte Inhibit	:4;
            }Fields;
            byte value;
        }SystemMessage;
        byte ProcedureId;
        byte ExposureType;
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
                byte PrepSignalStatus		  :1;
                byte ExpsignalStatus          :1;
                byte FlSignalStatus			  :1;
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
    }tGeneratorStatusV6;

    typedef struct
    {
        byte GeneratorStatus;
        union
        {
            struct
            {
                byte Active		:4;
                byte Inhibit	:4;
            }Fields;
            byte value;
        }SystemMessage;
        byte ProcedureId;
        byte ProcedureStatus;
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
                byte PrepSignalStatus		  :1;
                byte ExpsignalStatus          :1;
                byte FlSignalStatus			  :1;
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
    }tGeneratorStatusV5;

    // __________________________________________ / GENERATOR STATUS
    //___________________________________________ PROCEDURE DEFINITION
    typedef enum{
        ProcId_Standard_Mammography_2D=1,
        ProcId_Aec_Mammography_2D,
        ProcId_Standard_Mammography_3D,
        ProcId_Aec_Mammography_3D,
        ProcId_Last
    }tProcedureId;

    typedef enum{
        ProcType_Standard_Mammography_2D=50,
        ProcType_Aec_Mammography_2D=51,
        ProcType_Standard_Mammography_3D=10,
        ProcType_Aec_Mammography_3D=52,
    }tProcedureType;


    typedef enum{
        Proc_SoftwareActivation =0,
        Proc_HandswitchActivation,
    }tProcedureActivation;

    typedef enum{
        Proc_DatabankSeq_ND =0,
        Proc_DatabankSeq_Next,
        Proc_DatabankSeq_Software
    }tProcedureDatabankSeq;

    typedef struct
    {
        byte ProcedureId;
        byte GeneratorProcedureType;
        byte Handswitch;
        byte ActivationMode;
        byte WorkstationId;
        byte numDatabank;
        byte databankSeqMode;
        byte numberOfExposure;
        byte initialized;
    }tProcedureDefinition;
    // __________________________________________ / PROCEDURE DEFINITION

    // __________________________________________ DATABANK DEFINITION
    typedef enum{
        DB_Pre = 1,
        DB_Pulse,
        DB_LastId
    }tProcedureDbId;

    typedef enum{
        DB_Focus_Small = 0,
        DB_Focus_Large = 1,
    }tProcedureDbFocus;

    typedef enum{
        DB_Tech_0= 0,
        DB_Tech_1,
        DB_Tech_2,
        DB_Tech_3,
        DB_Tech_2_FallingLoad,
        DB_Tech_FullAuto,
        DB_Tech_2_max_ms,
    }tDbTech;

    typedef enum{
        DB_Tech_AutoMode_NotModify= 0,
        DB_Tech_AutoMode_Modify = 0x40,
    }tDbTechAutoMode;

    typedef enum{
        DB_Tech_AAdjustParam_NotModify= 0,
        DB_Tech_AdjustParam_Modify = 0x80,
    }tDbAdjustParam;



    // __________________________________________ / DATABANL DEFINITION



	class CaDataDicRadInterface
	{
	public:
        CaDataDicRadInterface(void){
            Initialitation();
        }


        /*!
         *	\brief Once the CaDataDicGen is instanced this method is called
         */
        void Initialitation(void) {
            for(int i=1; i< ProcId_Last; i++){
                procedureDefinitions[i-1].initialized = 0;
            }

            for(int i=1; i< DB_LastId; i++){
                DbDefinitions[i-1].ImagingSystemProtocolId = 0;
            }

        };


        void MET_Generator_Set_StatusV5(byte* data) { generatorStatusV5 =  *(tGeneratorStatusV5*) data; }
        void MET_Generator_Set_StatusV6(byte* data) { generatorStatusV6 =  *(tGeneratorStatusV6*) data; }

        byte MET_Generator_Update_ProcedureV5(byte* data) {
            if(data ==nullptr) return 0;

            byte id = data[0];
            if( id >= ProcId_Last) return 0;
            if( id < 1) return 0;
            tProcedureDefinition* tProc = &procedureDefinitions[id-1];

            tProc->ProcedureId = data[0];
            tProc->GeneratorProcedureType = data[1];
            tProc->Handswitch = data[2];
            tProc->ActivationMode = data[3];
            tProc->WorkstationId = data[4];
            tProc->numDatabank = data[5];
            tProc->databankSeqMode = data[6];
            tProc->numberOfExposure = 0;
            tProc->initialized = 1;
            return id;
        }

        byte MET_Generator_Update_ProcedureV6(byte* data) {
            if(data ==nullptr) return 0;

            byte id = data[0];
            if( id >= ProcId_Last) return 0;
            if( id < 1) return 0;
            tProcedureDefinition* tProc = &procedureDefinitions[id-1];

            tProc->ProcedureId = data[0];
            tProc->GeneratorProcedureType = data[1];
            tProc->Handswitch = data[2];
            tProc->ActivationMode = data[3];
            tProc->WorkstationId = data[4];
            tProc->numDatabank = data[5];
            tProc->databankSeqMode = data[6];
            tProc->numberOfExposure = data[8] + 256 * data[7];
            tProc->initialized = 1;
            return id;
        }

        public:
         tGeneratorStatusV5 generatorStatusV5;
         tGeneratorStatusV6 generatorStatusV6;
         tProcedureDefinition procedureDefinitions[ProcId_Last];
         bool isProcInitialized(byte i){return ( i >= ProcId_Last) ? false : procedureDefinitions[i-1].initialized;}

         tRadDb DbDefinitions[DB_LastId];
         bool isDbInitialized(byte i){return ( i >= DB_LastId) ? false : ( (DbDefinitions[i].ImagingSystemProtocolId == DB_LastId) ? true : false );}

	};//class
};//namesapce
#endif
