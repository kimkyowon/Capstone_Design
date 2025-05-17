#include "PSL_GCS.h"

PSL_GCS_::PSL_GCS_() : joystick(JOYSTICK_DEFAULT_REPORT_ID,
    JOYSTICK_TYPE_JOYSTICK, COUNT_BUTTON_MAX, 0,
    true, true, false,
    false, false, false,
    true, true,
    false, false, false),
btn_mission1(BTN_BASE_PIN),
btn_mission2(BTN_BASE_PIN+1) {}


void PSL_GCS_::getJoystickValues(){
    for(int index = 0; index < COUNT_JOYSTICK_MAX; index++){
        Value_Update_Joysticks[index] = analogRead(JOY_BASE_PIN + index);
        if(Value_Update_Joysticks[index] != Value_Joysticks[index]){
            Value_Joysticks[index] = Value_Update_Joysticks[index];
            State_diffJoysticks |= true;
        }
        else State_diffJoysticks |= false;
    }   
}

void PSL_GCS_:: getButtonValues(){
    // if want realtime value
    // Value_Update_Buttons[0] = btn_mission1.getButtonStateRealtime();
    // Value_Update_Buttons[1] = btn_mission2.getButtonStateRealtime();
    
    Value_Update_Buttons[0] = btn_mission1.getButtonPushed();
    Value_Update_Buttons[1] = btn_mission2.getButtonPushed();
    
    for(int index = 0; index < COUNT_BUTTON_MAX; index++){
        if(Value_Update_Buttons[index] != Value_Buttons[index]){
            Value_Buttons[index] = Value_Update_Buttons[index];
            State_diffButtons |= true;
            
        }
        else State_diffButtons |= false;
    }    
}

void PSL_GCS_::getSignalValues(byte command, byte distance){
    Value_Update_Signals   = command;
    Value_Update_Distances = distance; 
}

bool PSL_GCS_::getStateMode(){
    return State_mode;
} 

void PSL_GCS_::updateJoystickValues(){
    uint16_t TempValueArray[COUNT_JOYSTICK_MAX]; // XAxis, RudderAxis, YAxis, ThrottleAxis
    for(int i = 0; i < COUNT_JOYSTICK_MAX; i++) TempValueArray[i] = Value_Joysticks[i];

    // check flip or bypass
    if(hw_config.HW_INVERT_RUDDER) TempValueArray[RudderAxis] = invertValue(TempValueArray[RudderAxis]);
    if(hw_config.HW_INVERT_THROTTLE) TempValueArray[ThrottleAxis] = invertValue(TempValueArray[ThrottleAxis]);
    if(hw_config.HW_INVERT_X) TempValueArray[XAxis] = invertValue(TempValueArray[XAxis]);
    if(hw_config.HW_INVERT_Y) TempValueArray[YAxis] = invertValue(TempValueArray[YAxis]);
    
    // check arming or disarming
    if(TempValueArray[ThrottleAxis] <= (ANALOG_MIN_VAL + MARGIN_ARMING_DISARMING) ){
        if(TempValueArray[RudderAxis] >= (ANALOG_MAX_VAL - MARGIN_ARMING_DISARMING) ){
            // MAKE ARMING
            TempValueArray[ThrottleAxis] = ANALOG_MIN_VAL;
            TempValueArray[RudderAxis]   = ANALOG_MAX_VAL;
        }
        else if(TempValueArray[RudderAxis] <= (ANALOG_MIN_VAL + MARGIN_ARMING_DISARMING) ){
            // MAKE DISARMING
            TempValueArray[ThrottleAxis] = ANALOG_MIN_VAL;
            TempValueArray[RudderAxis]   = ANALOG_MIN_VAL;
        }
    }
    
    // final set
    joystick.setRudder(TempValueArray[RudderAxis]);
    joystick.setThrottle(TempValueArray[ThrottleAxis]);
    joystick.setXAxis(TempValueArray[XAxis]);
    joystick.setYAxis(TempValueArray[YAxis]);
    for(int i = 0; i < COUNT_BUTTON_MAX; i++){
        joystick.setButton(i,Value_Buttons[i]);
    }
}

void PSL_GCS_::processModeChange(){
    if(btn_mission1.getButtonPushed()) {
        setDroneStop();
        State_mode = !State_mode;
        // Serial.print("Mode Change!! [ ");
        // if(State_mode) Serial.print("TransData");
        // else Serial.print("DontTransData");

        // Serial.print(" ] to [ ");
        // if(State_mode) Serial.println("DontTransData ]");
        // else Serial.println("TransData ]");
        btn_mission1.set_isPushedToFalse();
    }
}

// void PSL_GCS_::processRTL(){
//     if(btn_mission2.getButtonPushed()){
//         joystick.setButton(2,1);
//         sendGcsData();
//         btn_mission2.set_isPushedToFalse();
//     }else{
//         joystick.setButton(2,0);
//     }
// }

void PSL_GCS_::processSignal(){
    
    if(Value_Signals != Value_Update_Signals || Value_Distances != Value_Update_Distances){
        Value_Signals   = Value_Update_Signals;
        Value_Distances = Value_Update_Distances;

        State_diffSignals = true;
    } else State_diffSignals = false;

    if(State_diffSignals == true ){
        // Serial.print("Get Mission!! [ ");
        // Serial.print(Value_Signals);
        // Serial.println(" ]");
        setJoystickValueToMiddle();
        
        switch(Value_Signals){
            case D_Foward:
                joystick.setYAxis(ANALOG_MIDDLE_VAL + Value_Distances);
                Serial.print("Go Foward!!...");
                break;
            case D_Backward:
                joystick.setYAxis(ANALOG_MIDDLE_VAL - Value_Distances);
                Serial.print("Go Backward!!...");
                break;     
            case D_Left:
                joystick.setXAxis(ANALOG_MIDDLE_VAL - Value_Distances);    
                Serial.print("Go Left!!...");
                break;
            case D_Right:
                joystick.setXAxis(ANALOG_MIDDLE_VAL + Value_Distances);
                Serial.print("Go Right!!...");
                break;
            case D_Land:
                joystick.setThrottle(ANALOG_MIDDLE_VAL - Value_Distances);
                Serial.print("Center...Landing start!!...");
                break;
        }
        Serial.print("Distance : [ ");
        Serial.print(Value_Distances);
        Serial.println(" ]");
        
        sendGcsData();
    }else {
        if (Value_Signals == D_Land){
            joystick.setThrottle(ANALOG_MIDDLE_VAL - Value_Distances);
            sendGcsData();
        }
    }
}

void PSL_GCS_::setJoystickValueToMiddle(){
    joystick.setRudder(ANALOG_MIDDLE_VAL);
    joystick.setThrottle(ANALOG_MIDDLE_VAL);
    joystick.setXAxis(ANALOG_MIDDLE_VAL);
    joystick.setYAxis(ANALOG_MIDDLE_VAL);
}

void PSL_GCS_::sendGcsData(){
    joystick.sendState();
}

void PSL_GCS_::setDroneStop(){
    setJoystickValueToMiddle();
    sendGcsData();
}


uint16_t PSL_GCS_::invertValue(uint16_t value){
    return (1023 - value);
}

bool PSL_GCS_::getDiffState(){
    if(State_diffJoysticks || State_diffButtons){
        State_diffJoysticks = false;
        State_diffButtons   = false;
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