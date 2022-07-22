/*
 * R2CP_Index.h
 *
 *  Created on: Mar 4, 2016
 *      Author: JuanMiguel.Andujar
 */

#ifndef R2CP_INDEX_H_
#define R2CP_INDEX_H_

// INDEXES / SUBINDEXES

//R2CP
#define R2CP_COMMANDS_ENTRY/*---------------------------------------------*/0x00

	#define R2CP_HW_VERSION													0
	#define R2CP_SW_VERSION													1
	#define R2CP_STATUS														2
	#define R2CP_RESET														3		 
	#define R2CP_ERROR_CODE													4
	#define R2CP_MASTER_LIFE_TIME_OUT										5
	#define R2CP_PROTOCOL_VERSION											6
	#define R2CP_BOOT_VERSION												7
	#define R2CP_PACKAGE_VERSION											8
	#define R2CP_PIPE_CONFIG												9
	#define R2CP_PIPE														10
	#define R2CP_SERIAL_NUMBER												11
	#define R2CP_DESCRIPTION												12
	#define R2CP_NODE_ID_CONFIG												13
	#define R2CP_INTERLOCK													14
	#define R2CP_WARNING													15
	#define R2CP_COMPILATION_DATE											16
	#define R2CP_SYSTEM_VERSION												17

#define ANALOG_INPUTS_ENTRY/*---------------------------------------------*/1

	#define AI_TYPE															0
	#define AI_VALUE														1
	#define AI_CONFIGURATION												2
	#define AI_EVENT_PERIOD													3
	#define AI_SAMPLING_PERIOD												4
	#define AI_THRESHOLD													5
	#define AI_FILTER_FREQUENCY												6
	#define AI_RESET														7
	#define AI_FRAME_CFG													8
	#define AI_FRAME0														9
	#define AI_FRAME1														10
	#define AI_FRAME2														11
	#define AI_FRAME3														12

//Entries for DIGITAL_INPUTS
#define DIGITAL_INPUTS_ENTRY/*--------------------------------------------*/3

	#define DI_TYPE															0
	#define DI_VALUE														1
	#define DI_CONFIGURATION												2
	#define EVENT_PERIOD													3
	#define SAMPLING_PERIOD													4
	#define DEBOUNCE_ACTIVATION												5
	#define DEBOUNCE_DEACTIVATION											6
	#define TRIGGER_TYPE													7
	#define DEADMAN															8
	#define DI_RESET														9

//Types shared for DIGITAL_INPUTS
typedef enum{
	DIGITALINPUTS_TRIGGERTYPE_NONE,
	DIGITALINPUTS_TRIGGERTYPE_FALLING,
	DIGITALINPUTS_TRIGGERTYPE_RISING,
	DIGITALINPUTS_TRIGGERTYPE_BOTH
}eDigitalInputsTriggerType;

//Entries for DIGITAL_OUTPUTS
#define DIGITAL_OUTPUTS_ENTRY/*-------------------------------------------*/4

	#define DO_TYPE															0
	#define DO_DEFAULT_VALUE												1
	#define DO_VALUE														2
	#define DO_TOGGLE														3
	#define DO_CONFIGURATION												4
	#define	DO_PWM_MODE														5
	#define DO_WORKING_LENGTH												6
	#define DO_RESET														7
	#define DO_PWM_RAMP_MODE												8
	
#define COLLIMATOR_COMMANDS_ENTRY/*---------------------------------------*/6

	#define COLLIMATOR_TYPE													0
	#define COLLIMATOR_STATE												1
	#define COLLIMATOR_LIGHT												2
	#define COLLIMATOR_WORKING_MODE											3
	#define COLLIMATOR_APERTURES_FEEDBACK									4
	#define COLLIMATOR_MAX_APERTURES										5 
	#define COLLIMATOR_APERTURES											6
	#define COLLIMATOR_FILTER												7
	#define COLLIMATOR_SID													8
	 
#define DOSIMETER_COMMANDS_ENTRY/*----------------------------------------*/7

	#define DOSIMETER_TYPE													0
	#define DOSIMETER_RESET													1
	#define DOSIMETER_END_EXPOSURE											2
	#define DOSIMETER_PATIENT_DISTANCE										3
	#define DOSIMETER_MEASURES_FEEDBACK_PERIOD								4
	#define DOSIMETER_MEASURES												5
	#define DOSIMETER_MODE													6
	#define DOSIMETER_EXPOSURE_START										7
	#define DOSIMETER_FOCAL_DISTANCE										8
	#define DOSIMETER_DEVICE_NAME											9
	#define DOSIMETER_HW_VERSION											10
	#define DOSIMETER_SW_VERSION											11
	
