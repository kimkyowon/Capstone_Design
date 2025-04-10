#include "Button.h"
#include "stdint.h"
#include <Arduino.h>

Button_::Button_(uint8_t pin){
    PIN = pin;
    pinMode(PIN, INPUT);
}

void Button_::ISR_isFalling(){
    Time_pressStart++; 
}

void Button_::ISR_isRising(){
    if(Time_pressStart >= 1000){
        if(State_isButtonPushed == false) State_isButtonPushed = true;
    }
    else {
        Time_pressStart      = 0;
        State_isButtonPushed = false;
    }
}

bool Button_::getButtonState(){
    return State_isButtonPushed;
}

uint8_t Button_::returnPin(){
    return PIN;
}




