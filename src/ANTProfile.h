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

#ifndef ANTPROFILE_H
#define ANTPROFILE_H

#if __has_include("ANT_Network_Keys.h") 
  #include "ANT_Network_Keys.h" //set the ANT+ Network key with #define in "ANT_Network_Keys.h"! 
#endif

#ifndef ANT_PLUS_NETWORK_KEY
   #warning ANT+ Network key is missing!!!
   #define ANT_PLUS_NETWORK_KEY    {0, 0, 0, 0, 0, 0, 0, 0}            /**< The ANT+ network key. */
#endif //ANT_PLUS_NETWORK_KEY
#ifndef ANT_FS_NETWORK_KEY
   #warning ANT FS network key is missing!!!
   #define ANT_FS_NETWORK_KEY      {0, 0, 0, 0, 0, 0, 0, 0}           /**< The ANT-FS network key. */
#endif // ANT_FS_NETWORK_KEY

#include <stdio.h>

#include "ant_interface.h"
#include "nrf_error.h"
#include "ant_event.h"
#include "ant_channel_config.h"
#include <Arduino.h>
#include <avr/pgmspace.h>

#define ANTPLUS_NETWORK_NUMBER  0                           /**< Network number. */
#define MAIN_DATA_INTERVAL          4       /**< The number of background data pages sent between main data pages.*/
#define BACKGROUND_DATA_INTERVAL    64      /**< The number of main data pages sent between background data page.
                                                 Background data page is sent every 65th message. */
#define TX_TOGGLE_DIVISOR           4       /**< The number of messages between changing state of toggle bit. */

enum ANTTransmissionMode
{
   RX,
   TX,
   Both
};

extern const  __FlashStringHelper * AntEventTypeDecode(const ant_evt_t *evt);
extern const  __FlashStringHelper * AntEventType2LongDescription(const ant_evt_t *evt);

class ANTProfile
{
public:

   ANTProfile(ANTTransmissionMode mode);

   //profile operartions 
   uint32_t Setup(uint8_t channel);
    
   //Profile name 
   void setName(const char* pname) {name=pname;}
   const char* getName(void) {return name;}
   uint8_t getChannelNumber(void) { return m_channel_number;}

   virtual void ProcessMessage(ant_evt_t* evt);
   void setUnhandledEventListener(void (*fp)(ant_evt_t* evt)) { _AntUnhandledEventLister = fp; };
   void setAllEventListener(void (*fp)(ant_evt_t* evt)) { _AntAllEventLister = fp; };
   //void setCustomDataPtr(void* ptr) { m_customDataPtr = ptr;}
   //void* getCustomDataPtr(void) {return m_customDataPtr;} 
   bool newRxData = false;
   uint32_t newTicks=0;

protected:

   virtual void DecodeMessage(uint8_t* buffer) = 0;
   virtual void EncodeMessage() = 0;
   uint32_t SendMessage();
   void (*_AntUnhandledEventLister)(ant_evt_t* evt) = NULL; 
   void (*_AntAllEventLister)(ant_evt_t* evt) = NULL; 
   const char *  name = "";

   uint8_t m_channel_number; ///< Channel number assigned to the profile.
   uint8_t m_message_payload[ANT_STANDARD_DATA_PAYLOAD_SIZE];
   ANTTransmissionMode m_op_mode;

   ant_channel_config_t m_channel_sens_config;
   ant_channel_config_t m_disp_config;
   void* m_customDataPtr= NULL; //the last time data sent/received

private:

};



class ANTProfileEntry
{
public:

   ANTProfileEntry(ANTProfile* e = NULL)
   {
      m_entry = e;
      m_next = NULL;
   }

   ANTProfile* m_entry;
   ANTProfileEntry* m_next;
};


class ANTProfileList
{
public:

   ANTProfileList()
   {
      m_head = NULL;
      m_tail = NULL;
   }

   void AddProfile(ANTProfile* p)
   {
      ANTProfileEntry* entry = new ANTProfileEntry(p);
      if (m_tail == NULL)
         m_head = entry;
      else
         m_tail->m_next = entry;
      m_tail = entry;
   }

   ANTProfileEntry* m_head;
   ANTProfileEntry* m_tail;
};
#endif