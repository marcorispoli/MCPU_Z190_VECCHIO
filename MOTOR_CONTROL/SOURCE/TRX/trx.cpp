// Those defines SHALL be defined here!!!
#define RIDUZIONE 452.8
#define SPEED_DENOMINATOR       120

#include "application.h"
#include "pd4_dictionary.h"
#include "nanoj_trx.h"

static const pd4Nanotec::_OD_InitVector trxConfigVector[]={

    // Hardware configuration
    {OD_4013_01,1},    // 1 = EXTERNAL VCC LOGIC ON


    // NMT Behavior in case of fault
    {OD_1029_01,0},
    {OD_1029_02,1},
    {OD_2031_00,5000}, 	// Peak current
    {OD_2032_00,5000}, 	// Maximum Speed
    {OD_2033_00,0 },	// Plunger Block
    {OD_2034_00,51500 },// Upper Voltage Warning Level
    {OD_2035_00,20000 },// ****************************************Lower Voltage Warning Level
    {OD_2036_00,2000}, 	// Open Loop Current Reduction Idle Time
    {OD_2037_00,(uint) (-50) },	// Open Loop Current Reduction Value/factor

    // I2t Parameters
    {OD_203B_01,5000 },	// Nominal Current
    {OD_203B_02,1000 },	// Maximum Duration Of Peak Current
    {OD_203B_03,0 },	// Threshold
    {OD_203B_04,0 },	// CalcValue
    {OD_203B_05,5000}, 	// LimitedCurrent
    {OD_2056_00,500 },	// Limit Switch Tolerance Band

    // user unitS
    {OD_2061_00,1 },	// Velocity Numerator
    {OD_2062_00,SPEED_DENOMINATOR },  // Velocity Denominator        ***********************
    {OD_2063_00,1 },	// Acceleration Numerator
    {OD_2064_00,SPEED_DENOMINATOR}, 	// Acceleration Denominator    ***********************
    {OD_2065_00,1 },	// Jerk Numerator
    {OD_2066_00,60 },	// Jerk Denominator
    {OD_3202_00,9}, 	// Motor Drive Submode Select: 6:BLDC 3:CurRed 2:Brake 1:VoS 0:CLOOP/OLOOP

    // Motor Drive Sensor Display Closed Loop
    {OD_320B_01,0 	},  // Commutation
    {OD_320B_02,0 },	// Torque
    {OD_320B_03,1 },	// Velocity
    {OD_320B_04,1}, 	// Position

    // Motor Drive Parameter Set
    {OD_3210_01,50000 }, // Position Loop, Proportional Gain (closed Loop)
    {OD_3210_02,10 },	 // Position Loop, Integral Gain (closed Loop)

    // Analogue Inputs Control
    {OD_3221_00,0},     // 0 , Voltage, 1, Current


    // Digital Input Capture
    {OD_3241_01,0 },    // Control (0:off, 1:RE, 2:FE, 3:RE+FE)
    // 3241:02,0        // Capture Count
    //3241:03,0         // Encoder user units
    //3241:04,0         // Encoder Raw Value


    // Following Error Option Code
    {OD_3700_00,(uint) (-1)},
            // -1 No reaction
            // 0 Immediate stop
            // 1 Braking with "slow down ramp"
            // 2 Braking with "quick stop ramp"

    // Quick Stop Option Code
    {OD_605A_00,2 },
            // 0 Immediate stop
            // 1 Braking with "slow down ramp"
            // 2 Braking with "quick stop ramp"

    // Shutdown Option Code
    {OD_605B_00,0 },
            // 0 Immediate stop
            // 1 Braking with "slow down ramp"

    // Disable Option Code
    {OD_605C_00,0 },
            // 0 Immediate stop
            // 1 Braking with "slow down ramp"

    // Halt Option Code
    {OD_605D_00,0 },
            // 0 Immediate stop
            // 1 Braking with "slow down ramp"
            // 2 Braking with "quick stop ramp"

    // Fault Option Code
    {OD_605E_00,0 },
            // 0 Immediate stop
            // 1 Braking with "slow down ramp"
            // 2 Braking with "quick stop ramp"

    // Following Error Window and time
    {OD_6065_00,256}, 	// Window
    {OD_6066_00,100 },	// Time (ms)


    // Position Window + time
    {OD_6067_00,10 },	// Window
    {OD_6068_00,100 },	// Time


    // Position Range Limit
    {OD_607B_01,0}, 	// Min Position Range Limit
    {OD_607B_02,0 },	// Max Position Range Limit

    // Software Position Limit
    {OD_607D_01,(uint) (cGRAD_TO_POS(-2700)) },	// Min Position Limit
    {OD_607D_02,cGRAD_TO_POS(2700) },	// Max Position Limit

    // Polarity
    {OD_607E_00,0 },	// b7:1-> inverse rotaion


    // Position Encoder Resolution: EncInc/MotRev
    {OD_608F_01,2000 },	// Encoder Increments
    {OD_608F_02,1}, 	// Motor Revolutions

    {OD_60F2_00,0x0002}, // Absolute positionning

    // Gear Ratio
    {OD_6091_01,1}, 	// Motor Revolutions
    {OD_6091_02,1 },	// Shaft Revolutions

    // Max Absolute Acceleration and Deceleration
    {OD_60C5_00,5000 },// Max Acceleration
    {OD_60C6_00,5000 },// Max Deceleration

    // Homing registers
    {OD_6098_00,21},                                            // Homing method 21
    {OD_607C_00,0},                                             // Offset value

    {0,0,(canOpenDictionary::_ODDataType) 0,0} // Last element always present!!
};

