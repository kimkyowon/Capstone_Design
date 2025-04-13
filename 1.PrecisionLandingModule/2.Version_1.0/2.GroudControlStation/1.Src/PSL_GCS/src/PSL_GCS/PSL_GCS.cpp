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

void PSL_GCS_::updateJoystickValues(){
    if(hw_config.HW_INVERT_RUDDER) joystick.setRudder(invertValue(Value_Joysticks[RudderAxis]));
    else joystick.setRudder(Value_Joysticks[RudderAxis]);

    if(hw_config.HW_INVERT_THROTTLE) joystick.setThrottle(invertValue(Value_Joysticks[ThrottleAxis]));
    else joystick.setThrottle(Value_Joysticks[ThrottleAxis]);

    if(hw_config.HW_INVERT_X) joystick.setXAxis(invertValue(Value_Joysticks[XAxis]));
    else joystick.setXAxis(Value_Joysticks[XAxis]);

    if(hw_config.HW_INVERT_Y) joystick.setYAxis(invertValue(Value_Joysticks[YAxis]));
    else joystick.setYAxis(Value_Joysticks[YAxis]);
}

void PSL_GCS_::processButton(){
    if(Value_Buttons[0] = true) // Process 1
    if(Value_Buttons[1] = true) // Process 2
    
    
}

void PSL_GCS_::processSignal(){}

void PSL_GCS_::sendGcsData(){
    joystick.sendState();
}




uint16_t PSL_GCS_::invertValue(uint16_t value){
    return (1023 - value);
}

bool PSL_GCS_::getDiffState(){
    if(State_diffJoysticks || State_diffButtons || State_diffSignals){
        State_diffJoysticks = false;
        State_diffButtons   = false;
        State_diffSignals   = false;
        return true;
    }
    else return false;
}

void PSL_GCS_::printCurrentValues(){

    Serial.print(" RudderAxis [");
    Serial.print(invertValue(Value_Joysticks[RudderAxis]));
        
    Serial.print("] ThrottleAxis [");
    Serial.print(Value_Joysticks[ThrottleAxis]);
    
    Serial.print("] XAxis [");
    Serial.print(Value_Joysticks[XAxis]);

    Serial.print("] YAxis [");
    Serial.print(invertValue(Value_Joysticks[YAxis]));
    
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