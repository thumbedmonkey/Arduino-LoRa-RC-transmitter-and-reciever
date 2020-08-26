
/**************************************************************************************************
  Transmitter master mcu code for Atmega328p

  This code reads stick inputs, handles User Interface, outputs via serial to a second mcu chip
  that handles the actual transmission via Rf module to the receiver.
  See the schematics.
  buk7456@gmail.com   2019 - 2020
  
  Released under the MIT Licence
  Tested to compile on Arduino IDE 1.8.9 or later
  Sketch should compile without warnings even with -WALL. Only warning is the Low memory.
  
  FREE RAM AFTER COMPILE SHOULD BE GREATER THAN 340 BYTES !
***************************************************************************************************/

#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <SPI.h>

#include "GFX.h"

#include "config.h"
#include "common.h"
#include "io.h"
#include "eestore.h"
#include "bitmaps.h"
#include "ui_128x64.h" 

// Declarations 
void sendToSlaveMCU(); //sends radio data to slave mcu
void serialWrite14bit(uint16_t value); //helper
void checkBattery();

//===================================== setup ======================================================
void setup()
{
  pinMode(ROW1_MTRX_PIN, INPUT);
  pinMode(ROW2_MTRX_PIN, INPUT);
  pinMode(COL1_MTRX_PIN, OUTPUT);
  pinMode(COL2_MTRX_PIN, OUTPUT);
  pinMode(COL3_MTRX_PIN, OUTPUT);

  Serial.begin(UARTBAUDRATE);
  delay(200);

  display.begin();
  display.setTextWrap(false);
  

  /********* EEPROM init **************
    Check for flag. If flag value is not what we expect, overwrite values in eeprom
    with defaults (initial values of certain global variables), Then write flag.
  */
  uint8_t eeCheckMark = EEPROM.read(_EEPROM_INIT);
  if (eeCheckMark != EE_INITFLAG)
  {
    DrawEEWarning();
    while(buttonCode == 0)
    {
      readSwitchesAndButtons();
      delay(30);
    }
    DrawEEFormatMsg();
    eeUpdateSysConfig();
    for (uint8_t mdlNo = 1; mdlNo <= 6; mdlNo++)
    {
      eeUpdateModelBasicData(mdlNo);
      eeUpdateMixData(mdlNo);
      eeUpdateModelName(mdlNo);
    }
    EEPROM.write(_EEPROM_INIT, EE_INITFLAG);
    delay(1000);
    
    changeToScreen(MODE_CALIB); //Set to sticks calib screen

    buttonCode = 0;    
  }

  ///--------- Load data from eeprom -----------
  DrawLoadingMsg();
  delay(500);
  eeReadSysConfig();
  eeReadModelBasicData(activeModel);
  eeReadMixData(activeModel); 
  eeReadModelName(activeModel);
  
  battVoltsNow = battVoltsMax; 
  
  ///--------- Override sounds mode if up key held while starting up ----------
  readSwitchesAndButtons();
  if(buttonCode == UP_KEY)
  {
    soundMode = SOUND_OFF;
    eeUpdateSysConfig();
  }
  
  ///----------Warn throttle if throttle position is more than 5% above minimum---
  readSwitchesAndButtons();
  readSticks();
  bool _rfState = rfModuleEnabled; //temporarily store rf state as we are going to override it
  while (throttleIn > -450 && SwAEngaged == false)
  {
    DrawThrottleWarning();
    readSwitchesAndButtons();
    readSticks();
    //play warning sound
    audioToPlay = AUDIO_THROTTLEWARN;
    //override rfModuleEnabled before sending to slave
    rfModuleEnabled = false;
    sendToSlaveMCU();
    delay(30);
  }
  rfModuleEnabled = _rfState; //restore original value before override

  throttleTimerLastPaused = millis(); 
}

//===================================== main =======================================================
void loop()
{
  unsigned long loopStartTime = millis();
  
  thisLoopNum++;
  
  /// -------- CHECk BATTERY --------------------
  checkBattery();

  /// --------- CORE FUNCTIONS ------------------
  readSwitchesAndButtons();
  readSticks();
  computeChannelOutputs();
 
  /// -------- USER INTERFACE -------------------
  HandleMainUI();
  
  /// -------- SEND CHANNEL DATA AND OTHER STATUSES VIA UART TO SLAVE MCU -----
  sendToSlaveMCU();

  ///--------- READ RETURNED BYTE ---------------
  //Slave mcu only sends back this parameter
  while (Serial.available() > 0)
  {
    returnedByte = Serial.read();
  }

  /// ---------- LIMIT MAX RATE OF LOOP ---------
  // Limited to around 35 loops a second. Changing this might affect some stuff. Making this any
  // faster could result in overwhelming the secondary mcu thus leading to lost data
  unsigned long loopTime = millis() - loopStartTime;
  if(loopTime < 28UL)
    delay(28UL - loopTime);
  
  // -- Debug info --
  // display.setCursor(0,57);
  // display.print(loopTime);
}

