#ifndef _CADATADICSERVICEINTERFACE_H__
	#define _CADATADICSERVICEINTERFACE_H__

#include "aDataDicInterface.h"

namespace R2CP
{

	typedef struct{
		byte 	PowerKW;
		byte 	MaxKvp;
		byte 	MinKvp;
		byte 	Max_mA_data_0_7;
		byte	Max_mA_data_8_15;
		byte 	Min_mA_data_0_7;
		byte	Min_mA_data_8_15;
		byte	Max_mAs1000_data_0_7;
		byte	Max_mAs1000_data_8_15;
		byte	Max_mAs1000_data_16_23;
		byte 	Max_mAs1000_data_24_31;
		byte	Min_mAs1000_data_0_7;
		byte	Min_mAs1000_data_8_15;
		byte	Min_mAs1000_data_16_23;
		byte 	Min_mAs1000_data_24_31;
		byte 	Max_ms_data_0_7;
		byte 	Max_ms_data_8_15;
		byte 	Min_ms_data_0_7;
		byte 	Min_ms_data_8_15;
		byte	Max_PPS;
		bool	High_SpeedEnabled;
		bool	AEC_Enabled;
		bool	Tomography_Enabled;
		bool	DualEnergy_Enabled;
		bool	R20Scale_Enabled;
		bool	RF_Enabled;
		bool	TrackingFormula0P_Enabled;
		bool	FluoroCurves_Enabled;
		bool	DSI_Enabled;
		bool	DigitalEnd_Enabled;
		bool	DSA_Enabled;
		bool	Cine_Enabled;
		bool	HCF_Enabled;
		bool	Scan_Enabled;
		byte	PowerSupply;
		bool	BatteriesPSU_Enabled;
		bool	BatteriesStationary_Enabled;
		bool	Capacitors_Enabled;
		bool	StarterBooster_Enabled;
		byte	ACFilter;
		byte	HighDutyCycle;
		byte 	Mamography;
		byte	InterfaceOptions_data_0_7;
		byte 	InterfaceOptions_data_8_15;
		bool	StarterEnabled;
		byte	Spare46;
		byte	Spare47;
		byte	Spare48;
		bool	Pikachu_Bucky_Enabled;
		bool	Pikachy_SerialPort_Enabled;
		bool	DemoMode_Enabled;
		byte	StarterType;
		byte	Spare53;
		byte	Spare54;
		byte	Spare55;
		byte	Spare56;
		byte	Spare57;
		byte	Spare58;
		byte 	Spare59;
		byte	Spare60;
		byte	Spare61;
		byte	Spare62;
		byte	Spare63;
		byte	Spare64;
	}tStandardLicense;
		                
	typedef struct{
		byte 	System;
		byte 	Generator;
		byte	Client;
		byte	ImageSystem;
		byte	StartupMode;
		bool	Stitching_Enabled;
		bool	Image_Preview_Enabled;
		bool	Tomosynthesis_Enabled;
		byte	DAP;
		bool	AutoCollimation_Enabled;
		bool	DigitalInterface_Enabled;
		bool	DetectorAligmentAssistance_Enabled;
		bool	RemoteExposureControl_Enabled;
		byte 	FluoromAStation;
		byte	isCyphered;
		byte	ServiceModeProtected;
		byte	Spare17;
		byte	Spare18;
		byte	Spare19;
		byte	Spare20;
		byte	Spare21;
		byte	Spare22;
		byte	Spare23;
		byte	Spare24;
		byte	Spare25;
		byte	Spare26;
		byte	Spare27;
		byte 	Spare28;
		byte	Spare29;
		byte	Spare30;
		byte	Spare31;
		byte	Spare32;
		byte	Spare33;
		byte	Spare34;
		byte	Spare35;
		byte	Spare36;
		byte	Spare37;
		byte 	Spare38;
		byte	Spare39;
		byte	Spare40;
		byte	Spare41;
		byte	Spare42;
		byte	Spare43;
		byte	Spare44;
		byte	Spare45;
		byte	Spare46;
		byte	Spare47;
		byte 	Spare48;
		byte	Spare49;
		byte	Spare50;
		byte	Spare51;
		byte	Spare52;
		byte	Spare53;
		byte	Spare54;
		byte	Spare55;
		byte	Spare56;
		byte	Spare57;
		byte	Spare58;
		byte 	Spare59;
		byte	LicenseRevision;
		byte	Spare61;
		byte	Spare62;
		byte	Spare63;
		byte	LicenseVersion;
	}tExtendedLicense;
	
	typedef struct{
		tStandardLicense StandardLicense;
		tExtendedLicense ExtendedLicense;
	}tR2CPLicense;
	
	typedef struct{
		byte 				SourceNode;
		byte 				ErrorCategory;
		unsigned long long 	Date;
		char 				Info[255];
	}tServiceLog;
	
	class CaDataDicServiceInterface
	{
		
	public:
		
		/*!
		 * \brief Class Destructor
		 */
		virtual ~CaDataDicServiceInterface(){}
		
		/*
		 * \brief	The intended use of this message is to read the license options. 
		 *			The device which needs to know the license options should ask for them to the generator sending the GET message. 
		 *			The generator will answer sending the EVENT message to the device which asked for them.
		 */
		virtual byte II_Service_SS_StandardLicense(void) { return Cp_MessageNotAvailable; }
		
		/*!
		 * \brief Gets the Extended License
		 */
		virtual byte II_Service_SS_ExtendedLicense(void) { return Cp_MessageNotAvailable; }
	};
};

#endif
