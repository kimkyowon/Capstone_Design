#include "PSL_GCS.h"

PSL_GCS_::PSL_GCS_() : joystick(JOYSTICK_DEFAULT_REPORT_ID,
    JOYSTICK_TYPE_JOYSTICK, COUNT_BUTTON_MAX, 0,
    true, true, false,
    false, false, false,
    true, true,
    false, false, false),
btn_mission1(BTN_BASE_PIN),
btn_mission2(BTN_BASE_PIN + 1) {}


void PSL_GCS_::getJoystickValues(){
    for(int index = 0; index < COUNT_JOYSTICK_MAX; index++){
        Value_Update_Joysticks[index] = analogRead(JOY_BASE_PIN + index);
        if(Value_Update_Joysticks[index] != Value_Joysticks[index]){
            Value_Joysticks[index] = Value_Update_Joysticks[index];
            State_diffJoysticks = true;
        }
        else State_diffJoysticks = false;
    }   
}

void PSL_GCS_::getButtonValues(){
    Value_Update_Buttons[0] = btn_mission1.getButtonState();
    Value_Update_Buttons[1] = btn_mission2.getButtonState();

    for(int index = 0; index < COUNT_BUTTON_MAX; index ++){
        if(Value_Update_Buttons[index] != Value_Buttons[index]){
            Value_Buttons[index] = Value_Update_Buttons[index];
            State_diffButtons = true;
        }
        else State_diffButtons = false;
    }

    
}

void PSL_GCS_::getSignalValues(){
    for(int index = 0; index < COUNT_SIGNAL_MAX; index++){
        Value_Update_Signals[index] = analogRead(SIG_BASE_PIN + index);
        if(Value_Update_Signals[index] != Value_Update_Signals[index]){
            Value_Update_Signals[index] = Value_Update_Signals[index];
            State_diffSignals = true;
        }
        else State_diffSignals = false;
    }   
}

uint16_t PSL_GCS_::invertValue(uint16_t value){
    return (1023 - value);
}

bool PSL_GCS_::getDiffState(){
    if(State_diffJoysticks || State_diffButtons || State_diffSignals){
        return true;
    }
    else return false;
}

void PSL_GCS_::printCurrentValues(){

    Serial.print(" RudderAxis [");
    Serial.print(invertValue(Value_Joysticks[1]));
        
    Serial.print("] ThrottleAxis [");
    Serial.print(Value_Joysticks[3]);
    
    Serial.print("] XAxis [");
    Serial.print(Value_Joysticks[0]);

    Serial.print("] YAxis [");
    Serial.print(invertValue(Value_Joysticks[2]));
    
    Serial.print("] btn_mission1 [");
    Serial.print(Value_Buttons[0]);

    Serial.print("] btn_mission2 [");
    Serial.print(Value_Buttons[1]);
    
    /*
    Serial.print("] btn_mission1 [");
    Serial.print("] btn_mission1 [");
    Serial.print("] btn_mission1 [");
    Serial.print("] btn_mission1 [");
    
    Serial.print(" XAxis ");
    Serial.print(" XAxis ");
    Serial.print(" XAxis ");
    Serial.print(" XAxis ");
    
    Serial.print(" XAxis ");
    Serial.print(" XAxis ");
    */
    Serial.println("]");
}