//==================================================================================================

void sendToSlaveMCU()
{
  /** Master to Slave MCU communication format as below
  
  byte 0 - Start of message 0xBB
  byte 1 - Status byte 
      bit7 - always 0
      bit6 - Backlight 1 on, 0 off
      bit5 - Battery status, 1 healthy, 0 low
      bit4 - RF module, 1 on, 0 off 
      bit3 - DigChA, 1 on, 0 off
      bit2 - DigChB, 1 on, 0 off
      bit1 - PWM mode for Channel3, 1 means servo pwm, 0 ordinary pwm
      bit0 - Failsafe, 1 means failsafe data, 0 normal data
      
  byte 2 to 17 - Ch1 thru 8 data (2 bytes per channel, total 16 bytes)
  byte 18- Sound to play  (1 byte)
  byte 19- End of message 0xDD
  */
  
  /// ---- message start
  Serial.write(0xBB); 
  
  /// ---- status byte
  
  uint8_t status = 0x00;
  
  static unsigned long lastBtnDownTime = 0;
  if(buttonCode > 0) 
    lastBtnDownTime = millis();
  unsigned long elapsed = millis() - lastBtnDownTime;
  if(backlightMode == BACKLIGHT_ON 
     || (backlightMode == BACKLIGHT_5S  && elapsed < 5000UL )
     || (backlightMode == BACKLIGHT_15S && elapsed < 15000UL)
     || (backlightMode == BACKLIGHT_60S && elapsed < 60000UL)) 
    status |= 0x40;
  
  status |= (battState & 0x01) << 5;
  status |= (rfModuleEnabled & 0x01) << 4;
  status |= DigChA << 3;
  status |= DigChB << 2;
  status |= PWM_Mode_Ch3 << 1;
  
  bool isFailsafeData = false; 
  if(thisLoopNum % 64 == 1) 
    isFailsafeData = true;

  status |= isFailsafeData & 0x01;
  
  Serial.write(status); 
  
  /// ---- channel data and failsafes
  if(isFailsafeData == false) 
  {
    for(uint8_t i = 0; i < 8; i++)
    {
      uint16_t val = (ChOut[i] + 500) & 0xFFFF; 
      serialWrite14bit(val);
    }
  }
  else //send failsafe
  {
    for(uint8_t i = 0; i < 8; i++)
    {
      if(Failsafe[i] == 0) //failsafe not specified, send 1023
        serialWrite14bit(1023);
      else //failsafe specified, send it
      {
        uint16_t fsf = (Failsafe[i] - 1) * 5;
        uint16_t lowerLim = (100 - EndpointL[i]) * 5;
        uint16_t upperLim = (100 + EndpointR[i]) * 5;
        if(fsf < lowerLim)  fsf = lowerLim;
        else if(fsf > upperLim) fsf = upperLim;
        serialWrite14bit(fsf);
      }
    }
  }

  /// ---- sounds
  if((soundMode == SOUND_OFF)
      || (soundMode == SOUND_ALARMS && (audioToPlay == AUDIO_KEYTONE || audioToPlay == AUDIO_SWITCHMOVED))
      || (soundMode == SOUND_NOKEY && audioToPlay == AUDIO_KEYTONE))
    audioToPlay = AUDIO_NONE; 

  Serial.write(audioToPlay); 
  audioToPlay = AUDIO_NONE; //set to none
  
  /// ---- end of message
  Serial.write(0xDD); 
}

void serialWrite14bit(uint16_t value)
{
  // The data is sent as a "14 bit" value (Two bytes) as 0b0LLLLLLL 0b0HHHHHHH
  Serial.write(value & 0x7f);
  Serial.write((value >> 7) & 0x7f);
}

void checkBattery()
{
  /* Apply smoothing to measurement. Cant afford to store any data points due to limited RAM,
  so we cant use the usual moving average method. Instead we implement exponential recursive
  smoothing as described here
  https://www.megunolink.com/articles/coding/3-methods-filter-noisy-arduino-measurements/
  It works by subtracting out the mean each time, and adding in a new point. */
  
  enum { _NUM_SAMPLES = 30 };
  /*_NUM_SAMPLES parameter defines number of samples to average over. Higher value results in slower
  response.
  Formula x = x - x/n + a/n  */
  
  long anaRd = ((long)analogRead(BATTVOLTSPIN) * battVfactor) / 100;
  long battV = ((long)battVoltsNow * (_NUM_SAMPLES - 1) + anaRd) / _NUM_SAMPLES; 
  battVoltsNow = int(battV); 
  
  //add some hysterisis to battState
  if (battState == _BATTGOOD && battVoltsNow <= battVoltsMin)
    battState = _BATTLOW;
  else if (battState == _BATTLOW && battVoltsNow > (battVoltsMin + 100)) //100mV hysteris
    battState = _BATTGOOD;
}
