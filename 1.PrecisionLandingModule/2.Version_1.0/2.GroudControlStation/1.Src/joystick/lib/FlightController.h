#ifndef _FLIGHT_CONTROLLER_H_
#define _FLIGHT_CONTROLLER_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ARRAY_SIZE(x) ( sizeof((x))/sizeof((x)[0]) )
#define ABS(x)        ( ((x)<0)?-(x):(x) )
#define SQUARE(x)     ( (x)*(x) )
#define MAX(x,y)      ( (x) > (y) ? (x) : (y) )
#define MIN(x,y)      ( (x) < (y) ? (x) : (y) )
#define UPCASE(c)     ( ( (c)>='a' && (c)<='z') ? (c)-('a'-'A') : (c) )
#define LOWCASE(c)    ( ( (c)>='A' && (c)<='Z') ? (c)+('a'-'A') : (c) )

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////////////////////////////


enum JOY_TYPE {
    JOY_TYPE_RUDDER = 0, 
    JOY_TYPE_THROTTLE, 
    JOY_TYPE_X_AXIS, 
    JOY_TYPE_Y_AXIS,
    JOY_TYPE_END
};

enum JOY_REAL_RANGE_IDX {
    JOY_REAL_RANGE_MIN = 0, 
    JOY_REAL_RANGE_MAX,
    JOY_REAL_RANGE_CENTER,
    JOY_REAL_RANGE_IDX_MAX
};

// NOTE: Reference ADC PIN Names - https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
#define JOY_BASE_PIN            A0
#define JOY_RUDDER_PIN          A0 // rudder
#define JOY_THROTTLE_PIN        A1 // throttle
#define JOY_X_AXIS_PIN          A2 // x
#define JOY_Y_AXIS_PIN          A3 // y


#define JOY_RANGE_MIN           0
#define JOY_RANGE_MAX           1023
#define JOY_RANGE               (JOY_RANGE_MAX - JOY_RANGE_MIN)
#define JOY_RANGE_CENTER        512
#define JOY_RANGE_MARGIN        180
#define JOY_RANGE_CENTER_MARGIN 4

//#define JOY_THROTTLE_RANGE_MIN  102
//#define JOY_THROTTLE_RANGE_MAX  992

#define JOY_GAIN                (0.7)

#define JOY_MODE_1              0
#define JOY_MODE_2              1

// JOY HW COMMON Settings
#define HW_COMMON_MCU_IO_CALB_MODE_PIN       12       // For Joystick Calibration Mode
#define HW_COMMON_MCU_IO_JOYSTICK_MODE_PIN  13        // For Joystick Mode
#define HW_COMMON_SWITCH_COUNT_MAX          2         // Max Switch Count
#define HW_COMMON_SWITCH_START              0         // Switch Start Index
#define HW_COMMON_SWITCH_END                23        // Switch End Index
#define HW_COMMON_SX1509_ADDRESS_00         0x3E      // 1st IO Expander I2C Address
#define HW_COMMON_SX1509_ADDRESS_01         0x3F      // 2nd IO Expander I2C Address

// JOY_HW_DESIGN_T Settings
#define HW_DT_EXPANDER_IO_START             0
#define HW_DT_EXPANDER_IO_END               15
#define HW_DT_COMMON_SX1509_COUNT_MAX          2         // Max IO Expander(SX1509) Count 
#define HW_DT_CALC_START_EXPGPIO            12
#define HW_DT_CALC_END_EXPGPIO              13

// JOY_HW_DESIGN_1 Settings
#define HW_D1_MCU_IO_START                  2
#define HW_D1_MCU_IO_END                    12
#define HW_D1_MCU_IO_COUNT                  (HW_D1_MCU_IO_END-HW_D1_MCU_IO_START + 1)

// JOY_HW_DESIGN_2 Settings
#define HW_D2_EXPANDER_IO_START             0
#define HW_D2_EXPANDER_IO_END               15
#define HW_D2_COMMON_SX1509_COUNT_MAX       1         // Max IO Expander(SX1509) Count 
#define HW_D2_CAL_SIG_GPIO_01               7
#define HW_D2_CAL_SIG_GPIO_02               8
#define HW_D2_CAL_SIG_GPIO_03               9
#define HW_D2_CAL_SIG_GPIO_04               10
#define HW_D2_CAL_SIG_GPIO_05               11
#define HW_D2_CALC_START_EXPGPIO            14
#define HW_D2_CALC_END_EXPGPIO              15

// JOY_HW_DESIGN_3 Settings
#define HW_D3_EXPANDER_IO_START             0
#define HW_D3_EXPANDER_IO_END               13
#define HW_D3_COMMON_SX1509_COUNT_MAX       1         // Max IO Expander(SX1509) Count 
#define HW_D3_CAL_SIG_GPIO_01               7
#define HW_D3_CAL_SIG_GPIO_02               8
#define HW_D3_CALC_START_EXPGPIO            12
#define HW_D3_CALC_END_EXPGPIO              13

// JOY_HW_DESIGN_4 Settings
#define HW_D4_EXPANDER_IO_START             0
#define HW_D4_EXPANDER_IO_END               7
#define HW_D4_COMMON_SX1509_COUNT_MAX       1         // Max IO Expander(SX1509) Count 
#define HW_D4_CALC_START_EXPGPIO            4
#define HW_D4_CALC_END_EXPGPIO              6

#define HW_D4_CAL_SIG_GPIO_01               7         //LED0
#define HW_D4_CAL_SIG_GPIO_02               8         //LED1

#endif