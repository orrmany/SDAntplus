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

#include "BicycleSpeedCadence.h"
#include "../util.h"
#include <Arduino.h>

BSCPage0::BSCPage0() :
   event_time(0),
   rev_count(0)
{
}

void BSCPage0::Decode(uint8_t const* buffer)
{
    ant_bsc_page0_data_layout_t const * p_incoming_data = (ant_bsc_page0_data_layout_t *)buffer;

    event_time = (uint16_t)((p_incoming_data->bsc_evt_time_MSB << 8) + p_incoming_data->bsc_evt_time_LSB);
    rev_count = (uint16_t) ((p_incoming_data->bsc_rev_count_MSB << 8) + p_incoming_data->bsc_rev_count_LSB);
}

void BSCPage0::Encode(uint8_t* buffer)
{
    ant_bsc_page0_data_layout_t * p_outcoming_data = (ant_bsc_page0_data_layout_t *) buffer;

    p_outcoming_data->reserved[0]       = UINT8_MAX;
    p_outcoming_data->reserved[1]       = UINT8_MAX;
    p_outcoming_data->reserved[2]       = UINT8_MAX;
    p_outcoming_data->bsc_evt_time_LSB  = (uint8_t)(event_time & UINT8_MAX);
    p_outcoming_data->bsc_evt_time_MSB  = (uint8_t)((event_time >> 8) & UINT8_MAX);
    p_outcoming_data->bsc_rev_count_LSB = (uint8_t)(rev_count & UINT8_MAX);
    p_outcoming_data->bsc_rev_count_MSB = (uint8_t)((rev_count >> 8) & UINT8_MAX);
}


BSCPage1::BSCPage1() :
   operating_time(0)
{
}

void BSCPage1::Decode(uint8_t const* buffer)
{
    ant_bsc_page1_data_layout_t const * p_incoming_data = (ant_bsc_page1_data_layout_t *)buffer;

    operating_time          = uint24_decode(p_incoming_data->cumulative_operating_time);
}

void BSCPage1::Encode(uint8_t* buffer)
{
    ant_bsc_page1_data_layout_t * p_outcoming_data = (ant_bsc_page1_data_layout_t *)buffer;

    UNUSED_PARAMETER(uint24_encode(operating_time, p_outcoming_data->cumulative_operating_time));
}


BSCPage2::BSCPage2() :
   manuf_id(0),
   serial_num(0)
{
}

void BSCPage2::Decode(uint8_t const* buffer)
{
    ant_bsc_page2_data_layout_t const * p_incoming_data = (ant_bsc_page2_data_layout_t *)buffer;

    serial_num = (uint32_t)((p_incoming_data->serial_num_MSB << 8) + p_incoming_data->serial_num_LSB);
    manuf_id   = (uint32_t)p_incoming_data->manuf_id;
    serial_num = serial_num;
}

void BSCPage2::Encode(uint8_t* buffer)
{
    ant_bsc_page2_data_layout_t * p_outcoming_data = (ant_bsc_page2_data_layout_t *)buffer;

    p_outcoming_data->manuf_id       = (uint8_t)manuf_id;
    p_outcoming_data->serial_num_LSB = (uint8_t)(serial_num & UINT8_MAX);
    p_outcoming_data->serial_num_MSB = (uint8_t)((serial_num >> 8) & UINT8_MAX);
}

BSCPage3::BSCPage3() :
   hw_version(0),
   sw_version(0),
   model_num(0)
{
}

void BSCPage3::Decode(uint8_t const* buffer)
{
   ant_bsc_page3_data_layout_t const * p_incoming_data = (ant_bsc_page3_data_layout_t *)buffer;

   hw_version = (uint32_t)p_incoming_data->hw_version;
   sw_version = (uint32_t)p_incoming_data->sw_version;
   model_num  = (uint32_t)p_incoming_data->model_num;
}

void BSCPage3::Encode(uint8_t* buffer)
{
   ant_bsc_page3_data_layout_t * p_outcoming_data = (ant_bsc_page3_data_layout_t *)buffer;

   p_outcoming_data->hw_version = (uint8_t)hw_version;
   p_outcoming_data->sw_version = (uint8_t)sw_version;
   p_outcoming_data->model_num  = (uint8_t)model_num;
}

BSCPage4::BSCPage4() :
   fract_bat_volt(0),
   coarse_bat_volt(0),
   bat_status(BSCPage4::ant_bsc_bat_status_t::BSC_BAT_STATUS_INVALID)
{
}

