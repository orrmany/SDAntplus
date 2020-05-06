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

#include "ANTProfile.h"

 const  __FlashStringHelper *   AntEventTypeDecode(const ant_evt_t *evt)
{
   String desc;
   switch (evt->event)
   {
   case RESPONSE_NO_ERROR: ///< Command response with no error
                           //return F("0x00 = RESPONSE_NO_ERROR");
                           //break;
      //case NO_EVENT: ///< No Event
      return F("0x00 = RESPONSE_NO_ERROR or NO_EVENT");
      break;
   case EVENT_RX_SEARCH_TIMEOUT: ///< ANT stack generated event when rx searching state for the channel has timed out
      return F("0x01 = EVENT_RX_SEARCH_TIMEOUT");
      break;
   case EVENT_RX_FAIL: ///< ANT stack generated event when synchronous rx channel has missed receiving an ANT packet
      return F("0x02 = EVENT_RX_FAIL");
      break;
   case EVENT_TX: ///< ANT stack generated event when synchronous tx channel has occurred
      return F("0x03 = EVENT_TX");
      break;
   case EVENT_TRANSFER_RX_FAILED: ///< ANT stack generated event when the completion of rx transfer has failed
      return F("0x04 = EVENT_TRANSFER_RX_FAILED");
      break;
   case EVENT_TRANSFER_TX_COMPLETED: ///< ANT stack generated event when the completion of tx transfer has succeeded
      return F("0x05 = EVENT_TRANSFER_TX_COMPLETED");
      break;
   case EVENT_TRANSFER_TX_FAILED: ///< ANT stack generated event when the completion of tx transfer has failed
      return F("0x06 = EVENT_TRANSFER_TX_FAILED");
      break;
   case EVENT_CHANNEL_CLOSED: ///< ANT stack generated event when channel has closed
      return F("0x07 = EVENT_CHANNEL_CLOSED");
      break;
   case EVENT_RX_FAIL_GO_TO_SEARCH: ///< ANT stack generated event when synchronous rx channel has lost tracking and is entering rx searching state
      return F("0x08 = EVENT_RX_FAIL_GO_TO_SEARCH");
      break;
   case EVENT_CHANNEL_COLLISION: ///< ANT stack generated event during a multi-channel setup where an instance of the current synchronous channel is blocked by another synchronous channel
      return F("0x09 = EVENT_CHANNEL_COLLISION");
      break;
   case EVENT_TRANSFER_TX_START: ///< ANT stack generated event when the start of tx transfer is occuring
      return F("0x0A: EVENT_TRANSFER_TX_START");
      break;
   case EVENT_RX_DATA_OVERFLOW: ///< ANT stack generated event when data has been blocked due to latency in application event servicing
      return F("0x0B: EVENT_RX_DATA_OVERFLOW");
      break;
   //...
   case EVENT_TRANSFER_NEXT_DATA_BLOCK: ///< ANT stack generated event when the stack requires the next transfer data block for tx transfer continuation or completion
      return F("0x11 = EVENT_TRANSFER_NEXT_DATA_BLOCK");
      break;
   //...
   case CHANNEL_IN_WRONG_STATE: ///< Command response on attempt to perform an action from the wrong channel state
      return F("0x15 = CHANNEL_IN_WRONG_STATE");
      break;
   case CHANNEL_NOT_OPENED: ///< Command response on attempt to communicate on a channel that is not open
      return F("0x16 = CHANNEL_NOT_OPENED");
      break;
   //...
   case CHANNEL_ID_NOT_SET: ///< Command response on attempt to open a channel without setting the channel ID
      return F("0x18 = CHANNEL_ID_NOT_SET");
      break;
   case CLOSE_ALL_CHANNELS: ///< Command response when attempting to start scanning mode, when channels are still open
      return F("0x19 = CLOSE_ALL_CHANNELS");
      break;
   //...
   case TRANSFER_IN_PROGRESS: ///< Command response on attempt to communicate on a channel with a TX transfer in progress
      return F("0x1F: TRANSFER_IN_PROGRESS");
      break;
   case TRANSFER_SEQUENCE_NUMBER_ERROR: ///< Command response when sequence number of burst message or burst data segment is out of order
      return F("0x20 = TRANSFER_SEQUENCE_NUMBER_ERROR");
      break;
   case TRANSFER_IN_ERROR: ///< Command response when transfer error has occured on supplied burst message or burst data segment
      return F("0x21 = TRANSFER_IN_ERROR");
      break;
   case TRANSFER_BUSY: ///< Command response when transfer is busy and cannot process supplied burst message or burst data segment
      return F("0x22 = TRANSFER_BUSY");
      break;
   //...
   case MESSAGE_SIZE_EXCEEDS_LIMIT: ///< Command response if a data message is provided that is too large
      return F("0x27 = MESSAGE_SIZE_EXCEEDS_LIMIT");
      break;
   case INVALID_MESSAGE: ///< Command response when the message has an invalid parameter
      return F("0x28 = INVALID_MESSAGE");
      break;
   case INVALID_NETWORK_NUMBER: ///< Command response when an invalid network number is provided
      return F("0x29 = INVALID_NETWORK_NUMBER");
      break;
   case INVALID_LIST_ID: ///< Command response when the provided list ID or size exceeds the limit
      return F("0x30 = INVALID_LIST_ID");
      break;
   case INVALID_SCAN_TX_CHANNEL: ///< Command response when attempting to transmit on channel 0 when in scan mode
      return F("0x31 = INVALID_SCAN_TX_CHANNEL");
      break;
   case INVALID_PARAMETER_PROVIDED: ///< Command response when an invalid parameter is specified in a configuration message
      return F("0x33 = INVALID_PARAMETER_PROVIDED");
      break;
   case EVENT_QUE_OVERFLOW: ///< ANT stack generated event when the event queue in the stack has overflowed and drop 1 or 2 events
      return F("0x35 = EVENT_QUE_OVERFLOW");
      break;
   case EVENT_ENCRYPT_NEGOTIATION_SUCCESS: ///< ANT stack generated event when connecting to an encrypted channel has succeeded
      return F("0x38 = EVENT_ENCRYPT_NEGOTIATION_SUCCESS");
      break;
   case EVENT_ENCRYPT_NEGOTIATION_FAIL: ///< ANT stack generated event when connecting to an encrypted channel has failed
      return F("0x39 = EVENT_ENCRYPT_NEGOTIATION_FAIL");
      break;
   case EVENT_RFACTIVE_NOTIFICATION: ///< ANT stack generated event when the time to next synchronous channel RF activity exceeds configured time threshold
      return F("0x3A: EVENT_RFACTIVE_NOTIFICATION");
      break;
   case EVENT_CONNECTION_START: ///< Application generated event used to indicate when starting a connection to a channel
      return F("0x3B: EVENT_CONNECTION_START");
      break;
   case EVENT_CONNECTION_SUCCESS: ///< Application generated event used to indicate when successfuly connected to a channel
      return F("0x3C: EVENT_CONNECTION_SUCCESS");
      break;
   case EVENT_CONNECTION_FAIL: ///< Application generated event used to indicate when failed to connect to a channel
      return F("0x3D: EVENT_CONNECTION_FAIL");
      break;
   case EVENT_CONNECTION_TIMEOUT: ///< Application generated event used to indicate when connecting to a channel has timed out
      return F("0x3E: EVENT_CONNECTION_TIMEOUT");
      break;
   case EVENT_CONNECTION_UPDATE: ///< Application generated event used to indicate when connection parameters have been updated
      return F("0x3F: EVENT_CONNECTION_UPDATE");
      break;
   //...
   case NO_RESPONSE_MESSAGE: ///< Command response type intended to indicate that no serial reply message should be generated
      return F("0x50 = NO_RESPONSE_MESSAGE");
      break;
   case EVENT_RX: ///< ANT stack generated event indicating received data (eg. broadcast, acknowledge, burst) from the channel
      return F("0x80 = EVENT_RX");
      break;
   case EVENT_BLOCKED: ///< ANT stack generated event that should be ignored (eg. filtered events will generate this)
      return F("0xFF: EVENT_BLOCKED");
      break;

   default:
      return F("(Unknown event type");
      break;
   }
   return F("(Unknown event type");
}

