/*(Modified) MIT License
Copyright (c) 2020 Gábor Ziegler and other contributors
Portions of this repo contains sourcecode either inspired by or copied from 
published code from Adafruit Industries, from thisisant.com and from 
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
// This example shows a Radar display
// The sensor RX channels are (re)open(ed) perpetually.
// Logging is done both to BLEUart and to Serial

#include "ANTProfile.h"
#include "profiles/Radar.h"
#include "sdant.h"

#include <Adafruit_NeoPixel.h>

#include <bluefruit.h>
//#include <bluefruit52.h>
//#include <Adafruit_LittleFS.h>
//#include <InternalFileSystem.h>

// Adafruit Feather M4 Express uses pin 8 for its built-in  NeoPixel.
#define NEOPIXEL_PIN 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// BLE Service
//BLEDfu  bledfu;  // OTA DFU service
BLEDis bledis;   // device information
BLEUart bleuart; // uart over ble

//fwd dcls
// callback invoked when central connects
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void startAdv(void);


Radar radar(RX);

void _OnVehicleStatusChanged(bool hasVehicles)
{
  if (hasVehicles) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("!!! OH NO! CARS");
    bleuart.println("!!! OH NO! CARS");

    strip.setPixelColor(0, 0xFF, 0, 0);
    strip.show();
} else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("OH, NO CARS <3");
    bleuart.println("OH, NO CARS <3");

    strip.setPixelColor(0, 0, 0xFF, 0);
    strip.show();
  }
}
void _OnThreatDataUpdated(int page, ant_radar_threats_t threats[4])
{
  for (int i = 0; i < RADAR_NUM_THREATS_PER_PAGE; i ++ ) {
  Serial.printf("Radar page %d threat %d level %d side %d range %d speed %d\r\n", page, i, threats[i].level, threats[i].side, threats[i].range_meters, threats[i].speed_mps);
  bleuart.printf("Radar page %d threat %d level %d side %d range %d speed %d\n", page, threats[i].level, threats[i].side, threats[i].range_meters, threats[i].speed_mps);
  }
}
void _OnStatusUpdated(ant_radar_status_t status)
{
  Serial.printf("Radar status %d\r\n", status);
  bleuart.printf("Radar status %d\n", status);
}
void _OnVendorInfoUpdated(ant_radar_vendor_info_t info)
{
  Serial.printf("Radar info hw_revision %d manufacturer id %d model number %d\r\n", info.hw_revision, info.manufacturer_id, info.model_number);
  bleuart.printf("Radar info hw_revision %d manufacturer id %d model number %d\n", info.hw_revision, info.manufacturer_id, info.model_number);
}
void _OnDeviceInfoUpdated(ant_radar_device_info_t info)
{
  Serial.printf("Radar info version %f serial %u\r\n", info.version, info.serial);
  bleuart.printf("Radar info version %f serial %u\n", info.version, info.serial);
}

void PrintUnhandledANTEvent(ant_evt_t *evt)
{
  Serial.printf("Channel #%d for %s: event %s", evt->channel, ANTplus.getAntProfileByChNum(evt->channel)->getName(), AntEventTypeDecode(evt)); 
  if (evt->event != EVENT_CHANNEL_COLLISION 
    && evt->event != EVENT_RX_FAIL
    && evt->event != EVENT_CHANNEL_CLOSED
    )
    Serial.printf("  (%s)", AntEventType2LongDescription(evt));
    Serial.println();

    if (evt->event == EVENT_CHANNEL_CLOSED) {
          digitalWrite(LED_BUILTIN, LOW);
          strip.setPixelColor(0, 0, 0, 0);
          strip.show();
    }
}
void ReopenANTChannel(ant_evt_t *evt)
{
  if (evt->event == EVENT_CHANNEL_CLOSED ) {
    Serial.printf("Channel #%d closed for %s\r\n", evt->channel,ANTplus.getAntProfileByChNum(evt->channel)->getName()); 
    Serial.printf("Reopening...");
    uint32_t ret=sd_ant_channel_open(evt->channel);
    if (ret == NRF_SUCCESS) Serial.println("success!");
    else Serial.printf("failed with code:%#x\r\n", ret);
  }
}


void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  strip.begin();
  strip.setBrightness(80);
  strip.show();

  bool ret = false;
  Serial.begin(115200);
  while (!Serial) yield;

  Serial.println("Adding Radar profile");
  radar.SetOnVehicleStatusChanged(_OnVehicleStatusChanged);
//  radar.SetOnThreatDataUpdated(_OnThreatDataUpdated);
  radar.SetOnStatusUpdated(_OnStatusUpdated);
  radar.SetOnVendorInfoUpdated(_OnVendorInfoUpdated);
  radar.SetOnDeviceInfoUpdated(_OnDeviceInfoUpdated);
  radar.setUnhandledEventListener(PrintUnhandledANTEvent);
  radar.setAllEventListener(ReopenANTChannel);
  radar.setName("RDR");
  ANTplus.AddProfile(&radar);

  //ble on
  Serial.println("Bluefruit52 BLEUART Startup");
  Serial.println("---------------------------\n");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_NORMAL);

  Serial.print("Starting BLE stack. Expecting 'true':");
  ret = Bluefruit.begin(1, 0);
  Serial.println(ret);
  Serial.print("Starting ANT stack. Expecting 'true':");
  ret = ANTplus.begin(3);
  Serial.println(ret);
  
  ANTProfile* profiles[] = {&radar};
  for (auto i: profiles) {
    Serial.printf("Channel number for %s became %d\r\n", i->getName(), i->getChannelNumber());
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
} //loop
