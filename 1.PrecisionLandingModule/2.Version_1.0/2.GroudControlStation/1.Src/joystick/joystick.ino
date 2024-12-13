/* USER CODE BEGIN Header */


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lib/Joystick.h"
#include "lib/FlightController.h"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */



/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 2, 0,
  true, true, false, false, false, false,
  true, true, false, false, false);
  
unsigned int g_uHeartBeatCount = 0;
const bool g_bTestAutoSendMode = false;
unsigned int g_uJoyPrevValues[JOY_TYPE_END] = {0,};
uint8_t g_u8Prev_switchValues[HW_COMMON_SWITCH_COUNT_MAX] = {0,};
unsigned int g_uJoyRealRangeValues[JOY_TYPE_END][JOY_REAL_RANGE_IDX_MAX] = {  {JOY_RANGE_MAX, JOY_RANGE_MIN, (unsigned int)(JOY_RANGE / 2)}, {JOY_RANGE_MAX, JOY_RANGE_MIN, (unsigned int)(JOY_RANGE / 2)}, {JOY_RANGE_MAX, JOY_RANGE_MIN, (unsigned int)(JOY_RANGE / 2)}, {JOY_RANGE_MAX, JOY_RANGE_MIN, (unsigned int)(JOY_RANGE / 2)}};
uint8_t g_u8InitJoyMode = JOY_MODE_2; // Default configuration is Mode 2.
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

void init_ADC();
bool mappingJoystickValues(unsigned int *, uint8_t );
void init_JoyRange();
bool getJoystickValues(unsigned int *, uint8_t );
bool getSwitchValues(uint8_t *);
bool setJoystickValues(unsigned int *, uint8_t *);
void print_Joyval(unsigned int *);
void print_SWval(uint8_t *u8SwitchValues);
void Invert_Joyval(unsigned int *, JOY_TYPE);


void setup() { 
  // put your setup code here, to run once:
#if DBG_PRINT_JOYSTICK_DATA
  Serial.begin(115200);
#endif
  if(g_bTestAutoSendMode) Joystick.begin();
  delay(10);
  init_ADC();
  init_JoyRange();
}

void loop() {
  // put your main code here, to run repeatedly:
  bool bDiffJoyState = false;
  bool bDiffBtnState = false;
  uint8_t u8SwitchValues[HW_COMMON_SWITCH_COUNT_MAX] = {0,};
  unsigned int uJoyValues[JOY_TYPE_END] = {0,};
  
  bDiffJoyState = getJoystickValues(uJoyValues, JOY_TYPE_END);
  bDiffBtnState = getSwitchValues(u8SwitchValues);

  // Send state through the HID when values are changed.
  if (bDiffJoyState || bDiffBtnState)
  {
    mappingJoystickValues(uJoyValues, JOY_TYPE_END);
    setJoystickValues(uJoyValues, u8SwitchValues);

    print_Joyval(uJoyValues);
    print_SWval(u8SwitchValues);
    if (g_bTestAutoSendMode == false) Joystick.sendState();
  }

  if (g_uHeartBeatCount++ == 1000)
  {
    Serial.print(".");
    g_uHeartBeatCount = 0;
  }
  delay(1);
}


void init_ADC()
{
  analogReference(DEFAULT);
}

void init_JoyRange(){
  // Set value range of the Joystick.
  Joystick.setXAxisRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setYAxisRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setZAxisRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setThrottleRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setRudderRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setRxAxisRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setRyAxisRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
  Joystick.setRzAxisRange(JOY_RANGE_MIN, JOY_RANGE_MAX);
}

bool getJoystickValues(unsigned int *pJoyVal, uint8_t u8CalCount)
{
  bool bDiffJoyState = false;
  for (uint8_t nIdxJoy = 0; nIdxJoy <= 4; nIdxJoy++){
    pJoyVal[nIdxJoy] = analogRead(JOY_BASE_PIN + nIdxJoy); // + g_nJoyOffsets[nIdxJoy];

    if (g_uJoyPrevValues[nIdxJoy] != pJoyVal[nIdxJoy]){
      bDiffJoyState = true;
      g_uJoyPrevValues[nIdxJoy] = pJoyVal[nIdxJoy];
     }
  }
  
  return bDiffJoyState;
}

bool getSwitchValues(uint8_t *updateValues){
  bool retDiffState = false;
  int nButtonIoCount = 0;

  uint8_t SW_updateValues[HW_COMMON_SWITCH_COUNT_MAX] = {0,}; 
  for (uint8_t idx = HW_COMMON_SWITCH_START; idx <= HW_COMMON_SWITCH_END; idx++){
    SW_updateValues[idx - HW_COMMON_SWITCH_START] = digitalRead(idx);
    
  }
  return retDiffState;
}

