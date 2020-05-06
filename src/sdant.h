#ifndef SDANT_H_
#define SDANT_H_

#include <bluefruit.h>
#include "ANTProfile.h"

//NOTE ANT network key settings moved to "ant/ANTProfile.h"
//#ifndef ANT_PLUS_NETWORK_KEY
//    #define ANT_PLUS_NETWORK_KEY    {0, 0, 0, 0, 0, 0, 0, 0}            /**< The ANT+ network key. */
//#endif //ANT_PLUS_NETWORK_KEY
//#ifndef ANT_FS_NETWORK_KEY
//    #define ANT_FS_NETWORK_KEY      {0, 0, 0, 0, 0, 0, 0, 0}           /**< The ANT-FS network key. */
//#endif // ANT_FS_NETWORK_KEY


class SdAnt
{
  public:
 
    SdAnt(void); // Constructor


    bool begin(uint8_t ant_count);

    /*------------------------------------------------------------------*/
    /* Callbacks
     *------------------------------------------------------------------*/
    void setANTEventCallback( void (*fp) (ant_evt_t*) );

   void AddProfile(ANTProfile* p);
   ANTProfile* getAntProfileByChNum(uint8_t ch) {
      for (ANTProfileEntry* entry = m_profile_list.m_head; entry != NULL; entry = entry->m_next)
      {
         if (entry->m_entry->getChannelNumber() == ch)  return entry->m_entry;
      }
      return NULL;
   }

  private:
    /*------------- SoftDevice Configuration -------------*/

    SemaphoreHandle_t _ant_event_sem;
    void (*_ant_event_cb) (ant_evt_t*);

   uint8_t m_ant_plus_network_key[8];
   uint8_t m_ant_fs_network_key[8];

   ANTProfileList m_profile_list;
   // Memory buffer provided in order to support channel configuration.
   __ALIGN(4) uint8_t* m_ant_stack_buffer;

    /*------------------------------------------------------------------*/
    /* INTERNAL USAGE ONLY
     *------------------------------------------------------------------*/
    void _ant_handler(ant_evt_t* evt);
    friend void SD_EVT_IRQHandler(void);
    friend void adafruit_ant_task(void* arg);
};

extern SdAnt ANTplus;

#endif
