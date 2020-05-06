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

#include "profiles/SpeedDistanceMonitor.hh"
#include "util.h"
#include <Arduino.h>
SDMPage1::SDMPage1()
{
    time =0;              // Measured as 1/ANT_SDM_TIME_UNIT_REVERSAL seconds (i.e., per 200msec)
                                // SDM time of the last distance and/or speed computation. 
                                // Time starts when SDM is powered ON and continues 
                                // until it is powered OFF. Set to 0x00 when unused.
                                // Rolls over at 256sec. 
    distance =0;          // Measured as 1/ANT_SDM_DISTANCE_UNIT_REVERSAL meters (i.e., per 1/16 meters)
                                // Accumulated distance. Set to 0x00 when unused
                                // Rolls over at 256m. 
    instant_speed =0;     ///Measured as 1/ANT_SDM_SPEED_UNIT_REVERSAL (i.e., per 1/256 m/s)
                                // Instantaneous speed is intended to be appropriately filtered by the SDM, 
                                // such that the receiving unit can directly display this value to the user. 
                                // Set to 0x00 when unused.
    strides =0;            // Accumulated strides. This value is incremented once for every two footfalls.
                                // This is a required field. Rolls over at 256steps
    update_latency =0;     // Measured as 1/ANT_SDM_UPDATE_LATENCY_UNIT_REVERSAL (i.e, 1/32 sec)
                                // The time elapsed between the last speed and distance computation 
                                // and the tran
};

void SDMPage1::Decode(uint8_t const *buffer) {
    ant_sdm_page1_data_layout_t const*  p_incoming_data = (ant_sdm_page1_data_layout_t const * )buffer;

    time= p_incoming_data->time_integer*ANT_SDM_TIME_UNIT_REVERSAL
            +p_incoming_data->time_fractional;
    distance= p_incoming_data->distance_integer*ANT_SDM_DISTANCE_UNIT_REVERSAL
            +p_incoming_data->distance_fractional;
    instant_speed = p_incoming_data->instant_speed_integer*ANT_SDM_SPEED_UNIT_REVERSAL
            +p_incoming_data->instant_speed_fractional;
    strides = p_incoming_data->strides;
    update_latency = p_incoming_data->update_latency*ANT_SDM_UPDATE_LATENCY_UNIT_REVERSAL;
    #if false
    Serial.printf("[Page1.decoding] Time fractional:%d\n", p_incoming_data->time_fractional);
    Serial.printf("[Page1.decoding] Time integer:%d\n", p_incoming_data->time_integer);
    Serial.printf("[Page1.decoding] Distance integer:%d\n", p_incoming_data->distance_integer);
    Serial.printf("[Page1.decoding] Distance fractional:%d\n", p_incoming_data->distance_fractional);
    Serial.printf("[Page1.decoding] Inst. speed integer:%d\n", p_incoming_data->instant_speed_integer);
    Serial.printf("[Page1.decoding] Inst. speed fract:%d\n", p_incoming_data->instant_speed_fractional);
    Serial.printf("[Page1.decoding] Stride count:%d\n", p_incoming_data->strides);
    Serial.printf("[Page1.decoding] Update latency:%d\n", p_incoming_data->update_latency);
    #endif
} ;

void SDMPage1::Encode(uint8_t *buffer) 
   {
    ant_sdm_page1_data_layout_t *  p_outgoing_data = (ant_sdm_page1_data_layout_t  * )buffer;

    p_outgoing_data->time_fractional= UINT8_MAX & (time%ANT_SDM_TIME_UNIT_REVERSAL); // Byte #1
    p_outgoing_data->time_integer= UINT8_MAX & (time / ANT_SDM_TIME_UNIT_REVERSAL); // Byte #2 
    p_outgoing_data->distance_integer = UINT8_MAX & (distance / ANT_SDM_TIME_UNIT_REVERSAL); // Byte #3
    p_outgoing_data->distance_fractional = 0x0F & (distance % ANT_SDM_TIME_UNIT_REVERSAL); //Byte #4 upper nibble
    p_outgoing_data->instant_speed_integer = 0x0F & (instant_speed / ANT_SDM_TIME_UNIT_REVERSAL); //Byte #4 lower nibble
    p_outgoing_data->instant_speed_fractional = UINT8_MAX & ( instant_speed % ANT_SDM_TIME_UNIT_REVERSAL); //Byte #5
    p_outgoing_data->strides = UINT8_MAX & ( strides); //byte #6
    p_outgoing_data->update_latency =  UINT8_MAX & ( update_latency); //byte #7
   }; 