static const pd4Nanotec::_OD_InitVector trxVolatileVector[]={

    //Digital Outputs Control
    {OD_3250_02,0},     // Function Inverted
    {OD_3250_03,3 },    // Force Enable
    {OD_3250_04,0 },    // Force Value
    {OD_3250_08,0 },    // Routing Enable

    // Digital Inputs Control
    {OD_3240_01,4},     // Special Function Enable (b2:ZS, b1:PL, b0:Nl)
    {OD_3240_02,0},     // Function Inverted (0,NO; 1,NC)
    {OD_3240_03,0 },    // Force Enable
    {OD_3240_04,0 },    // Force Value
    {OD_3240_06,0 },    // Input Range Select (0,threshold,5V, 1: threshold,24V)

    {OD_6081_00,250 },  // Position Target Speed (at the end of the ramp)
    {OD_6082_00,0 },	// End Velocity
    {OD_6083_00,250 },	// Position acceleraton
    {OD_6084_00,250 },	// Position Deceleration
    {OD_6085_00,cGRADsec_TO_ROT_min(400) },	// Quick Stop Deceleration

    {0,0,(canOpenDictionary::_ODDataType) 0,0} // Last element always present!!
};

static const pd4Nanotec::_OD_InitVector trx_zero_vector[]={
    {OD_6099_01,cGRADsec_TO_ROT_min(100)}, // Speed approaching to the target
    {OD_6099_02,cGRADsec_TO_ROT_min(50)}, // Speed approaching reverse
    {OD_609A_00,cGRADsec_TO_ROT_min(100)}, // Acceleration
    {OD_607C_00,cGRAD_TO_POS(0)}, // Offset
    {0,0,(canOpenDictionary::_ODDataType) 0,0} // Last element always present!!
};

static  pd4Nanotec::_OD_InitVector trx_positioning_vector[]={
    {OD_6081_00,cGRADsec_TO_ROT_min(400)}, // Target Speed
    {OD_6083_00,cGRADsec_TO_ROT_min(100)}, // Acc
    {OD_6084_00,cGRADsec_TO_ROT_min(100)}, // DEC
    {OD_6085_00,cGRADsec_TO_ROT_min(200) },	// Quick Stop Deceleration
    {OD_607A_00,cGRAD_TO_POS(0)},          // Target Position
    {0,0,(canOpenDictionary::_ODDataType) 0,0} // Last element always present!!
};



trxModule::trxModule(void):pd4Nanotec(Application::TRX_ID)
{
    this->configVector = (_OD_InitVector*) trxConfigVector; // This is the register vector that will be stored if necessary
    this->initVector = (_OD_InitVector*) trxVolatileVector; // Initialize the device with the Main OD vector
    this->zeroSettingVector = (_OD_InitVector*) trx_zero_vector; // Initialize the device with the zero setting vector
    this->positionSettingVector = trx_positioning_vector; // Initialize the device with the positioning setting vector
    this->setNanojVector((uchar*) nanojTrxfile, sizeof(nanojTrxfile));

    this->gearratio = RIDUZIONE; // Sets the current Gear reduction
    this->speed_denominator = SPEED_DENOMINATOR; // Sets the current Speed denominator


    setSafetyDigitalInput(1,1); // Set the internal Safety Input handling


}


