

#include "../Joystick/Joystick.h"
#include "../Button/Button.h"
#include "stdint.h"

#define JOY_BASE_PIN            A0
#define BTN_BASE_PIN            0
#define SIG_BASE_PIN            7
#define COUNT_JOYSTICK_MAX      4
#define COUNT_BUTTON_MAX        2
#define COUNT_SIGNAL_MAX        2

#define ANALOG_MIN_VAL          0
#define ANALOG_MIDDLE_VAL       512
#define ANALOG_MAX_VAL          1023
#define ANALOG_MOVE_VAL         50
#define MARGIN_ARMING_DISARMING 30

typedef struct
{
    uint16_t PIN_XAxis;

    uint16_t PIN_RudderAxis;
    uint16_t PIN_YAxis;
    uint16_t PIN_ThrottleAxis;
    
    bool HW_INVERT_RUDDER;
    bool HW_INVERT_THROTTLE;
    bool HW_INVERT_X;
    bool HW_INVERT_Y;

} joystick_hw_config_t;

enum SequenceJoystickValues {
    XAxis, RudderAxis, YAxis, ThrottleAxis
};

enum SequenceSignalValues {
    Up, Down, Left, Right
};

enum Mode{
    TransData, DontTransData
};

enum AutoMove{
    D_Foward = 0x01, D_Backward, D_Left, D_Right,D_Land
};

class PSL_GCS_
{
private:
    bool State_mode;
    bool State_diffJoysticks;
    bool State_diffButtons;
    bool State_diffSignals;

    uint16_t Value_Joysticks[COUNT_JOYSTICK_MAX];
    bool     Value_Buttons[COUNT_BUTTON_MAX];
    byte Value_Signals;

    uint32_t Time_modeChange;
    
    joystick_hw_config_t hw_config = {
        .PIN_XAxis          = A0,
        .PIN_RudderAxis     = A1,
        .PIN_YAxis          = A3,
        .PIN_ThrottleAxis   = A2,
    
        .HW_INVERT_RUDDER   = true,
        .HW_INVERT_THROTTLE = false,
        .HW_INVERT_X        = false,
        .HW_INVERT_Y        = false
    };

    
public:
    PSL_GCS_();

    Joystick_ joystick;

    Button_ btn_mission1;
    Button_ btn_mission2;

    uint16_t Value_Update_Joysticks[COUNT_JOYSTICK_MAX];
    bool Value_Update_Buttons[COUNT_BUTTON_MAX];
    byte Value_Update_Signals;    
    
    void getJoystickValues();
    void getButtonValues();
    void getSignalValues(byte command);
    bool getStateMode();

    void updateJoystickValues();
    void processModeChange();
    void processSignal();
    void setJoystickValueToMiddle();
    void sendGcsData();
    void setDroneStop();

    uint16_t invertValue(uint16_t value);

    bool getDiffState();
    void printCurrentValues();

};