SDMPage2::SDMPage2() {
    cadence         = 0;
    instant_speed   = 0;
    status.flags.state  = inactive;
    status.flags.health = ok;
    status.flags.battery = ok_new;
    status.flags.location = laces;

}
void SDMPage2::Decode(uint8_t const *buffer) {
    ant_sdm_page2_data_layout_t const*  p_incoming_data = (ant_sdm_page2_data_layout_t const * )buffer;

    cadence= p_incoming_data->cadence_integer*ANT_SDM_CADENCE_UNIT_REVERSAL
            +p_incoming_data->cadence_fractional;
    instant_speed = p_incoming_data->instant_speed_integer*ANT_SDM_SPEED_UNIT_REVERSAL
            +p_incoming_data->instant_speed_fractional;
    status.byte = p_incoming_data->status_flags;
    #if false
    Serial.printf("[Page2.decoding] Cadence integer:%d\n", p_incoming_data->cadence_integer);
    Serial.printf("[Page2.decoding] Cadence fractional:%d\n", p_incoming_data->cadence_fractional);
    Serial.printf("[Page2.decoding] Inst. speed inteher:%d\n", p_incoming_data->instant_speed_integer);
    Serial.printf("[Page2.decoding] Inst. speed fract:%d\n", p_incoming_data->instant_speed_fractional);
    Serial.printf("[Page2.decoding] Status:%d\n", p_incoming_data->status_flags);
    Serial.printf("[Page2.decoding] Cadence decoced:%d\n", cadence);
    Serial.printf("[Page2.decoding] Instant speed decoced:%d\n", instant_speed);
    #endif
} ;

void SDMPage2::Encode(uint8_t *buffer) 
   {
    ant_sdm_page2_data_layout_t *  p_outgoing_data = (ant_sdm_page2_data_layout_t  * )buffer;

    p_outgoing_data->reserved0[0]           =UINT8_MAX; // byte 1
    p_outgoing_data->reserved0[1]           =UINT8_MAX; // byte 2 
    p_outgoing_data->cadence_integer        = cadence / ANT_SDM_CADENCE_UNIT_REVERSAL;  // Byte #3
    p_outgoing_data->instant_speed_integer  = 0x0F & (instant_speed / ANT_SDM_SPEED_UNIT_REVERSAL); //Byte #4 lower nibble
    p_outgoing_data->cadence_fractional     = 0x0F & (cadence % ANT_SDM_CADENCE_UNIT_REVERSAL); //Byte #4 upper nibble
    p_outgoing_data->instant_speed_fractional = UINT8_MAX & ( instant_speed % ANT_SDM_SPEED_UNIT_REVERSAL); //Byte #5
    p_outgoing_data->reserved1              = UINT8_MAX ; //byte #6
    p_outgoing_data->status_flags           = status.byte ; //byte #7
   }; 


void SpeedDistanceMonitor::AdjustCumulativeDataWith(SDMPage1* p_page)
{
////ANT+ Device Profile, Stride Based Speed and Distance Monitor, rev. 1.4, sec.6.2.2
    /* cumulative time */
    uint16_t msg_val= p_page->getTime();
    cumulative_time += msg_val-prev_time;
    //times rolls over with 256
    if (prev_time > msg_val) cumulative_time += ANT_SDM_TIME_ROLLOVER*ANT_SDM_TIME_UNIT_REVERSAL;
    prev_time = msg_val;

   /* cumulative distance */
    msg_val= p_page->getDistance();
    cumulative_distance += msg_val-prev_distance;
    //distance rolls over with 256
    if (prev_distance > msg_val) cumulative_distance += ANT_SDM_DISTANCE_ROLLOVER*ANT_SDM_DISTANCE_UNIT_REVERSAL;
    prev_distance = msg_val;

   /* cumulative strides */
    msg_val= p_page->getStrideCount();
    cumulative_strides += msg_val-prev_strides;
    //strides rolls over with 256
    if (prev_strides > msg_val) cumulative_strides += ANT_SDM_STRIDE_ROLLOVER;
    prev_strides = msg_val;

};

