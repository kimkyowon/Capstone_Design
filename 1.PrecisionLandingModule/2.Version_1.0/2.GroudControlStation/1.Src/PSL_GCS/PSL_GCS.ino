#include "src/PSL_GCS/PSL_GCS.h"


PSL_GCS_ CapstoneGCS;

static void btn1FallingISR();
static void btn1RisingISR();
static void btn2FallingISR();
static void btn2RisingISR();


void setup(){
  delay(10);
  Serial.begin(115200);
  
  attachInterrupt(digitalPinToInterrupt(CapstoneGCS.btn_mission1.returnPin()), btn1FallingISR, LOW);
  attachInterrupt(digitalPinToInterrupt(CapstoneGCS.btn_mission1.returnPin()), btn1RisingISR, RISING);
    
  attachInterrupt(digitalPinToInterrupt(CapstoneGCS.btn_mission2.returnPin()), btn2FallingISR, LOW);
  attachInterrupt(digitalPinToInterrupt(CapstoneGCS.btn_mission2.returnPin()), btn2RisingISR, RISING);
  
}


void loop(){
  CapstoneGCS.getJoystickValues();
  CapstoneGCS.getButtonValues();
  
  if(CapstoneGCS.getDiffState()) CapstoneGCS.printCurrentValues();
  //delay(1);
}




static void btn1FallingISR() {
  CapstoneGCS.btn_mission1.ISR_isFalling();
}

static void btn1RisingISR() {
  CapstoneGCS.btn_mission1.ISR_isRising();
}

static void btn2FallingISR() {
  CapstoneGCS.btn_mission2.ISR_isFalling();
}

static void btn2RisingISR() {
  CapstoneGCS.btn_mission2.ISR_isRising();
}