const  __FlashStringHelper *  AntEventType2LongDescription(const ant_evt_t *evt)
{
   switch (evt->event)
   {
   case RESPONSE_NO_ERROR:
      return F("'Command response with no error', or 'No Event'");
      //case NO_EVENT:
      return F("0x00 = RESPONSE_NO_ERROR or NO_EVENT");
      break;
   case EVENT_RX_SEARCH_TIMEOUT:
      return F("ANT stack generated event when rx searching state for the channel has timed out");
      break;
   case EVENT_RX_FAIL:
      return F("ANT stack generated event when synchronous rx channel has missed receiving an ANT packet");
      break;
   case EVENT_TX:
      return F("ANT stack generated event when synchronous tx channel has occurred");
      break;
   case EVENT_TRANSFER_RX_FAILED:
      return F("ANT stack generated event when the completion of rx transfer has failed");
      break;
   case EVENT_TRANSFER_TX_COMPLETED:
      return F("ANT stack generated event when the completion of tx transfer has succeeded");
      break;
   case EVENT_TRANSFER_TX_FAILED:
      return F("ANT stack generated event when the completion of tx transfer has failed");
      break;
   case EVENT_CHANNEL_CLOSED:
      return F("ANT stack generated event when channel has closed");
      break;
   case EVENT_RX_FAIL_GO_TO_SEARCH:
      return F("ANT stack generated event when synchronous rx channel has lost tracking and is entering rx searching state");
      break;
   case EVENT_CHANNEL_COLLISION:
      return F("ANT stack generated event during a multi-channel setup where an instance of the current synchronous channel is blocked by another synchronous channel");
      break;
   case EVENT_TRANSFER_TX_START:
      return F("ANT stack generated event when the start of tx transfer is occuring");
      break;
   case EVENT_RX_DATA_OVERFLOW:
      return F("ANT stack generated event when data has been blocked due to latency in application event servicing");
      break;
   //...
   case EVENT_TRANSFER_NEXT_DATA_BLOCK:
      return F("ANT stack generated event when the stack requires the next transfer data block for tx transfer continuation or completion");
      break;
   //...
   case CHANNEL_IN_WRONG_STATE:
      return F("Command response on attempt to perform an action from the wrong channel state");
      break;
   case CHANNEL_NOT_OPENED:
      return F("Command response on attempt to communicate on a channel that is not open");
      break;
   //...
   case CHANNEL_ID_NOT_SET:
      return F("Command response on attempt to open a channel without setting the channel ID");
      break;
   case CLOSE_ALL_CHANNELS:
      return F("Command response when attempting to start scanning mode, when channels are still open");
      break;
   //...
   case TRANSFER_IN_PROGRESS:
      return F("Command response on attempt to communicate on a channel with a TX transfer in progress");
      break;
   case TRANSFER_SEQUENCE_NUMBER_ERROR:
      return F("Command response when sequence number of burst message or burst data segment is out of order");
      break;
   case TRANSFER_IN_ERROR:
      return F("Command response when transfer error has occured on supplied burst message or burst data segment");
      break;
   case TRANSFER_BUSY:
      return F("Command response when transfer is busy and cannot process supplied burst message or burst data segment");
      break;
   //...
   case MESSAGE_SIZE_EXCEEDS_LIMIT:
      return F("Command response if a data message is provided that is too large");
      break;
   case INVALID_MESSAGE:
      return F("Command response when the message has an invalid parameter");
      break;
   case INVALID_NETWORK_NUMBER:
      return F("Command response when an invalid network number is provided");
      break;
   case INVALID_LIST_ID:
      return F("Command response when the provided list ID or size exceeds the limit");
      break;
   case INVALID_SCAN_TX_CHANNEL:
      return F("Command response when attempting to transmit on channel 0 when in scan mode");
      break;
   case INVALID_PARAMETER_PROVIDED:
      return F("Command response when an invalid parameter is specified in a configuration message");
      break;
   case EVENT_QUE_OVERFLOW:
      return F("ANT stack generated event when the event queue in the stack has overflowed and drop 1 or 2 events");
      break;
   case EVENT_ENCRYPT_NEGOTIATION_SUCCESS:
      return F("ANT stack generated event when connecting to an encrypted channel has succeeded");
      break;
   case EVENT_ENCRYPT_NEGOTIATION_FAIL:
      return F("ANT stack generated event when connecting to an encrypted channel has failed");
      break;
   case EVENT_RFACTIVE_NOTIFICATION:
      return F("ANT stack generated event when the time to next synchronous channel RF activity exceeds configured time threshold");
      break;
   case EVENT_CONNECTION_START:
      return F("Application generated event used to indicate when starting a connection to a channel");
      break;
   case EVENT_CONNECTION_SUCCESS:
      return F("Application generated event used to indicate when successfuly connected to a channel");
      break;
   case EVENT_CONNECTION_FAIL:
      return F("Application generated event used to indicate when failed to connect to a channel");
      break;
   case EVENT_CONNECTION_TIMEOUT:
      return F("Application generated event used to indicate when connecting to a channel has timed out");
      break;
   case EVENT_CONNECTION_UPDATE:
      return F("Application generated event used to indicate when connection parameters have been updated");
      break;
   //...
   case NO_RESPONSE_MESSAGE:
      return F("Command response type intended to indicate that no serial reply message should be generated");
      break;
   case EVENT_RX:
      return F("ANT stack generated event indicating received data (eg. broadcast, acknowledge, burst) from the channel");
      break;
   case EVENT_BLOCKED:
      return F("ANT stack generated event that should be ignored (eg. filtered events will generate this)");
      break;

   default:
      return F("(Unknown event type");
      break;
   }
   return F("(Unknown event type");
}

