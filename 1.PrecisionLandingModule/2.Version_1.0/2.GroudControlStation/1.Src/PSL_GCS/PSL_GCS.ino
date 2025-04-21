#include "src/PSL_GCS/PSL_GCS.h"
#include <Wire.h>

#define I2C_ADDRESS 0x08

PSL_GCS_ CapstoneGCS;
uint16_t countHeartBeat;
byte command;

static void btn1ChangeISR();
static void btn2ChangeISR();
static void InturruptSignal();
void receiveEvent();

void setup(){
  delay(10);
  
  pinMode(SIG_BASE_PIN, INPUT);

  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);

  Serial.begin(115200);
  
  analogReference(DEFAULT);

  attachInterrupt(digitalPinToInterrupt(BTN_BASE_PIN), btn1ChangeISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN_BASE_PIN+1), btn2ChangeISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SIG_BASE_PIN),InturruptSignal, CHANGE);
  
  delay(10);
}


void loop(){
  
  // Main Processes
 
  CapstoneGCS.getButtonValues();

  CapstoneGCS.btn_mission1.debounceButton();
  CapstoneGCS.btn_mission2.debounceButton();

  CapstoneGCS.processModeChange();

  if(CapstoneGCS.getStateMode() == TransData) {
    CapstoneGCS.getJoystickValues();

    if(CapstoneGCS.getDiffState()) {
      CapstoneGCS.updateJoystickValues();
      //CapstoneGCS.printCurrentValues();
      CapstoneGCS.sendGcsData();
    }else{
      countHeartBeat++;
      if(countHeartBeat >= 1000){
        Serial.print(".");
        countHeartBeat = 0;
      }
    }
  }else{     // CapstoneGCS.getStateMode() == DontTransData "AutoMode"
    CapstoneGCS.getSignalValues(command);
    CapstoneGCS.processSignal();
  } 
  
  delay(1);
  
}




static void btn1ChangeISR() {
  CapstoneGCS.btn_mission1.ISR_isChange();
}
static void btn2ChangeISR() {
  CapstoneGCS.btn_mission2.ISR_isChange();
}
static void InturruptSignal(){
  CapstoneGCS.setDroneStop();
}

void receiveEvent(){
  command = Wire.read();
  // Serial.print("receiveEvent() : ");
  // Serial.println(command);
}