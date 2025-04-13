#include "src/PSL_GCS/PSL_GCS.h"

PSL_GCS_ CapstoneGCS;

uint16_t countHeartBeat;

static void btn1ChangeISR();
static void btn2ChangeISR();

void setup(){
  delay(10);
  Serial.begin(115200);
  
  analogReference(DEFAULT);

  attachInterrupt(digitalPinToInterrupt(CapstoneGCS.btn_mission1.returnPin()), btn1ChangeISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CapstoneGCS.btn_mission2.returnPin()), btn2ChangeISR, CHANGE);
  
  delay(10);
}


void loop(){
  
  // Main Processes
  CapstoneGCS.getJoystickValues();
  CapstoneGCS.getButtonValues();

  CapstoneGCS.btn_mission1.debounceButton();
  CapstoneGCS.btn_mission2.debounceButton();

  CapstoneGCS.processModeChange();
  if(CapstoneGCS.getDiffState()) {
    
    if(CapstoneGCS.getStateMode() == TransData) {
      CapstoneGCS.updateJoystickValues();
      CapstoneGCS.sendGcsData();
    }
    //CapstoneGCS.updateSignalValues();    
    CapstoneGCS.printCurrentValues();
  }else {
    countHeartBeat++;
    if(countHeartBeat >= 1000){
      Serial.print(".");
      countHeartBeat = 0;
    }
  }
  delay(1);
}




static void btn1ChangeISR() {
  CapstoneGCS.btn_mission1.ISR_isChange();
}

static void btn2ChangeISR() {
  CapstoneGCS.btn_mission2.ISR_isChange();
}