SpeedDistanceMonitor ::SpeedDistanceMonitor (ANTTransmissionMode mode) : ANTProfile(mode)
{
    prev_time=0;
    prev_distance=0;
    prev_strides=0;
    cumulative_time=0;
    cumulative_distance=0;
    cumulative_strides=0;

    current_distance=0;
    current_strides=0;
    current_time=0;

    m_disp_config.channel_type  = SDM_DISP_CHANNEL_TYPE;
    m_disp_config.ext_assign    = SDM_EXT_ASSIGN;
    m_disp_config.rf_freq       = SDM_ANTPLUS_RF_FREQ;
    m_disp_config.transmission_type = 0;
    m_disp_config.device_type   = SDM_DEVICE_TYPE;
    m_disp_config.device_number = 0;
    m_disp_config.channel_period    = SDM_MSG_PERIOD_2Hz;
    m_disp_config.network_number    = ANTPLUS_NETWORK_NUMBER;

    m_channel_sens_config.channel_type  = SDM_SENS_CHANNEL_TYPE;
    m_channel_sens_config.ext_assign    = SDM_EXT_ASSIGN;
    m_channel_sens_config.rf_freq       = SDM_ANTPLUS_RF_FREQ;
    m_channel_sens_config.transmission_type = 0x05; //213; //Transmission Type Bits 0:3 - 5 (0x05) ANT+ devices will follow the transmission type definition as outlined in the ANT protocol: The lower nibble shall be set to 0x05. The upper nibble may be used to extend the device number
    m_channel_sens_config.device_type   = SDM_DEVICE_TYPE;
    m_channel_sens_config.device_number = DeviceNumber;
    m_channel_sens_config.channel_period    = SDM_MSG_PERIOD_2Hz;
    m_channel_sens_config.network_number    = ANTPLUS_NETWORK_NUMBER;

    _OnStrideCountData_cb = NULL;
    _OnCadenceData_cb = NULL;
    _OnSpeedData_cb = NULL; 
} ;


//SpeedDistanceMonitor ::~SpeedDistanceMonitor () {} ;

void SpeedDistanceMonitor::DecodeMessage(uint8_t* buffer)
{
   ant_sdm_message_layout_t* p_env_message_payload = (ant_sdm_message_layout_t *)buffer;
    #if false
    Serial.printf("[SDM] Decoding page #%d:", p_env_message_payload->page_number);
    for (uint8_t i = 0; i < 7; i++)
    {
        Serial.printf("[%02x]", p_env_message_payload->page_payload[i]);
    }
    Serial.println("");
    #endif 

   switch (p_env_message_payload->page_number)
   {
   case ANT_SDM_DATA_PAGE1:
      page1.Decode(p_env_message_payload->page_payload);
      getPacePerKm(page1.getSpeed(), &pace_min, &pace_sec);
      AdjustCumulativeDataWith(&page1);
      #if false
      Serial.printf("Time (1/200s):%d &  in msec:%d\n", page1.getTime(), page1.getTimeInMsec());
      Serial.printf("Distance (1/16m):%d &  in cm:%d\n", page1.getDistance(), page1.getDistanceInCm());
      Serial.printf("Inst. speed (1/256 m/s):%d & in cm/s:%d\n", page1.getSpeed(), page1.getSpeedInCmps());
      Serial.printf("Strides :%d\n", page1.getStrideCount());
      Serial.printf("Update latency (1/32 s):%d and in msec:%d\n", page1.getLatency(), page1.getLatencyinMsec());
      #endif
      if (_OnStrideCountData_cb != NULL) 
      {
         _OnStrideCountData_cb(page1.getStrideCount());
      }
      if (_OnSpeedData_cb != NULL) 
      {
         _OnSpeedData_cb(page1.getSpeedInKmph());
      }
      break;
   case ANT_SDM_DATA_PAGE2:
      page2.Decode(p_env_message_payload->page_payload);
      getPacePerKm(page2.getSpeed(), &pace_min, &pace_sec);
      #if false
      Serial.printf("[Page2] Cadence (1/16 spm):%d & in (1/10 spm):%d \n", page2.getCadence(), page2.getCadenceInTenthSpm());
      Serial.printf("[Page2] Cadence (1spm):%d  \n", page2.getCadenceInSpm());
      Serial.printf("[Page2] Inst.speed (1/256 m/s):%d & in (cm/s):%d \n", page2.getSpeed(), page2.getSpeedInCmps());
      page2.getPacePerKm(min, sec);
      Serial.printf("[Page2] Pace: %d:%d min/km\n", min,sec);
      Serial.printf("[Page2] Battery:%d\n", page2.getSdmBatteryStatus());
      Serial.printf("[Page2] Health:%d\n", page2.getSdmHealth());
      Serial.printf("[Page2] Location:%d\n", page2.getSdmLocation());
      Serial.printf("[Page2] Use:%d\n", page2.getSdmUseState());
      #endif
      if (_OnSpeedData_cb != NULL) 
      {
         _OnSpeedData_cb(page2.getSpeedInKmph());
      }
      if (_OnCadenceData_cb != NULL) 
      {
         _OnCadenceData_cb(page2.getCadenceInSpm());
      }
      if (_OnPaceData_cb != NULL) 
      {
        _OnPaceData_cb(pace_min, pace_sec);
      }
      break;

   default:
      break;
   }
};

void SpeedDistanceMonitor::EncodeMessage() {
 //todo
}

