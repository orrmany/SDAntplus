/*
(Modified) MIT License

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

#include "Environment.hh"
#include "../util.h"
#include <Arduino.h> 
EnvironmentPage0::EnvironmentPage0(ant_env_transmission_rate_t p_default_transmission)
{
   local_time_support = not_supported;
   utc_time_support = not_supported;
   default_transmission_rate = p_default_transmission;
}

void EnvironmentPage0::Decode(uint8_t const *buffer)
{
   ant_env_page0_data_layout_t const *p_incoming_data = (ant_env_page0_data_layout_t const *)buffer;
   //local time support
   switch ((p_incoming_data->transmission_info & TRANSMISSION_INFO_BITS_LOCAL_TIME_bitmask) >> TRANSMISSION_INFO_BITS_LOCAL_TIME_bitshift)
   {
   case TRANSMISSION_INFO_BITS_LOCAL_TIME_NotSupported:
      local_time_support = not_supported;
      break;
   case TRANSMISSION_INFO_BITS_LOCAL_TIME_SupportedNotSet:
      local_time_support = supported_not_set;
      break;
   case TRANSMISSION_INFO_BITS_LOCAL_TIME_SupportedSet:
      local_time_support = supported_and_set;
      break;
   case TRANSMISSION_INFO_BITS_LOCAL_TIME_Reserved:
   default:
      local_time_support = reserved;
      break;
   }
   //UTC time support
   switch ((p_incoming_data->transmission_info & TRANSMISSION_INFO_BITS_UTC_TIME_bitmask) >> TRANSMISSION_INFO_BITS_UTC_TIME_bitshift)
   {
   case TRANSMISSION_INFO_BITS_UTC_TIME_NotSupported:
      utc_time_support = not_supported;
      break;
   case TRANSMISSION_INFO_BITS_UTC_TIME_SupportedNotSet:
      utc_time_support = supported_not_set;
      break;
   case TRANSMISSION_INFO_BITS_UTC_TIME_SupportedSet:
      utc_time_support = supported_and_set;
      break;
   case TRANSMISSION_INFO_BITS_UTC_TIME_Reserved:
   default:
      utc_time_support = reserved;
      break;
   }
   //default transmission rate
   switch ((p_incoming_data->transmission_info & TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitmask) >> TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitshift)
   {
   case TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_0p5Hz:
      default_transmission_rate = half_Hz;
      break;
   case TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_4Hz:
      default_transmission_rate = four_Hz;
      break;
   case TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_Reserved10:
      default_transmission_rate = reserved10;
      break;
   case TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_Reserved11:
   default:
      default_transmission_rate = reserved11;
      break;
   }
   //page 1 support
   if (p_incoming_data->supported_pages[0] & BIT1)
      page1_support = true;
   else
      page1_support = false;
}
void EnvironmentPage0::Encode(uint8_t *buffer)
{
   ant_env_page0_data_layout_t *p_outgoing_data = (ant_env_page0_data_layout_t *)buffer;

   //p_outgoing_data->page_number = 0x00;       //byte 0: Data Page Number: 0 (0x00)
   p_outgoing_data->reserved[0] = 0xFF;       //byte 1: Reserved: Set to 0xFF
   p_outgoing_data->reserved[1] = 0xFF;       //byte 2: Reserved: Set to 0xFF
   p_outgoing_data->transmission_info = 0x00; //byte 3: initally setting to all 0
   //bits 6:7 Reserved Set to 0.
   //bits 4:5 Local Time
   switch (local_time_support)
   {
   case not_supported:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_LOCAL_TIME_NotSupported << TRANSMISSION_INFO_BITS_LOCAL_TIME_bitshift);
      break;
   case supported_not_set:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_LOCAL_TIME_SupportedNotSet << TRANSMISSION_INFO_BITS_LOCAL_TIME_bitshift);
      break;
   case supported_and_set:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_LOCAL_TIME_SupportedSet << TRANSMISSION_INFO_BITS_LOCAL_TIME_bitshift);
      break;
   case reserved:
   default:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_LOCAL_TIME_Reserved << TRANSMISSION_INFO_BITS_LOCAL_TIME_bitshift);
      break;
   }
   //bits 2:3 UTC Time
   switch (utc_time_support)
   {
   case not_supported:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_UTC_TIME_NotSupported << TRANSMISSION_INFO_BITS_UTC_TIME_bitshift);
      break;
   case supported_not_set:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_UTC_TIME_SupportedNotSet << TRANSMISSION_INFO_BITS_UTC_TIME_bitshift);
      break;
   case supported_and_set:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_UTC_TIME_SupportedSet << TRANSMISSION_INFO_BITS_UTC_TIME_bitshift);
      break;
   case reserved:
   default:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_UTC_TIME_Reserved << TRANSMISSION_INFO_BITS_UTC_TIME_bitshift);
      break;
   }
   //bits 0:1 default transmission rate
   switch (default_transmission_rate)
   {
   case half_Hz:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_0p5Hz << TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitshift);
      break;
   case four_Hz:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_4Hz << TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitshift);
      break;
   case reserved10:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_Reserved10 << TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitshift);
      break;
   case reserved11:
   default:
      p_outgoing_data->transmission_info |= (TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_Reserved11 << TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitshift);
      break;
   }
   //supported pages
   p_outgoing_data->supported_pages[0] = BIT0; //Page0 support is mandatory
   if (page1_support)
      p_outgoing_data->supported_pages[0] |= BIT1;
   p_outgoing_data->supported_pages[1] = 0x00;
   p_outgoing_data->supported_pages[2] = 0x00;
   p_outgoing_data->supported_pages[3] = 0x00;
};

EnvironmentPage1::EnvironmentPage1() : p_event_count(0), p_temp_min(0), p_temp_max(0), p_temp_current(0){};

void EnvironmentPage1::Decode(uint8_t const *buffer)
{
   ant_env_page1_data_layout_t const *p_incoming_data = (ant_env_page1_data_layout_t const *)buffer;
   p_event_count = (uint8_t)p_incoming_data->event_count;
   p_temp_min = (uint16_t)((uint8_t)p_incoming_data->temp_Min_Max24h[0] | (((uint8_t)p_incoming_data->temp_Min_Max24h[1] & BITMASK_MSN) << BITSHIFT_MSN));
   p_temp_max = (uint16_t)(((uint8_t)p_incoming_data->temp_Min_Max24h[1] & BITMASK_LSN) | ((uint8_t)p_incoming_data->temp_Min_Max24h[2] << BITSHIFT_BYTE));
   p_temp_current = (uint16_t)((uint8_t)p_incoming_data->current_temp_lsb | ((uint8_t)p_incoming_data->current_temp_msb << BITSHIFT_BYTE));
};
void EnvironmentPage1::Encode(uint8_t *buffer)
{
   ant_env_page1_data_layout_t *p_outgoing_data = (ant_env_page1_data_layout_t *)buffer;
   //p_outgoing_data->page_number = 0x01;                                                                  // byte 0: Data Page Number: 1 (0x01)
   p_outgoing_data->reserved = 0xFF;                                                                     //byte 1: Reserved: Set to 0xFF
   p_outgoing_data->event_count = (uint8_t)(p_event_count & BITMASK_BYTE);                               //byte2: Event count: increments with each measurement
   p_outgoing_data->temp_Min_Max24h[0] = (uint8_t)(p_temp_min & BITMASK_BYTE);                           //byte3: 24 Hour Low LSB: 1.5 Bytes Signed Integer representing the lowest temperature recorded over the last 24 hours (0x800 invalid)
   p_outgoing_data->temp_Min_Max24h[1] = (uint8_t)((uint8_t)((p_temp_min >> BITSHIFT_MSN) & BITMASK_MSN) //byte4: (bits 4:7) 24 Hour Low MSN
                                                   | (uint8_t)(p_temp_max & BITMASK_LSN));               //byte4: (bits 0:3) 24 Hour High LSN
   p_outgoing_data->temp_Min_Max24h[2] = (uint8_t)((p_temp_max >> BITSHIFT_MSN) & BITMASK_BYTE);         //byte:5 24 Hour High MSB  1.5 Bytes Signed Integer representing the highest temperature recorded over the last 24 hours (0x800 invalid)
   p_outgoing_data->current_temp_lsb = (uint8_t)(p_temp_current & BITMASK_BYTE);                         //byte 6: Current Temp LSB 2 Bytes Signed Integer representing the current Temperature (0x8000 invalid)
   p_outgoing_data->current_temp_msb = (uint8_t)((p_temp_current >> BITSHIFT_BYTE) & BITMASK_BYTE);      //byte 7: Current Temp MSB
};

EnvironmentSensor::EnvironmentSensor(ANTTransmissionMode mode) : ANTProfile(mode)
{
   //m_ant_env_channel_env_disp_config.channel_number = ENV_CHANNEL_NUM;
   m_disp_config.channel_type = ENV_DISP_CHANNEL_TYPE;
   m_disp_config.ext_assign = ENV_EXT_ASSIGN;
   m_disp_config.rf_freq = ENV_ANTPLUS_RF_FREQ;
   m_disp_config.transmission_type = 0; //0x05; //213; //Transmission Type Bits 0:3 - 5 (0x05) ANT+ devices will follow the transmission type definition as outlined in the ANT protocol: The lower nibble shall be set to 0x05. The upper nibble may be used to extend the device number
   m_disp_config.device_type = ENV_DEVICE_TYPE;
   m_disp_config.device_number = 0; //CHAN_ID_DEV_NUM;
   m_disp_config.channel_period = ENV_MSG_PERIOD_0p5Hz;
   m_disp_config.network_number = ANTPLUS_NETWORK_NUM;
  
   m_channel_sens_config.channel_type       = ENV_SENS_CHANNEL_TYPE;
   m_channel_sens_config.ext_assign         = ENV_EXT_ASSIGN;
   m_channel_sens_config.rf_freq            = ENV_ANTPLUS_RF_FREQ;
   m_channel_sens_config.transmission_type  = 0x05; //213; //Transmission Type Bits 0:3 - 5 (0x05) ANT+ devices will follow the transmission type definition as outlined in the ANT protocol: The lower nibble shall be set to 0x05. The upper nibble may be used to extend the device number
   m_channel_sens_config.device_type        = ENV_DEVICE_TYPE;
   m_channel_sens_config.channel_period     = ENV_MSG_PERIOD_4Hz;
   m_channel_sens_config.device_number      = DeviceNumber;
   m_channel_sens_config.network_number     = ANTPLUS_NETWORK_NUMBER;


   _OnTemperatureData_cb = NULL;
};

#if false
bool EnvironmentRx::Setup()
{
   uint32_t err_code;

   err_code = ant_channel_init(&m_ant_env_channel_env_disp_config);
   if (err_code != NRF_SUCCESS)
   {
      Serial.printf("EnvironmentRx::Setup(): Error in channel init. Error code diff:0x%x\n", err_code - NRF_ERROR_ANT_BASE_NUM);
      Serial.println("Check https://infocenter.nordicsemi.com/index.jsp?topic=%2Fsdk_nrf5_v16.0.0%2Findex.html");
      return false;
   }

   Serial.println("Setting Environment sensor extended timeout");
   err_code = sd_ant_channel_search_timeout_set(channel_number, 
      1 // 24*2.5sec=60sec for 
         // ucTimeout is an unsigned char (1 octet) denoting the timeout value.
         //   When applied to an assigned slave channel, ucTimeout is in 2.5 second increments. Default = 10 (25s) at channel assignment
         //   When applied to an assigned master channel, ucTimeout is in 250ms increments. Default = 0 (disabled) at channel assignment
      ); 
   if (err_code != NRF_SUCCESS)
   {
      Serial.printf("EnvironmentRx::Setup(): Error in setting timeout . Error code diff:0x%x\n", err_code-NRF_ERROR_ANT_BASE_NUM);
      Serial.println("Check https://infocenter.nordicsemi.com/index.jsp?topic=%2Fsdk_nrf5_v16.0.0%2Findex.html");
      
      return false;
   }

   err_code = sd_ant_channel_open(channel_number);
   if (err_code != NRF_SUCCESS)
   {
      Serial.printf("EnvironmentRx::Setup(): Error in opening channel #%d . Error code diff:0x%x\n", channel_number, err_code - NRF_ERROR_ANT_BASE_NUM);
      Serial.println("Check https://infocenter.nordicsemi.com/index.jsp?topic=%2Fsdk_nrf5_v16.0.0%2Findex.html");
      return false;
   }

   return true;
};
#endif

#if false
void EnvironmentSensor::ProcessMessage(ant_evt_t *evt)
{
   //Serial.printf("Environment: checking event for ch. %d...\n", channel_number);
   Serial.printf("Event is: ch:%d ev:0x%x\n", evt->channel, evt->event);
   if (evt->channel == channel_number)
   {
      switch (evt->event)
      {
      case EVENT_RX:
         Serial.println("Environment: EVENT_RX");
         Serial.printf("MessageID: 0x%x\n", evt->message.ANT_MESSAGE_ucMesgID);
         if (evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID) Serial.println("MESG_BROADCAST_DATA");
         if (evt->message.ANT_MESSAGE_ucMesgID == MESG_ACKNOWLEDGED_DATA_ID) Serial.println("MESG_ACKNOWLEDGED_DATA");
         if (evt->message.ANT_MESSAGE_ucMesgID == MESG_BURST_DATA_ID) Serial.println("MESG_BURST_DATA");

         if (evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID //evt->message.stMessage.uFramedData.stFramedData.ucMesgID
             || evt->message.ANT_MESSAGE_ucMesgID == MESG_ACKNOWLEDGED_DATA_ID 
             || evt->message.ANT_MESSAGE_ucMesgID == MESG_BURST_DATA_ID)
         {
            ant_env_message_layout_t *p_env_msg_payload = (ant_env_message_layout_t *)evt->message.ANT_MESSAGE_aucPayload;
            uint8_t *payload = (uint8_t *)p_env_msg_payload;
            Serial.printf("Env. processing page 0x%x\nHex-payload:[", p_env_msg_payload->page_number);
            for (uint8_t i = 0; i < ANT_STANDARD_DATA_PAYLOAD_SIZE; i++)
               Serial.printf("%02X", payload[i]);
            Serial.printf("]\n");
            
            switch (p_env_msg_payload->page_number)
            {
            case ANT_ENV_PAGE_0:
               Serial.printf("Decoding page 0\n");
               page0.Decode(p_env_msg_payload->page_payload);
               break;
            case ANT_ENV_PAGE_1:
               page1.Decode(p_env_msg_payload->page_payload);
               Serial.printf("Decoding page 1\n");
               if (_OnTemperatureData_cb != NULL)
               {
                  _OnTemperatureData_cb(page1.GetCurrTemp());
               }
               break;
            default:
               Serial.print("Environment: unsupported page");
               break;
            }
         }
         break;
      case EVENT_CHANNEL_COLLISION :
         Serial.println("Environment: EVENT_CHANNEL_COLLISION");
         Serial.println("(ANT stack generated event during a multi-channel setup where an instance of the current synchronous channel is blocked by another synchronous channel)");
         Serial.printf("MessageID: 0x%x\n", evt->message.ANT_MESSAGE_ucMesgID);
         break;
      case EVENT_RX_DATA_OVERFLOW:
         Serial.println("Environment: EVENT_RX_DATA_OVERFLOW");
         Serial.println("(ANT stack generated event when data has been blocked due to latency in application event servicing)");
         Serial.printf("MessageID: 0x%x\n", evt->message.ANT_MESSAGE_ucMesgID);
         break;
      case  EVENT_CHANNEL_CLOSED:
         Serial.println("Environment: EVENT_CHANNEL_CLOSED");
         Serial.println("(ANT stack generated event when channel has closed)");
         Serial.printf("MessageID: 0x%x\n", evt->message.ANT_MESSAGE_ucMesgID);
         break;
      case EVENT_RX_FAIL:                              
         Serial.println("Environment: EVENT_RX_FAIL");
         Serial.println("ANT stack generated event when synchronous rx channel has missed receiving an ANT packet");
         Serial.printf("MessageID: 0x%x\n", evt->message.ANT_MESSAGE_ucMesgID);
         break;
      case EVENT_RX_FAIL_GO_TO_SEARCH:
         Serial.println("Environment: EVENT_RX_FAIL_GO_TO_SEARCH");
         Serial.println("ANT stack generated event when synchronous rx channel has lost tracking and is entering rx searching state");
         break;
      default:
         Serial.print("Environment: unknown event");
         break;
      }
   }
   Serial.println();
};
#endif //false

void EnvironmentSensor::EncodeMessage()
{
   //todo
}

void EnvironmentSensor::DecodeMessage(uint8_t* buffer)
{
   ant_env_message_layout_t* p_env_message_payload = (ant_env_message_layout_t *)buffer;
   switch (p_env_message_payload->page_number)
   {
   case ANT_ENV_PAGE_0:
      page0.Decode(p_env_message_payload->page_payload);
      break;
   case ANT_ENV_PAGE_1:
      page1.Decode(p_env_message_payload->page_payload);
      if (_OnTemperatureData_cb != NULL) 
      {
         _OnTemperatureData_cb(page1.GetCurrTemp());
      }
      break;

   default:
      break;
   }
}