#define HDLDEVICE_COMMANDS_ENTRY/*-----------------------------------------*/10

	#define HDLDEVICE_FPGAVERSION											0
	#define HDLDEVICE_FPGARESET												1
	#define HDLDEVICE_FPGADNA												2
	#define HDLDEVICE_FPGACONNECTIONS										3
	#define HDLDEVICE_UPDATE_STATUS											4
	#define HDLDEVICE_UPDATE_LENGHT											5
	#define HDLDEVICE_UPDATE_ERASE_SECTOR									6
	#define HDLDEVICE_UPDATE_PROGRAMM_PAGE									7
	#define HDLDEVICE_UPDATE_CHECKSUM										8
	#define HDLDEVICE_GOLDEN_FPGA_VERSION									9
	#define HDLDEVICE_NEW_FPGAVERSION										10
	#define HDLDEVICE_NEW_GOLDEN_FPGA_VERSION								11

#define MISCELLANEOUS_DEVICE_COMMANDS_ENTRY/*-----------------------------*/12								
	#define DEVICE_ENABLE													0
	
#define BATTERYMONITOR_COMMANDS_ENTRY/*------------------------------------*/18
	//CAN
	
	#define BATTERYMONITOR_INFO_SR0											0	
	#define BATTERYMONITOR_INFO_SR1											1
	#define BATTERYMONITOR_INFO_SR2											2
	#define BATTERYMONITOR_INFO_SR3											3
	#define BATTERYMONITOR_INFO_SR4											4
	#define BATTERYMONITOR_INFO_SR5											5
	#define BATTERYMONITOR_BMS_SOH											7
	#define BATTERYMONITOR_BMS_SOC											8
	#define BATTERYMONITOR_CONFIG_DATA										32
	#define BATTERYMONITOR_CONFIG_CHK										33
    //ETH
	#define	XRAY_BATTERY_LEVEL												1
	#define MOVEMENT_BATTERY_LEVEL											2
	#define XRAY_BATTERY_CHARGE 											3
	#define MOVEMENT_BATTERY_CHARGE 										4



//Entries for RFID
#define RFID_SUBINDEX_COMMANDS_ENTRY/*-------------------------------------*/0x0B
typedef enum {
	RFID_SYSTEM_TURN_ON_METHOD		= 0x00,
	RFID_CONFIG_ID_MODE,
	RFID_CONFIG_QUALIFICATION_MODE,
	RFID_STATUS,
	RFID_FIELD,
	RFID_TAG,
	RFID_QUALIFY_TAG,
	RFID_DISQUALIFY_TAG,
	RFID_LIST_MAX_CAPACITY,
	RFID_LIST_TAGS_NUMBER,
	RFID_LIST_TAG,
	RFID_LIST_CLEAR_TAG_INDEX,
	RFID_LIST_CLEAR_TAG,
	RFID_LIST_CLEAR,
	RFID_EEPROM_CLEAR,
	RFID_CAN_SUBINDEX_CONFIG,
	SPEAKER_VOLUME			= 0x20,
	CONFIG_SPEAKER_VOLUME,
	SPEAKER_EXPOSURE_SOUND_ON,
	SPEAKER_EXPOSURE_SOUND_OFF,
	SPEAKER_GENERATE_SOUND_ON,
	SPEAKER_GENERATE_SOUND_OFF,
	SPEAKER_MUTE,
	STATUS_LED_COLOR,
	BATTERY_LEVEL_INDICATOR
}eRFID_Subindex;

//Entries for UARC
#define UARC_COMMANDS_ENTRY/*---------------------------------------------*/0x13