void BSCPage4::Decode(uint8_t const* buffer)
{
   ant_bsc_page4_data_layout_t const * p_incoming_data = (ant_bsc_page4_data_layout_t *)buffer;

   fract_bat_volt  = p_incoming_data->fract_bat_volt;
   coarse_bat_volt = p_incoming_data->coarse_bat_volt;
   bat_status      = (BSCPage4::ant_bsc_bat_status_t)p_incoming_data->bat_status;
}

void BSCPage4::Encode(uint8_t* buffer)
{
   ant_bsc_page4_data_layout_t * p_outcoming_data = (ant_bsc_page4_data_layout_t *)buffer;

   p_outcoming_data->reserved_byte     = UINT8_MAX;
   p_outcoming_data->fract_bat_volt    = fract_bat_volt;
   p_outcoming_data->coarse_bat_volt   = coarse_bat_volt;
   p_outcoming_data->bat_status        = bat_status;
   p_outcoming_data->bitfield_reserved = 0;
}

BSCPage5::BSCPage5() :
   stop_indicator(0),
   reserved(0)
{
}

void BSCPage5::Decode(uint8_t const* buffer)
{
   ant_bsc_page5_data_layout_t const * p_incoming_data = (ant_bsc_page5_data_layout_t *)buffer;

   stop_indicator  = (p_incoming_data->flags) & 0x01;
}

void BSCPage5::Encode(uint8_t* buffer)
{
   ant_bsc_page5_data_layout_t * p_outcoming_data = (ant_bsc_page5_data_layout_t *)buffer;

   p_outcoming_data->flags = (uint8_t)stop_indicator;
}

BSCCombinedPage0::BSCCombinedPage0() :
   cadence_event_time(0),
   cadence_rev_count(0),
   speed_event_time(0),
   speed_rev_count(0)
{
}

void BSCCombinedPage0::Decode(uint8_t const* buffer)
{
   ant_bsc_combined_page0_data_layout_t const * p_incoming_data = (ant_bsc_combined_page0_data_layout_t *)buffer;

   cadence_event_time = (uint16_t)((p_incoming_data->cadence_evt_time_MSB << 8) + p_incoming_data->cadence_evt_time_LSB);
   cadence_rev_count = (uint16_t) ((p_incoming_data->cadence_rev_count_MSB << 8) + p_incoming_data->cadence_rev_count_LSB);
   speed_event_time = (uint16_t)((p_incoming_data->speed_evt_time_MSB << 8) + p_incoming_data->speed_evt_time_LSB);
   speed_rev_count = (uint16_t) ((p_incoming_data->speed_rev_count_MSB << 8) + p_incoming_data->speed_rev_count_LSB);
}

void BSCCombinedPage0::Encode(uint8_t* buffer)
{
    ant_bsc_combined_page0_data_layout_t * p_outcoming_data = (ant_bsc_combined_page0_data_layout_t *) buffer;

    p_outcoming_data->cadence_evt_time_LSB  = (uint8_t)(cadence_event_time & UINT8_MAX);
    p_outcoming_data->cadence_evt_time_MSB  = (uint8_t)((cadence_event_time >> 8) & UINT8_MAX);
    p_outcoming_data->cadence_rev_count_LSB = (uint8_t)(cadence_rev_count & UINT8_MAX);
    p_outcoming_data->cadence_rev_count_MSB = (uint8_t)((cadence_rev_count >> 8) & UINT8_MAX);
    p_outcoming_data->speed_evt_time_LSB    = (uint8_t)(speed_event_time & UINT8_MAX);
    p_outcoming_data->speed_evt_time_MSB    = (uint8_t)((speed_event_time >> 8) & UINT8_MAX);
    p_outcoming_data->speed_rev_count_LSB   = (uint8_t)(speed_rev_count & UINT8_MAX);
    p_outcoming_data->speed_rev_count_MSB   = (uint8_t)((speed_rev_count >> 8) & UINT8_MAX);
}