ANTProfile::ANTProfile(ANTTransmissionMode mode)
{
   m_op_mode = mode;
}

void ANTProfile::ProcessMessage(ant_evt_t* evt)
{
   if (evt->channel == m_channel_number)
   {
      switch (evt->event)
      {
            case EVENT_TX                                    : // ((uint8_t)0x03)   ///< ANT stack generated event when synchronous tx channel has occurred
               EncodeMessage();
               SendMessage();
               break;

            case EVENT_RX                                    : // ((uint8_t)0x80)   ///< ANT stack generated event indicating received data (eg. broadcast, acknowledge, burst) from the channel
               if (evt->message.ANT_MESSAGE_ucMesgID == MESG_BROADCAST_DATA_ID
               || evt->message.ANT_MESSAGE_ucMesgID == MESG_ACKNOWLEDGED_DATA_ID
               || evt->message.ANT_MESSAGE_ucMesgID == MESG_BURST_DATA_ID)
               {
                  DecodeMessage(evt->message.ANT_MESSAGE_aucPayload);
                  newRxData = true;
                  newMillis = millis();
               }
               break;

            case RESPONSE_NO_ERROR                           : // ((uint8_t)0x00)   ///< Command response with no error
            //case NO_EVENT                                    : // ((uint8_t)0x00)   ///< No Event
            case EVENT_RX_SEARCH_TIMEOUT                     : // ((uint8_t)0x01)   ///< ANT stack generated event when rx searching state for the channel has timed out
            case EVENT_RX_FAIL                               : // ((uint8_t)0x02)   ///< ANT stack generated event when synchronous rx channel has missed receiving an ANT packet
            case EVENT_TRANSFER_RX_FAILED                    : // ((uint8_t)0x04)   ///< ANT stack generated event when the completion of rx transfer has failed
            case EVENT_TRANSFER_TX_COMPLETED                 : // ((uint8_t)0x05)   ///< ANT stack generated event when the completion of tx transfer has succeeded
            case EVENT_TRANSFER_TX_FAILED                    : // ((uint8_t)0x06)   ///< ANT stack generated event when the completion of tx transfer has failed
            case EVENT_CHANNEL_CLOSED                        : // ((uint8_t)0x07)   ///< ANT stack generated event when channel has closed
            case EVENT_RX_FAIL_GO_TO_SEARCH                  : // ((uint8_t)0x08)   ///< ANT stack generated event when synchronous rx channel has lost tracking and is entering rx searching state
            case EVENT_CHANNEL_COLLISION                     : // ((uint8_t)0x09)   ///< ANT stack generated event during a multi-channel setup where an instance of the current synchronous channel is blocked by another synchronous channel
            case EVENT_TRANSFER_TX_START                     : // ((uint8_t)0x0A)   ///< ANT stack generated event when the start of tx transfer is occuring
            case EVENT_RX_DATA_OVERFLOW                      : // ((uint8_t)0x0B)   ///< ANT stack generated event when data has been blocked due to latency in application event servicing
            case EVENT_TRANSFER_NEXT_DATA_BLOCK              : // ((uint8_t)0x11)   ///< ANT stack generated event when the stack requires the next transfer data block for tx transfer continuation or completion
            case CHANNEL_IN_WRONG_STATE                      : // ((uint8_t)0x15)   ///< Command response on attempt to perform an action from the wrong channel state
            case CHANNEL_NOT_OPENED                          : // ((uint8_t)0x16)   ///< Command response on attempt to communicate on a channel that is not open
            case CHANNEL_ID_NOT_SET                          : // ((uint8_t)0x18)   ///< Command response on attempt to open a channel without setting the channel ID
            case CLOSE_ALL_CHANNELS                          : // ((uint8_t)0x19)   ///< Command response when attempting to start scanning mode, when channels are still open
            case TRANSFER_IN_PROGRESS                        : // ((uint8_t)0x1F)   ///< Command response on attempt to communicate on a channel with a TX transfer in progress
            case TRANSFER_SEQUENCE_NUMBER_ERROR              : // ((uint8_t)0x20)   ///< Command response when sequence number of burst message or burst data segment is out of order
            case TRANSFER_IN_ERROR                           : // ((uint8_t)0x21)   ///< Command response when transfer error has occured on supplied burst message or burst data segment
            case TRANSFER_BUSY                               : // ((uint8_t)0x22)   ///< Command response when transfer is busy and cannot process supplied burst message or burst data segment
            case MESSAGE_SIZE_EXCEEDS_LIMIT                  : // ((uint8_t)0x27)   ///< Command response if a data message is provided that is too large
            case INVALID_MESSAGE                             : // ((uint8_t)0x28)   ///< Command response when the message has an invalid parameter
            case INVALID_NETWORK_NUMBER                      : // ((uint8_t)0x29)   ///< Command response when an invalid network number is provided
            case INVALID_LIST_ID                             : // ((uint8_t)0x30)   ///< Command response when the provided list ID or size exceeds the limit
            case INVALID_SCAN_TX_CHANNEL                     : // ((uint8_t)0x31)   ///< Command response when attempting to transmit on channel 0 when in scan mode
            case INVALID_PARAMETER_PROVIDED                  : // ((uint8_t)0x33)   ///< Command response when an invalid parameter is specified in a configuration message
            case EVENT_QUE_OVERFLOW                          : // ((uint8_t)0x35)   ///< ANT stack generated event when the event queue in the stack has overflowed and drop 1 or 2 events
            case EVENT_ENCRYPT_NEGOTIATION_SUCCESS           : // ((uint8_t)0x38)   ///< ANT stack generated event when connecting to an encrypted channel has succeeded
            case EVENT_ENCRYPT_NEGOTIATION_FAIL              : // ((uint8_t)0x39)   ///< ANT stack generated event when connecting to an encrypted channel has failed
            case EVENT_RFACTIVE_NOTIFICATION                 : // ((uint8_t)0x3A)   ///< ANT stack generated event when the time to next synchronous channel RF activity exceeds configured time threshold
            case EVENT_CONNECTION_START                      : // ((uint8_t)0x3B)   ///< Application generated event used to indicate when starting a connection to a channel
            case EVENT_CONNECTION_SUCCESS                    : // ((uint8_t)0x3C)   ///< Application generated event used to indicate when successfuly connected to a channel
            case EVENT_CONNECTION_FAIL                       : // ((uint8_t)0x3D)   ///< Application generated event used to indicate when failed to connect to a channel
            case EVENT_CONNECTION_TIMEOUT                    : // ((uint8_t)0x3E)   ///< Application generated event used to indicate when connecting to a channel has timed out
            case EVENT_CONNECTION_UPDATE                     : // ((uint8_t)0x3F)   ///< Application generated event used to indicate when connection parameters have been updated
            case NO_RESPONSE_MESSAGE                         : // ((uint8_t)0x50)   ///< Command response type intended to indicate that no serial reply message should be generated
            case EVENT_BLOCKED                               : // ((uint8_t)0xFF)   ///< ANT stack generated event that should be ignored (eg. filtered events will generate this)
            default:
               if (_AntUnhandledEventLister!=NULL) { _AntUnhandledEventLister(evt);}
               break;
      } //swith evt
      if (_AntAllEventLister!=NULL) { _AntAllEventLister(evt);}
   } //if (evt->channel == m_channel_number)
}

