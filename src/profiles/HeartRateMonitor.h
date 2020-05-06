/*
(Modified) MIT License

Copyright (c) 2020 Ryan Green.
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

#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include <stdint.h>

#include "../ANTProfile.h"

#define HRM_DEVICE_TYPE       0x78u     ///< Device type reserved for ANT+ heart rate monitor.
#define HRM_ANTPLUS_RF_FREQ   0x39u     ///< Frequency, decimal 57 (2457 MHz).

#define HRM_MSG_PERIOD_4Hz    0x1F86u   ///< Message period, decimal 8070 (4.06 Hz).
#define HRM_MSG_PERIOD_2Hz    0x3F0Cu   ///< Message period, decimal 16140 (2.03 Hz).
#define HRM_MSG_PERIOD_1Hz    0x7E18u   ///< Message period, decimal 32280 (1.02 Hz).

#define HRM_EXT_ASSIGN        0x00                  ///< ANT ext assign.
#define HRM_DISP_CHANNEL_TYPE CHANNEL_TYPE_SLAVE    ///< Display HRM channel type.
#define HRM_SENS_CHANNEL_TYPE CHANNEL_TYPE_MASTER   ///< Sensor HRM channel type.


class HRMPage0
{
public:
   HRMPage0();

   uint8_t GetBeatCount() { return beat_count; };
   void SetBeatCount(uint8_t val) { beat_count = val; }

   uint8_t GetComputedHeartRate() { return computed_heart_rate; };
   void SetComputedHeartRate(uint8_t val) { computed_heart_rate = val; }

   uint16_t GetBeatTime() { return beat_time; };
   void SetBeatTime(uint16_t val) { beat_time = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint8_t  beat_count;          ///< Beat count.
    uint8_t  computed_heart_rate; ///< Computed heart rate.
    uint16_t beat_time;           ///< Beat time.

   /**@brief HRM page 0 data layout structure. */
   typedef struct
   {
      uint8_t reserved[3];
      uint8_t heart_beat_evt_time_LSB;
      uint8_t heart_beat_evt_time_MSB;
      uint8_t heart_beat_count;
      uint8_t computed_heart_rate;
   }ant_hrm_page0_data_layout_t;

};

class HRMPage1
{
public:
   HRMPage1();

   uint8_t GetOperatingTime() { return operating_time; };
   void SetOperatingTime(uint8_t val) { operating_time = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint32_t operating_time; ///< Operating time.

   /**@brief HRM page 1 data layout structure. */
   typedef struct
   {
      uint8_t cumulative_operating_time[3];
      uint8_t reserved[4];
   }ant_hrm_page1_data_layout_t;

};

class HRMPage2
{
public:
   HRMPage2();

   uint8_t GetManufacturerID() { return manuf_id; };
   void SetManufacturerID(uint8_t val) { manuf_id = val; }

   uint16_t GetSerialNumber() { return serial_num; };
   void SetSerialNumber(uint16_t val) { serial_num = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
   uint8_t manuf_id; ///< Manufacturer ID.
   uint16_t serial_num; ///< Serial number.

   /**@brief HRM page 2 data layout structure. */
   typedef struct
   {
      uint8_t manuf_id;
      uint8_t serial_num_LSB;
      uint8_t serial_num_MSB;
      uint8_t reserved[4];
   }ant_hrm_page2_data_layout_t;

};

class HRMPage3
{
public:
   HRMPage3();

   uint8_t GetHardwareVersion() { return hw_version; };
   void SetHardwareVersion(uint8_t val) { hw_version = val; }

   uint8_t GetSoftwareVersion() { return sw_version; };
   void SetSoftwareVersion(uint8_t val) { sw_version = val; }

   uint8_t GetModelNumber() { return model_num; };
   void SetModelNumber(uint8_t val) { model_num = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
   uint8_t hw_version; ///< Hardware version.
   uint8_t sw_version; ///< Software version.
   uint8_t model_num;  ///< Model number.

   /**@brief HRM page 3 data layout structure. */
   typedef struct
   {
      uint8_t hw_version;
      uint8_t sw_version;
      uint8_t model_num;
      uint8_t reserved[4];
   }ant_hrm_page3_data_layout_t;

};

class HRMPage4
{
public:
   HRMPage4();

   uint8_t GetManufacturerSpecificByte() { return manuf_spec; };
   void SetManufacturerSpecificByte(uint8_t val) { manuf_spec = val; }

   uint16_t GetPreviousBeat() { return prev_beat; };
   void SetPreviousBeat(uint16_t val) { prev_beat = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
   uint8_t  manuf_spec; ///< Manufacturer specific byte.
   uint16_t prev_beat;  ///< Previous beat.

   /**@brief HRM page 4 data layout structure. */
   typedef struct
   {
      uint8_t manuf_spec;
      uint8_t prev_beat_LSB;
      uint8_t prev_beat_MSB;
      uint8_t reserved[4];
   }ant_hrm_page4_data_layout_t;

};

class HeartRateMonitor : public ANTProfile
{
public:
   HeartRateMonitor(ANTTransmissionMode mode);

   void ProcessMessage(ant_evt_t*);

   void SetOnComputedHeartRate(void (*fp)(int)) { _OnComputedHeartRate_cb = fp;}


private:

   static const uint8_t DeviceNumber = 49;

   /**@brief HRM page number type. */
   typedef enum
   {
      ANT_HRM_PAGE_0, ///< Main data page number 0.
      ANT_HRM_PAGE_1, ///< Background data page number 1. This page is optional.
      ANT_HRM_PAGE_2, ///< Background data page number 2.
      ANT_HRM_PAGE_3, ///< Background data page number 3.
      ANT_HRM_PAGE_4  ///< Main data page number 4.
   } ant_hrm_page_t;

   /**@brief HRM message data layout structure. */
   typedef struct
   {
      ant_hrm_page_t page_number         : 7;
      uint8_t        toggle_bit          : 1;
      uint8_t        page_payload[7];
   } ant_hrm_message_layout_t;

   HRMPage0 page0;
   HRMPage1 page1;
   HRMPage2 page2;
   HRMPage3 page3;
   HRMPage4 page4;



   void (*_OnComputedHeartRate_cb) (int);

   ant_hrm_page_t GetNextPageNumber();

   void EncodeMessage();
   void DecodeMessage(uint8_t* p_message_payload);


    uint8_t        toggle_bit;
    ant_hrm_page_t main_page_number;
    uint8_t        page_1_present;
    ant_hrm_page_t ext_page_number;
    uint8_t        message_counter;

};



#endif