typedef enum {
	UARC_MOV_ORDER 								= 0x00,
	UARC_STATE									= 0x01,
	UARC_WORKING_MODE_SEL						= 0x02,
	UARC_SAVE_CONFIGURATION						= 0x04,
	UARC_SELECT_DROPOUT_STATION					= 0x06,
	UARC_DEMO_MODE								= 0x08,
	UARC_EN_TUBE_ERRORS							= 0x09,
	UARC_INTERLOCK								= 0x0A,
	UARC_POWER_OFF								= 0x0B,
	UARC_SELECT_TUBE							= 0x0C,
	UARC_TUBE1_DROPOUT_CONFIGURATION			= 0x0F,
	UARC_READY									= 0x14,
	UARC_V_DC_BUS								= 0x23,
	UARC_CONFIG_MODE							= 0x24,
	UARC_TUBE1_CONFIGURATION_STRUCTURE_CHECKSUM = 0x25,
	UARC_TUBE1_DATA_STRUCTURE_CHECKSUM 			= 0x26,
	UARC_GET_LAST_ERROR							= 0x2E,
	UARC_TUBE1_CONFIGURATION_STRUCTURE			= 0x2F,
	UARC_TUBE2_CONFIGURATION_STRUCTURE			= 0x30,
	UARC_TUBE1_DATA_STRUCTURE					= 0x31,
	UARC_TUBE2_DATA_STRUCTURE					= 0x32,
	UARC_TUBE2_CONFIGURATION_STRUCTURE_CHECKSUM = 0x34,
	UARC_TUBE2_DATA_STRUCTURE_CHECKSUM			= 0x35,
	UARC_TUBE_SWITCH							= 0x36,
	UARC_TUBE2_DROPOUT_CONFIGURATION			= 0x37,
	UARC_INFORMATION_EVENT						= 0x38,
	UARC_ERASE_EEPROM
}tUARC_Subindex;


//Entries for ADMC
#define ADMC_ENTRY/*------------------------------------------------------*/0x15

	#define ADMC_ENABLE_MOVEMENT											0
	#define ADMC_STATE														1
	#define ADMC_SPECIAL_MOVEMENTS											2
	#define ADMC_LEFT_MOTOR_VDC_INPUT_VOLTAGE								3
	#define ADMC_LEFT_MOTOR_IAC_CURRENT										4
	#define ADMC_LEFT_MOTOR_U_PHASE_CURRENT									5
	#define ADMC_LEFT_MOTOR_W_PHASE_CURRENT									6
	#define ADMC_LEFT_MOTOR_IGBTs_TEMPERATURE								7
	#define ADMC_RIGHT_MOTOR_VDC_INPUT_VOLTAGE								8
	#define ADMC_RIGHT_MOTOR_IAC_CURRENT									9
	#define ADMC_RIGHT_MOTOR_U_PHASE_CURRENT								10
	#define ADMC_RIGHT_MOTOR_W_PHASE_CURRENT								11
	#define ADMC_RIGHT_MOTOR_IGBTs_TEMPERATURE								12
	#define ADMC_BOARD_TEMPERATURE											13
	#define ADMC_OBSTACLE_DETECTION_DISTANCE								14
	#define ADMC_PROXIMITY_DISTANCE_MEASUREMENT_1							15
	#define ADMC_PROXIMITY_DISTANCE_MEASUREMENT_2							16
	#define ADMC_PROXIMITY_SENSORS_OPTIONS									17
	#define ADMC_DC_BUS_DISCHARGE											18
	#define ADMC_LEFT_FORCE_SENSOR_1_VAL									19
	#define ADMC_LEFT_FORCE_SENSOR_2_VAL									20
	#define ADMC_LEFT_FORCE_SENSOR_3_VAL									21
	#define ADMC_LEFT_FORCE_SENSOR_4_VAL									22
	#define ADMC_RIGHT_FORCE_SENSOR_1_VAL									23
	#define ADMC_RIGHT_FORCE_SENSOR_2_VAL									24
	#define ADMC_RIGHT_FORCE_SENSOR_3_VAL									25
	#define ADMC_RIGHT_FORCE_SENSOR_4_VAL									26
	#define ADMC_LEFT_GAUGE_CAL												27
	#define ADMC_LEFT_POT_VAL												28
	#define ADMC_RIGHT_GAUGE_CAL											29
	#define ADMC_RIGHT_POT_VAL												30
	#define ADMC_SEL_MAX_SPEED_FW											31
	#define ADMC_SEL_MAX_SPEED_BACK											32
	#define ADMC_SEL_I_VS_FORCE												33

