/*(Modified) MIT License

Copyright (c) 2020 Gábor Ziegler and other contributors

Portions of this repo contains sourcecode either inspired by or copied from 
published code from Adafruit Industires, from thisisant.com and from 
Nordic Semiconductor ASA. The main inputs were:
* Adafruit_nRF52_Arduino repo and various public forks of that (LGPL License)
* The nRF5 SDK by Nordic Semiconductor (a mashup of licenses)
* Various ANT+ software from thisisant.com

The license conditions of particular files can be found in the top of the 
individual files. The TL/DR summary of the restrictions beyond the usual 
 MIT license:
* This software, with or without modification, must only be used with a
  Nordic Semiconductor ASA integrated circuit.
* The user if this software, with or without modification, must comply with
  the ANT licensing terms: https://www.thisisant.com/developer/ant/licensing.
  (Note particluarly that the said ANT license permits only non-commercial, 
  non revenue-generating usage without paying a yearly license fee.)

The rest of this library, which are original contributions or
derivative works falls under the MIT license. 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. The notifications about the 
legal requirements of adhering to the Nordic Semiconductor ASA and the
thisiant.com licensing terms shall be included.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

///////////////////////////////////////////////////////////////7
// This example shows a 3 sensors display: HRM, TEMPE and SDM.
// The sensor RX channels are (re)open(ed) perpetually.
// Logging is done both to BLEUart and to Serial
// Also, LiPo battery level is measured and advertised via Bluetooth
// for an assumed nRF52840 Feather Express board 

//TODO: code cleanup, proper commenting
//TODO: this is an ugly mixture of C and C++ style coding. A proper style is needed.

#include "ANTProfile.h"
#include "profiles/HeartRateMonitor.h"
#include "profiles/Environment.hh"
#include "profiles/SpeedDistanceMonitor.hh"
#include "sdant.h"


#include <bluefruit.h>
//#include <bluefruit52.h>
//#include <Adafruit_LittleFS.h>
//#include <InternalFileSystem.h>

// BLE Service
//BLEDfu  bledfu;  // OTA DFU service
BLEDis bledis;   // device information
BLEUart bleuart; // uart over ble
BLEBas blebas;   // battery

#define VBATPIN A6 //BAT battery voltage
#define VUSBPIN A0 //USB USB-voltage/2
float measuredvbat = 0.0, measuredvbat_old = 0.0;
char oldvoltage[4] = "0.0", newvoltage[4] = "0.0";
uint8_t batlevel = 0;

//fwd dcls
// callback invoked when central connects
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void startAdv(void);


HeartRateMonitor hrm(RX);
EnvironmentSensor env(RX);
SpeedDistanceMonitor sdm(RX);

uint8_t currHR= 0; 
float currATEMP=0;
uint8_t currCAD=0;

void _OnComputedHeartRate(int rate)
{
  Serial.printf("HRM computed rate: %d\n", rate);
  bleuart.printf("HRM: %d\n", rate);
  currHR=rate;
}
void _OnTemperature(int16_t temp)
{
  Serial.printf("Current temperature: %f\n", temp / 100.0);
  bleuart.print("Temp:");
  bleuart.println(temp / 100.0);
  currATEMP = temp/100.0;
}
void _OnSpeed(float speed)
{
  Serial.printf("Speed:  %f km/h\n", speed);

  bleuart.print("Speed (km/h):");
  bleuart.println(speed);
}
void _OnCadence(uint8_t cadence)
{
  Serial.printf("Cadence:  %d\n", cadence);
  bleuart.print("Cadence:");
  bleuart.println(cadence);
  currCAD=cadence;
}
void _OnStride(int16_t stride)
{
  Serial.printf("Stride count:  %d\n", stride);
  bleuart.print("Stride:");
  bleuart.println(stride);
}
void _OnPace(uint8_t min, uint8_t sec)
{
  Serial.printf("Pace:  %02d:%02d min/km\n", min, sec);
  bleuart.printf("Pace: %02d:%02d min/km\n", min, sec);
}
void PrintUnhandledANTEvent(ant_evt_t *evt)
{
  Serial.printf("Channel #%d for %s: event %s\n", evt->channel, ANTplus.getAntProfileByChNum(evt->channel)->getName(), AntEventTypeDecode(evt)); 
  if (evt->event != EVENT_CHANNEL_COLLISION 
    && evt->event != EVENT_RX_FAIL
    && evt->event != EVENT_CHANNEL_CLOSED
    )
    Serial.printf("  (%s)\n", AntEventType2LongDescription(evt));
}
void ReopenANTChannel(ant_evt_t *evt)
{
  if (evt->event == EVENT_CHANNEL_CLOSED ) {
    Serial.printf("Channel #%d closed for %s\n", evt->channel,ANTplus.getAntProfileByChNum(evt->channel)->getName()); 
    Serial.printf("Reopening...");
    uint32_t ret=sd_ant_channel_open(evt->channel);
    if (ret == NRF_SUCCESS) Serial.println("success!");
    else Serial.printf("failed with code:%#x\n", ret);
  }
}


void setup(void)
{
  bool ret = false;
  Serial.begin(115200);
  delay(5000);
  //while (!Serial);
  Serial.println("Adding HRM profile");
  hrm.SetOnComputedHeartRate(_OnComputedHeartRate);
  hrm.setUnhandledEventListener(PrintUnhandledANTEvent);
  hrm.setAllEventListener(ReopenANTChannel);
  hrm.setName("HRM");
  ANTplus.AddProfile(&hrm);

  Serial.println("Adding ENV profile");
  env.SetOnTemperatureData(_OnTemperature);
  env.setUnhandledEventListener(PrintUnhandledANTEvent);
  env.setAllEventListener(ReopenANTChannel);
  env.setName("TEMPE");
  ANTplus.AddProfile(&env);

  Serial.println("Adding SDM profile");
  sdm.setOnCadenceData(_OnCadence);
  sdm.setOnSpeedData(_OnSpeed);
  sdm.setOnStrideCountData(_OnStride);
  sdm.setOnPaceData(_OnPace);
  sdm.setUnhandledEventListener(PrintUnhandledANTEvent);
  sdm.setAllEventListener(ReopenANTChannel);
  sdm.setName("Footpod");
  ANTplus.AddProfile(&sdm);

  //ble on
  Serial.println("Bluefruit52 BLEUART Startup");
  Serial.println("---------------------------\n");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_NORMAL);

  Serial.print("Starting BLE stack. Expecting 'true':");
  ret = Bluefruit.begin(1, 0);
  Serial.println(ret);
  Serial.print("Starting ANT stack. Expecting 'true':");
  ret= ANTplus.begin(3) ;
  Serial.println(ret);
  
  ANTProfile* profiles[] = {&hrm, &env, &sdm};
  for (auto i: profiles) {
    Serial.printf("Channel number for %s became %d\n", i->getName(), i->getChannelNumber());
  }


  Bluefruit.setTxPower(5); // Check bluefruit.h for supported values
  Bluefruit.setName("Bluefruit52 S340");
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  //bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52 S340");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  Serial.println("Setup is finished!");
}
void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
  Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}
// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection *connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = {0};
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void)conn_handle;
  (void)reason;

  Serial.println();
  Serial.println("Disconnected");
}

void loop(void)
{
      // Measuring LiPo  battery
      measuredvbat = analogRead(VBATPIN);
      measuredvbat *= 2;    // we divided by 2, so multiply back
      measuredvbat *= 3.6;  // Multiply by 3.3V, our reference voltage
      measuredvbat /= 1024; // convert to voltage
      sprintf(newvoltage, "%1.1f", measuredvbat);
      batlevel = (uint8_t)100 * (measuredvbat - 3.5) / (4.2 - 3.5); //0%=3.5V, 100%=4.2
      if (0 != strcmp(newvoltage, oldvoltage))
      {
        blebas.write(batlevel);
      } //newvoltage
} //loop