BicycleSpeedCadence::BicycleSpeedCadence(BSCDeviceType d_t, ANTTransmissionMode mode) :
   ANTProfile(mode),
   device_type(d_t)
{

   page_1_present = true;
   page_4_present = true;
   main_page_number = ANT_BSC_PAGE_5;

   m_channel_sens_config.channel_type       = BSC_SENS_CHANNEL_TYPE;
   m_channel_sens_config.ext_assign         = BSC_EXT_ASSIGN;
   m_channel_sens_config.rf_freq            = BSC_ANTPLUS_RF_FREQ;
   m_channel_sens_config.transmission_type  = 1;
   m_channel_sens_config.device_type        = (uint8_t)d_t;
   m_channel_sens_config.device_number      = DeviceNumber;
   m_channel_sens_config.network_number     = ANTPLUS_NETWORK_NUMBER;

   switch (d_t)
   {
      case BSCDeviceType::Speed:
         m_channel_sens_config.channel_period     = BSC_MSG_PERIOD_SPEED * BSC_MSG_PERIOD_4Hz;
         break;

      case BSCDeviceType::Cadence:
         m_channel_sens_config.channel_period     = BSC_MSG_PERIOD_CADENCE * BSC_MSG_PERIOD_4Hz;
         break;
         
      case BSCDeviceType::SpeedAndCadence:
         m_channel_sens_config.channel_period     = BSC_MSG_PERIOD_COMBINED * BSC_MSG_PERIOD_4Hz;
         break;
         
   }



   m_disp_config.channel_type      = BSC_DISP_CHANNEL_TYPE;
   m_disp_config.ext_assign        = BSC_EXT_ASSIGN;
   m_disp_config.rf_freq           = BSC_ANTPLUS_RF_FREQ;
   m_disp_config.transmission_type = 0;
   m_disp_config.device_type       = (uint8_t)d_t;
   m_disp_config.device_number     = DeviceNumber;
   m_disp_config.channel_period    = BSC_MSG_PERIOD_4Hz;
   m_disp_config.network_number    = ANTPLUS_NETWORK_NUMBER;



   // Setup default Page values
   page2.SetManufacturerID(BSC_MF_ID);
   page2.SetSerialNumber(BSC_SERIAL_NUMBER);
   page3.SetHardwareVersion(BSC_HW_VERSION);
   page3.SetSoftwareVersion(BSC_SW_VERSION);
   page3.SetModelNumber(BSC_MODEL_NUMBER);



   page_1_present    = true;
   page_4_present    = true;
   main_page_number  = ANT_BSC_PAGE_5;
   bkgd_page_number  = page_1_present ? ANT_BSC_PAGE_1 : ANT_BSC_PAGE_2;
   message_counter   = 0;
   toggle_bit        = true;


   m_time_of_last_speed = millis();

//   _OnComputedHeartRate_cb = NULL;
}

/**@brief Function for setting the next background page number.
 *
 * @param[in]  p_profile        Pointer to the profile instance.
 *
 */
void BicycleSpeedCadence::SetNextBackgroundPageNumber()
{
    /* Determine the last background page*/
    ant_bsc_page_t last_bkgd_page = page_4_present ? ANT_BSC_PAGE_4 : ANT_BSC_PAGE_3;

    /* Switch the background page according to user settings */
    bkgd_page_number = (ant_bsc_page_t)((int)bkgd_page_number + 1);
    if (bkgd_page_number > last_bkgd_page)
    {
        bkgd_page_number = page_1_present ? ANT_BSC_PAGE_1 : ANT_BSC_PAGE_2;
    }
}

/**@brief Function for getting next page number to send.
 *
 * @param[in]  p_profile        Pointer to the profile instance.
 *
 * @return     Next page number.
 */
BicycleSpeedCadence::ant_bsc_page_t BicycleSpeedCadence::GetNextPageNumber()
{
    ant_bsc_page_t page_number;

    /* This is a single speed or cadence sensor - switch data page */
    if (message_counter < (BACKGROUND_DATA_INTERVAL))
    {
        /* Return main page */
        page_number  = main_page_number;
    }
    else
    {
        /* Return background page */
        page_number  = bkgd_page_number;
    }

    /* Set page toggle bit */
    if ((message_counter % TX_TOGGLE_DIVISOR) == 0)
    {
        toggle_bit ^= 1;
    }

    /* Update message counter, wrap when counter equals 64 + 4 */
    ++(message_counter);
    if (message_counter == (BACKGROUND_DATA_INTERVAL + MAIN_DATA_INTERVAL))
    {
        message_counter = 0;
        /* Set new background data page number */
        SetNextBackgroundPageNumber();
    }

    return page_number;
}

void BicycleSpeedCadence::EncodeMessage()
{
    ant_bsc_message_layout_t * p_bsc_message_payload = (ant_bsc_message_layout_t *)m_message_payload;

    if (device_type == BSCDeviceType::SpeedAndCadence)
    {
      combined_page0.Encode(p_bsc_message_payload->combined.page_payload);
    }
    else
    {
      p_bsc_message_payload->speed_or_cadence.page_number = GetNextPageNumber();
      p_bsc_message_payload->speed_or_cadence.toggle_bit  = toggle_bit;

      page0.Encode(p_bsc_message_payload->speed_or_cadence.page_payload);

      switch (p_bsc_message_payload->speed_or_cadence.page_number)
      {
         case ANT_BSC_PAGE_0:
               // No implementation needed
               break;
         case  ANT_BSC_PAGE_1:
               page1.Encode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;
         case ANT_BSC_PAGE_2:
               page2.Encode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;
         case ANT_BSC_PAGE_3:
               page3.Encode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;
         case ANT_BSC_PAGE_4:
               page4.Encode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;
         case ANT_BSC_PAGE_5:
               page5.Encode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;
         default:
               // No implementation needed
               break;
      }
   }

   m_time_of_last_encode = millis();
}