//Entries for GENERATOR
#define GENERATOR_COMMANDS_ENTRY/*----------------------------------------*/0x20

	#define GENERATOR_DATA_BANK_DEFINE_PROCEDURE							1
	#define GENERATOR_DATA_BANK_CLEAR_PROCEDURE								2
	#define GENERATOR_DATA_BANK_CLEAR_ALL_PROCEDURE							3
	#define GENERATOR_DATA_BANK_ASSIGN_EXPOSURE								4
	#define GENERATOR_DATA_BANK_EXPOSURE_ACCEPTANCE							5
	#define GENERATOR_DATA_BANK_PROCEDURE_DATABANK_ACTIVATE					6
	#define GENERATOR_DATA_BANK_PROCEDURE_DATABANK_DEFAULT					7
	#define GENERATOR_DATA_BANK_DEFINE_PROCEDURE_NEXPOSURES					8
	
	#define GENERATOR_RAD_DATA_BANK_LOAD_V5									10
	#define GENERATOR_RAD_DATA_BANK_ACCEPTANCE								11
	#define GENERATOR_RAD_DATA_BANK_LOAD_V6									12
	
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_TECHNIQUE_MODE					20
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_KVP							21
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_MAS							22
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_MA								23
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_MS								24
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_MIN_INTEGRATION_TIME			25
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_MAX_INTEGRATION_TIME			26
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_FOCAL_SPOT						27
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_SENSITIVITY				28
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_DENSITY					29
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_CHAMBERS_V5				30
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_TUBEPOWERLIMIT					31
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_FPS							32
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_TRACKINGID						33
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_KVSCAN							34
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_MASCAN							35
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_PATIENT_SIZE					36
	#define GENERATOR_RAD_EXPOSURE_AEC_REFERENCE							37
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_AEC_CHAMBERS_V6				38
	
	#define GENERATOR_FL_DATA_BANK_LOAD_V5									40
	#define GENERATOR_FL_DATA_BANK_ACCEPTANCE								41
	#define GENERATOR_FL_DATA_BANK_LOAD_V6									42
	
	#define GENERATOR_FL_EXPOSURE_PARAMETER_KVP								50
	#define GENERATOR_FL_EXPOSURE_PARAMETER_MA_V5							51
	#define GENERATOR_FL_EXPOSURE_PARAMETER_MS								52
	#define GENERATOR_FL_EXPOSURE_PARAMETER_MAX_INTEGRATION_TIME			53
	#define GENERATOR_FL_EXPOSURE_PARAMETER_PPS								54
	#define GENERATOR_FL_EXPOSURE_PARAMETER_ABC_V5							55
	#define GENERATOR_FL_EXPOSURE_PARAMETER_HIGHDOSE_V5						56
	#define GENERATOR_FL_EXPOSURE_PARAMETER_KV_SCAN							57
	#define GENERATOR_FL_EXPOSURE_PARAMETER_QbyPPS							58
	#define GENERATOR_FL_EXPOSURE_PARAMETER_DOSELEVELID						59
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURVEID							60

	//New Message From V6R0.0
	#define GENERATOR_FL_EXPOSURE_PARAMETER_MA								61
	#define GENERATOR_FL_EXPOSURE_PARAMETER_ABC_HIGHDOSE_LOCKING			62
	#define GENERATOR_FL_EXPOSURE_PARAMETER_TARGET_LSB						63
	#define GENERATOR_FL_EXPOSURE_PARAMETER_FOCAL_SPOT						64
	#define GENERATOR_FL_EXPOSURE_PARAMETER_ABC_UPDATE_TIME					65
	#define GENERATOR_FL_EXPOSURE_PARAMETER_FILTER_ID						66
	
	#define GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V5					70
	#define GENERATOR_CURRENT_RAD_DATA_BANK_RAD_PARAMS_V6					71
	
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_TECHNIQUE_MODE			80
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_KVP					81
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_MAS					82
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_MA						83
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_MS						84
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_MIN_INTERGRATION_TIME	85
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_MAX_INTEGRATION_TIME	86
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_FOCAL_SPOT				87
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_AEC_SENSITIVITY		88
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_AEC_DENSITY			89
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_AEC_CHAMBERS			90
	//#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_TUBEPOWERLIMIT		91
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_FPS					92
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_TRACKINGID				93
	//#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_KVSCAN				94
	//#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_MASCAN				95
	#define GENERATOR_RAD_EXPOSURE_PARAMETER_CURRENT_AEC_REFERENCE			97
	
	#define GENERATOR_CURRENT_FL_DATA_BANK_FL_PARAMS_V5						100
	#define GENERATOR_CURRENT_FL_DATA_BANK_FL_PARAMS_V6						101
	
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_KVP						110
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_MA_V5					111
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_MS						112
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_MAX_INTEGRATION_TIME	113
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_PPS						114
	//#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_ABC						115
	//#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_HIGHDOSE					116
	//#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_KV_SCAN					117
	//#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_QbyPPS					118
	//#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_DOSELEVELID				119
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_CURVEID					120
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_MA						121
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_ABC_HIGHDOSE_LOCKIN		122
	#define GENERATOR_FL_EXPOSURE_PARAMETER_CURRENT_TARGET_LSB				123
	
	#define GENERATOR_OTHER_FUNCTIONS_FL_TIME_RESET							130
	#define GENERATOR_OTHER_FUNCTIONS_FL_5MIN_ALARM_RESET					131
	#define GENERATOR_OTHER_FUNCTIONS_GENPOWERLIMIT							132
    #define GENERATOR_OTHER_FUNCTIONS_FILAMENTS_ENABLED						133
	#define GENERATOR_OTHER_FUNCTIONS_TUBEPOWERLIMIT						134
	
	#define GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V5				140
	#define GENERATOR_EXPOSURE_MANAGEMENT_POST_CONDITION_V5					141
	#define GENERATOR_EXPOSURE_MANAGEMENT_START_STOP_EXPOSURE				142
	#define GENERATOR_EXPOSURE_MANAGEMENT_COMMUNICATIONS_INHIBIT_TIMEOUT	143
	#define GENERATOR_EXPOSURE_MANAGEMENT_RAD_POST_EXPOSURE					144
	#define GENERATOR_EXPOSURE_MANAGEMENT_FL_POST_EXPOSURE					145
	#define GENERATOR_EXPOSURE_MANAGEMENT_RESET_EXPOSURE_COUNTER			146
	#define GENERATOR_EXPOSURE_MANAGEMENT_GENERATOR_STATUS_V6				147
	#define GENERATOR_EXPOSURE_SWITCHES_ACTIVATION_DEVICE					148
	
	#define GENERATOR_MISCELLANEOUS_MAXPROCEDURE							150
	#define GENERATOR_MISCELLANEOUS_MAXDATABANK								151
	#define GENERATOR_MISCELLANEOUS_RAD_EXPOSURE_PARAMETER_RANGES			152 //0x98
	#define GENERATOR_MISCELLANEOUS_FLUORO_EXPOSURE_PARAMETER_RANGES		153
	
	#define GENERATOR_MISCELLANEOUS_AEC_EXTERNAL_REFERENCE					158
	#define GENERATOR_MISCELLANEOUS_COUNTERS_DATA							159

	#define GENERATOR_MISCELLANEOUS_DIGITAL_INPUTS_OUTPUTS					160


	#define	GENERATOR_LOCKIN_DATABANK										180
	#define GENERATOR_ASSIGN_LOCKINDB_TO_PROCEDURE							181
	#define	GENERATOR_LOCKIN_FRAME_REACHED									182
	#define GENERATOR_INJECTOR												183

	#define GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES			190
	#define GENERATOR_MISCELLANEOUS_RAD_PROCEDURE_PARAMETER_RANGES_ASSIGN	191
	#define GENERATOR_MISCELLANEOUS_RAD_PARAMETER_SCALES					192
	#define GENERATOR_MISCELLANEOUS_CALIBRATION_STATUS						193