uint32_t ANTProfile::Setup(uint8_t channel)
{
   uint32_t err_code;

   m_channel_number = channel;
   m_channel_sens_config.channel_number = channel;
   m_disp_config.channel_number = channel;

   if (m_op_mode == ANTTransmissionMode::TX)
   {
      // Fill tx buffer for the first frame
      EncodeMessage();

      uint32_t err_code;
      // Set Channel Number.
      err_code = sd_ant_channel_assign(m_channel_sens_config.channel_number,
                                       m_channel_sens_config.channel_type,
                                       m_channel_sens_config.network_number,
                                       m_channel_sens_config.ext_assign);
      if (err_code != NRF_SUCCESS)
      {
         return err_code;
      }

      // Set Channel ID.
      err_code = sd_ant_channel_id_set(m_channel_sens_config.channel_number,
                                       m_channel_sens_config.device_number,
                                       m_channel_sens_config.device_type,
                                       m_channel_sens_config.transmission_type);
      if (err_code != NRF_SUCCESS)
      {
         return err_code;
      }

      // Set Channel RF frequency.
      err_code = sd_ant_channel_radio_freq_set(m_channel_sens_config.channel_number, m_channel_sens_config.rf_freq);
      if (err_code != NRF_SUCCESS)
      {
         return err_code;
      }

      // Set Channel period.
      if (!(m_channel_sens_config.ext_assign & EXT_PARAM_ALWAYS_SEARCH) && (m_channel_sens_config.channel_period != 0))
      {
         err_code = sd_ant_channel_period_set(m_channel_sens_config.channel_number, m_channel_sens_config.channel_period);
      }

      if (err_code != NRF_SUCCESS)
      {
         return err_code;
      }

      SendMessage();
    }
    else if (m_op_mode == ANTTransmissionMode::RX)
    {
        err_code = ant_channel_init(&m_disp_config);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    err_code = sd_ant_channel_open(m_channel_number);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

   return NRF_SUCCESS;
}

uint32_t ANTProfile::SendMessage()
{
   uint32_t err_code = sd_ant_broadcast_message_tx(m_channel_number, sizeof(m_message_payload), m_message_payload);

   return err_code;
}