bool mappingJoystickValues(unsigned int *pJoyVal, uint8_t u8CalCount)
{
  for (uint8_t nIdxJoy = 0; nIdxJoy < JOY_TYPE_END; nIdxJoy++)
  {
    pJoyVal[nIdxJoy] = constrain(pJoyVal[nIdxJoy], g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_MIN], g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_MAX]);
    if (g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_MIN] < g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_MAX])
    {
      // Mapping range values.
      if (pJoyVal[nIdxJoy] == g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_CENTER])
      {
        pJoyVal[nIdxJoy] = JOY_RANGE_CENTER;
      }
      else if (pJoyVal[nIdxJoy] < g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_CENTER])
      {
        pJoyVal[nIdxJoy] = map(pJoyVal[nIdxJoy],
                               g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_MIN],
                               g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_CENTER],
                               JOY_RANGE_MIN, JOY_RANGE_CENTER);
#if HW_JOYSTICK_MAP_QUADRANTIC
        pJoyVal[nIdxJoy] = map_quadraticfunc(pJoyVal[nIdxJoy]);
#endif
      }
      else if (pJoyVal[nIdxJoy] > g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_CENTER])
      {
        pJoyVal[nIdxJoy] = map(pJoyVal[nIdxJoy],
                               g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_CENTER],
                               g_uJoyRealRangeValues[nIdxJoy][JOY_REAL_RANGE_MAX],
                               JOY_RANGE_CENTER, JOY_RANGE_MAX);
#if HW_JOYSTICK_MAP_QUADRANTIC
        pJoyVal[nIdxJoy] = map_quadraticfunc(pJoyVal[nIdxJoy]);
#endif
      }
    }
    #if HW_JOYSTICK_CENTER_MARGIN
    if ((int)ABS((int)pJoyVal[nIdxJoy] - (int)JOY_RANGE_CENTER) < (int)JOY_RANGE_CENTER_MARGIN)
    {
      pJoyVal[nIdxJoy] = JOY_RANGE_CENTER;
    }
    #endif
  }
}

bool setJoystickValues(unsigned int *pJoyVal, uint8_t *pBtnVal)
{
  if (g_u8InitJoyMode == JOY_MODE_2)
  {
    Joystick.setRudder(pJoyVal[JOY_TYPE_RUDDER]);
    Joystick.setThrottle(pJoyVal[JOY_TYPE_THROTTLE]);
    Joystick.setXAxis(JOY_RANGE_MAX - pJoyVal[JOY_TYPE_X_AXIS]);
    Joystick.setYAxis(pJoyVal[JOY_TYPE_Y_AXIS]);
  }
  else if (g_u8InitJoyMode == JOY_MODE_1)
  {
    Joystick.setRudder(pJoyVal[JOY_TYPE_RUDDER]);
    Joystick.setThrottle(pJoyVal[JOY_TYPE_Y_AXIS]);
    Joystick.setXAxis(JOY_RANGE_MAX - pJoyVal[JOY_TYPE_X_AXIS]);
    Joystick.setYAxis(JOY_RANGE_MAX - pJoyVal[JOY_TYPE_THROTTLE]);
  }

  for (uint8_t idx = HW_COMMON_SWITCH_START; idx < HW_COMMON_SWITCH_COUNT_MAX; idx++)
  {
    Joystick.setButton(idx, (pBtnVal[idx] == 0) ? 1 : 0);
  }
}
void print_Joyval(unsigned int *uJoyValues)
{
#if DBG_PRINT_JOYSTICK_DATA
  Serial.print("[SEND] ");
  // Update current state
  for (uint8_t nIdxJoy = 0; nIdxJoy < JOY_TYPE_END; nIdxJoy++)
  {
    Serial.print("A");
    Serial.print(nIdxJoy);
    Serial.print(": ");
    Serial.print(uJoyValues[nIdxJoy]);

    if (nIdxJoy != JOY_TYPE_END - 1)
      Serial.print(", ");
  }
#endif
}

void print_SWval(uint8_t *u8SwitchValues)
{
#if DBG_PRINT_JOYSTICK_DATA
  Serial.print("|| switchs: ");
  for (uint8_t idx = HW_COMMON_SWITCH_START; idx < HW_COMMON_SWITCH_COUNT_MAX; idx++)
  {
    Serial.print(u8SwitchValues[idx]);
    Serial.print("/");
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  Serial.println(".");
#endif
}