#define IMAGING_SOFTWARE_UNIVERSAL_ENTRY/*--------------------------------*/0x30

	#define IMAGING_SOFTWARE_UNIVERSAL_CURRENT_LSB							20

//Entries for POSITIONER
#define POSITIONER_COMMANDS_ENTRY/*---------------------------------------*/0x40

	#define POSITIONER_DEFINE_PROCEDURE	                                    1 
	#define POSITIONER_CLEAR_PROCEDURE                                      2
	#define POSITIONER_CLEAR_ALL_PROCEDURES                                 3
	#define POSITIONER_ASSIGN_POSITIONING_DATA_BANK_TO_PROCEDURE            4
	#define POSITIONER_ASSIGN_COLLIMATOR_FOV_DATA_BANK_TO_PROCEDURE         5
	#define POSITIONER_ASSIGN_COLLIMATOR_FILTER_DATA_BANK_TO_PROCEDURE      6
	#define POSITIONER_ACTIVATE_PROCEDURE_AND_DATA_BANKS                    7
	#define POSITIONER_DEFAULT_PROCEDURE_AND_DATA_BANKS						8
	#define POSITIONER_PROCEDURE_NEXPOSURES_DEFINITION						9

	#define POSITIONER_LOAD_STANDAR_PROGRAMMABLE_POSITION_DATA_BANK			20
	#define POSITIONER_LOAD_FREE_POSITION_DATA_BANK							21
	#define POSITIONER_READ_STANDAR_PROGRAMMABLE_POSITION_DATA_BANK			22

	#define POSITIONER_TRACKING                                             50
	#define POSITIONER_DETENTS                                              51
	#define POSITIONER_MOVEMENT_SPEED										52
	#define POSITIONER_GRAB_AND_GO_AXIS                                     53
	#define POSITIONER_REQUESTED_GRID                                       55
	#define POSITIONER_ALIGNMENT											56
	#define POSITIONER_MUTE													68
	#define POSITIONER_COLUMN_IN_PARKING_POSITION							69 
	
	#define POSITIONER_LOAD_FOV_DATA_BANK                                   70
	#define POSITIONER_LOAD_FILTER_DATA_BANK                                71
	#define POSITIONER_LIGHT                                                72
	#define POSITIONER_MAXIMUM_FOV											73
	#define POSITIONER_SAVE_CURRENT_FOV										74
	#define POSITIONER_RECALL_SAVED_FOV										75
	
	#define POSITIONER_POSITIONER_STATUS                                    80
	#define POSITIONER_CURRENT_POSITION                                     81
	#define POSITIONER_CURRENT_COLLIMATOR_PARAMETERS                        82
	#define POSITIONER_CURRENT_GRID                                         83
	#define POSITIONER_CURRENT_DETECTOR_ROTATION_AND_GRID_LINES_ROTATION    84
	#define POSITIONER_POSITIONER_POST_EXPOSURE_POST_CONDITION              85
	#define POSITIONER_CURRENT_GEOMETRIC_PARAMETERS                         86
	#define	POSITIONER_CURRENT_DETECTOR_GRID_LINES_ION_CHAMBER_ROTATION		87
	
	#define POSITIONER_MAXIMUM_NUMBER_OF_PROCEDURES							90
	#define POSITIONER_MAXIMUM_NUMBER_OF_DATA_BANKS							91

