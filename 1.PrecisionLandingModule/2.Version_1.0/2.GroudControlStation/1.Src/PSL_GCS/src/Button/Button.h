#ifndef BUTTON_H
#define BUTTON_H
#include "stdint.h"

class Button_{

private:
    uint8_t PIN;
    uint32_t Time_pressStart;
    bool State_isButtonPushed;

public:
    Button_(uint8_t pin);
    void ISR_isFalling();
    void ISR_isRising(); 
    bool getButtonState();
    uint8_t returnPin();
};

#endif // BUTTON_H