void BicycleSpeedCadence::DecodeMessage(uint8_t* buffer)
{
   ant_bsc_message_layout_t * p_bsc_message_payload = (ant_bsc_message_layout_t *)buffer;

   if (device_type == BSCDeviceType::SpeedAndCadence)
   {
      combined_page0.Decode(p_bsc_message_payload->combined.page_payload);
   }
   else
   {
      page0.Decode(p_bsc_message_payload->speed_or_cadence.page_payload);

      switch (p_bsc_message_payload->speed_or_cadence.page_number)
      {
         case ANT_BSC_PAGE_0:
               // No implementation needed
               break;

         case ANT_BSC_PAGE_1:
               page1.Decode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;

         case ANT_BSC_PAGE_2:
               page2.Decode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;

         case ANT_BSC_PAGE_3:
               page3.Decode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;

         case ANT_BSC_PAGE_4:
               page4.Decode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;

         case ANT_BSC_PAGE_5:
               page5.Decode(p_bsc_message_payload->speed_or_cadence.page_payload);
               break;

         default:
               break;
      }
   }
}

// void BicycleSpeedCadence::SetSpeed(float val)
// {
//    m_speed = val;  // in Miles/hour for now....
//    uint32_t time = millis();

//    float since = time - time_of_last_speed;
//    time_of_last_speed = time; 

//    page4.SetCoarseBatteryVoltage(2);
//    page4.SetFractionalBatteryVoltage(200);
//    page4.SetBatteryStatus(BSCPage4::ant_bsc_bat_status_t::BSC_BAT_STATUS_GOOD);

//    if (m_speed == 0)
//    {
//       page5.SetStopIndicator(1);
//    }
//    else
//    {
//       page5.SetStopIndicator(0);
//    }

//    page1.SetOperatingTime(time / 2000);


//    m_distance_meters += ((since / 1000.0) * (m_speed / 2.237));

//    float ticks_since = (float)time * 1024.0 / 1000.0;


//       page0.SetRevCount(m_distance_meters / 2.15);
//       page0.SetEventTime(ticks_since); // <- B<4,5> <-
//       //   combined_page0.SetSpeedRevCount(combined_page0.GetSpeedRevCount() + new_s_events);
//       //   combined_page0.SetCadenceRevCount(combined_page0.GetCadenceRevCount() + new_c_events);
//       //   combined_page0.SetSpeedEventTime((float)millis() / 1000.0 * 1024.0);
//       //   combined_page0.SetCadenceEventTime((float)millis() / 1000.0 * 1024.0);
// }

void BicycleSpeedCadence::SetStopped()
{
   page5.SetStopIndicator(1);
}

void BicycleSpeedCadence::AddCadenceRevolution()
{
   page4.SetCoarseBatteryVoltage(2);
   page4.SetFractionalBatteryVoltage(200);
   page4.SetBatteryStatus(BSCPage4::ant_bsc_bat_status_t::BSC_BAT_STATUS_GOOD);
   page5.SetStopIndicator(0);

   uint32_t time = millis();
   float ticks_since = (float)time * 1024.0 / 1000.0;

   page0.SetRevCount(page0.GetRevCount() + 1);
   page0.SetEventTime(ticks_since); // <- B<4,5> <-
   combined_page0.SetCadenceRevCount(page0.GetRevCount());
   combined_page0.SetCadenceEventTime(ticks_since);
}

void BicycleSpeedCadence::AddSpeedRevolution()
{
//    m_distance_meters += ((since / 1000.0) * (m_speed / 2.237));


   page4.SetCoarseBatteryVoltage(2);
   page4.SetFractionalBatteryVoltage(200);
   page4.SetBatteryStatus(BSCPage4::ant_bsc_bat_status_t::BSC_BAT_STATUS_GOOD);
   page5.SetStopIndicator(0);

   uint32_t time = millis();
   //float since = time - m_time_of_last_speed;
   m_time_of_last_speed = time; 

   float ticks_since = (float)time * 1024.0 / 1000.0;

   page0.SetRevCount(page0.GetRevCount() + 1);
   page0.SetEventTime(ticks_since); // <- B<4,5> <-
   combined_page0.SetSpeedRevCount(page0.GetRevCount());
   combined_page0.SetSpeedEventTime(ticks_since);
}