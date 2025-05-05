#include "Button.h"
#include "stdint.h"
#include <Arduino.h>

Button_::Button_(uint8_t pin){
    PIN = pin;
    pinMode(PIN, INPUT_PULLUP);
}

void Button_::ISR_isChange(){
    if(digitalRead(PIN)) State_Button = Not_pushed; 
    else State_Button = Pushed;
}

void Button_::debounceButton(){
    if(State_Button == Pushed){
        Time_pressStart++;
    }
    else{
        if(Time_pressStart >= 10){
            if(Time_pressStart >= ValidTime){
                Serial.print("Detect user push button [");
                Serial.print(PIN);
                Serial.print("] while :");
                Serial.println(Time_pressStart);
                State_isButtonPushed = true;
                Time_pressStart = 0;
            }
            else{
                Serial.print(Time_pressStart);
                Serial.println(" Not yet.. holding push state more...");
                Time_pressStart = 0;
                set_isPushedToFalse();
            }
        }
    }
}

bool Button_::getButtonStateRealtime(){
    return State_Button;
}

bool Button_::getButtonPushed(){
    return State_isButtonPushed;
}

void Button_::set_isPushedToFalse(){
    State_isButtonPushed = false;
}



uint8_t Button_::returnPin(){
    return PIN;
}




