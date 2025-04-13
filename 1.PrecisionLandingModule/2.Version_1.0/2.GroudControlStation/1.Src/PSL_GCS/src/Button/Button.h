#ifndef BUTTON_H
#define BUTTON_H
#include "stdint.h"

#define ValidTime 1000

enum StateButton_{
    Pushed, Not_pushed
};

class Button_{

private:
    uint8_t PIN;
    uint32_t Time_pressStart;
    bool State_Button;

    bool State_isButtonPushed;

public:
    Button_(uint8_t pin);
    
    void ISR_isChange();
    void debounceButton();

    bool getButtonStateRealtime();
    bool getButtonPushed();
    void set_isPushedToFalse();
    
    uint8_t returnPin();
};

#endif // BUTTON_H