//Entries for RFID
#define RFID_COMMANDS_ENTRY/*---------------------------------------------*/0x50
	
	#define RFID_ETH_STATUS													0x01
	#define RFID_ETH_MODE													0x02
	#define RFID_ETH_TAGS 													0x03
	#define RFID_ETH_LIST_MAX_CAPACITY										0x08
	#define RFID_ETH_LIST_TAGS_NUMBER										0x09
	#define RFID_ETH_LIST_TAG												0x0A
	#define RFID_ETH_LIST_CLEAR												0x0D
    #define RFID_ETH_CYPHERING_MODE										    0x0E
    #define RFID_ETH_CYPHERING_TAG										    0x0F
    #define RFID_ETH_CYPHERING_LIST_TAG										0x10

//Entries for DOSIMETER
#define DOSIMETER_ETH_COMMANDS_ENTRY/*------------------------------------*/0x60
	
	#define DOSIMETER_ETH_RESET												0x01
	#define DOSIMETER_ETH_READOUT											0x02
	#define DOSIMETER_ETH_READOUT_INTERVAL 									0x03
	#define DOSIMETER_ETH_RESET_ESTIMATED_DOSE								0x04
	#define DOSIMETER_ETH_ESTIMATED_DOSE									0x05

//Entries for SYSTEM MESSAGE
#define SYSTEM_COMMANDS_ENTRY/*-------------------------------------------*/0x80

	#define SYSTEM_SYSTEM_MESSAGE											1
	#define SYSTEM_ALL_SYSTEM_MESSAGES										2

	#define SYSTEM_LIGHT_REMOTE_MANAGEMENT									10
	#define SYSTEM_LIGHT_TURN_ON_OFF										11
	#define SYSTEM_SOUNDS_REMOTE_MANAGEMENT									20
	#define SYSTEM_SOUNDS_TURN_ON_OFF										21
