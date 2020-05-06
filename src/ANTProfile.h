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

   void ProcessMessage(ant_evt_t* evt);
   void setUnhandledEventListener(void (*fp)(ant_evt_t* evt)) { _AntUnhandledEventLister = fp; };
   void setAllEventListener(void (*fp)(ant_evt_t* evt)) { _AntAllEventLister = fp; };
   //void setCustomDataPtr(void* ptr) { m_customDataPtr = ptr;}
   //void* getCustomDataPtr(void) {return m_customDataPtr;} 
   bool newRxData = false;
   uint32_t newMillis=0;

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