#if 0
	#define SYSTEM_VD_LOAD_DESKTOP											10
	#define SYSTEM_VD_DESKTOP_DISPLAY										11
	#define SYSTEM_VD_NUMERIC_PROPERTY_SETUP								20
	#define SYSTEM_VD_NUMERIC_PROPERTY_VALUE								21
	#define SYSTEM_VD_STRING_PROPERTY										30
	#define SYSTEM_VD_NUMERIC_VALUE_TABLE									40
	#define SYSTEM_VD_NUMERIC_VALUE_TABLE_ITEM								41
	#define SYSTEM_VD_STRING_TABLE											42 
	#define SYSTEM_VD_SINGLE_USER_REQUEST									50
	#define SYSTEM_VD_USER_REQUEST_TABLE									51
#endif
	#define VIRTUAL_DESKTOP_OFFSET											100
	#define SYSTEM_VD_LOAD_DESKTOP											10 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_DESKTOP_DISPLAY										11 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_NUMERIC_PROPERTY_SETUP								220/*20 + VIRTUAL_DESKTOP_OFFSET*///Provisional
	#define SYSTEM_VD_NUMERIC_PROPERTY_VALUE								21 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_STRING_PROPERTY										30 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_NUMERIC_VALUE_TABLE									40 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_NUMERIC_VALUE_TABLE_ITEM								41 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_STRING_TABLE											42 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_SINGLE_USER_REQUEST									50 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_USER_REQUEST_TABLE									51 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_MULTICOLUMN_TABLE										52 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_CLEAR_TABLE											53 + VIRTUAL_DESKTOP_OFFSET
	#define SYSTEM_VD_PROPERTY_STATUS										60 + VIRTUAL_DESKTOP_OFFSET		//VISIBLE AND SENSITIVE

	#define SYSTEM_TOUCH_SCREEN_ENABLE										170
	#define SYSTEM_USER_ACTION_NOTIFICACION									171
//Entries for NETWORK
#define NETWORK_COMMANDS_ENTRY/*------------------------------------------*/0xA0
	
	#define NETWORK_CONNECTION_CHANGED										0x01
	#define NETWORK_NODESTATUS												0x02
	#define NETWORK_HEARTBEAT												0x03
	#define NETWORK_CONNECTION_REQUEST										0x04
	#define NETWORK_CONFIGURATION											0x05
	#define NETWORK_FILECONFIGUPDATE										0x06
	#define NETWORK_SNAPSHOT												0x07
	#define NETWORK_BACKUP_SNAPSHOT											0x08
	#define NETWORK_RESTORE													0x09
	#define NETWORK_CALIBRATION_UPDATE										0x0A

//Entries for PATIENT WORKFLOW
#define PATIENT_COMMANDS_ENTRY/*------------------------------------------*/0xE0

	#define PATIENT_PROCEDURE_DEFINITION									1
	#define PATIENT_PROCEDURE_ACTIVATE										2
	#define PATIENT_PROCEDURE_DEFAULT										3
	#define PATIENT_PROCEDURE_CLEAR											4
	#define PATIENT_PROCEDURE_CLEAR_ALL										5
	#define PATIENT_PROCEDURE_NEXPOSURES_DEFINITION							6
	
	#define PATIENT_REDEFINE_HANDFOOTSWITCH_ACTIVATION						10
	#define PATIENT_REDEFINE_WORKSTATION									11
	#define PATIENT_REDEFINE_WORKSTATION_REQUEST							12
	
	#define PATIENT_INFO													30
	#define PATIENT_INFO_PROCEDURE											31
	
	#define PATIENT_MISCELLANEOUS_SYNC_UP									40
	#define PATIENT_MISCELLANEOUS_POWEROFF_REQUEST							41
	#define PATIENT_MISCELLANEOUS_POWEROFF									42
	#define PATIENT_MISCELLANEOUS_POWEROFF_CANCEL							43
	#define PATIENT_MISCELLANEOUS_POWEROFF_CONFIRM							44
	#define PATIENT_MISCELLANEOUS_WORKINGMODE								50
	#define PATIENT_MISCELLANEOUS_POSITIONER_CALIBRATION_NUMBER				51

#define SERVICE_COMMANDS_ENTRY/*****************************************/0xC0
	
	#define SERVICE_STANDARD_LICENSE										1
	#define	SERVICE_LOG														2
	#define SERVICE_EXTENDED_LICENSE										3



#endif /* R2CP_INDEX_H_ */
