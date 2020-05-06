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

#include "sdant.h"

#ifndef CFG_ANT_TASK_STACKSIZE
#define CFG_ANT_TASK_STACKSIZE (256 * 5)
#endif

SdAnt ANTplus;

void adafruit_ant_task(void *arg);

static void nrf_error_cb(uint32_t id, uint32_t pc, uint32_t info)
{
#if CFG_DEBUG
  PRINT_INT(id);
  PRINT_HEX(pc);
  PRINT_HEX(info);

  if (id == NRF_FAULT_ID_SD_ASSERT && info != 0)
  {
    typedef struct
    {
      uint16_t line_num;          /**< The line number where the error occurred. */
      uint8_t const *p_file_name; /**< The file in which the error occurred. */
    } assert_info_t;

    assert_info_t *assert_info = (assert_info_t *)info;

    LOG_LV1("SD Err", "assert at %s : %d", assert_info->p_file_name, assert_info->line_num);
  }

  while (1)
    yield();
#endif
}

// Constructor
SdAnt::SdAnt(void)
{
  _ant_event_sem = NULL;
  _ant_event_cb = NULL;
}

bool SdAnt::begin(uint8_t ant_count)
{
  if (ant_count <= 0)
    return false; //no channels needed

  // ###################################################################
  // sd_softdevice_enable is supposed to be called already by Bluefruit.
  // If not then we  return with false
  // ####################################################################
  //check if SD is enabled
  uint8_t sd_enabled = 0;
  sd_softdevice_is_enabled(&sd_enabled);
  if (sd_enabled != 1)
    return false; //SD must be enabled!

  // sd_softdevice_enable(&clock_cfg, nrf_error_cb, ANT_LICENSE_KEY), false );

  m_ant_stack_buffer = (uint8_t *)malloc(ANT_ENABLE_GET_REQUIRED_SPACE(ant_count, 0, 128, 64));

  ANT_ENABLE ant_enable_cfg =
      {
          .ucTotalNumberOfChannels = ant_count,
          .ucNumberOfEncryptedChannels = 0,
          .usNumberOfEvents = 64,
          .pucMemoryBlockStartLocation = m_ant_stack_buffer,
          .usMemoryBlockByteSize = ANT_ENABLE_GET_REQUIRED_SPACE(ant_count, 0, 128, 64),
      };

  if (sd_ant_enable(&ant_enable_cfg) != NRF_SUCCESS) return false;

  memset(m_ant_plus_network_key, 0, 8);
#ifdef ANT_PLUS_NETWORK_KEY
  uint8_t ant_plus_network_key[] = ANT_PLUS_NETWORK_KEY;
  memcpy(m_ant_plus_network_key, ant_plus_network_key, 8);
#endif
  if (sd_ant_network_address_set(0, m_ant_plus_network_key) != NRF_SUCCESS) return false;

  // memset(m_ant_fs_network_key, 0, 8);
  // #ifdef ANT_FS_NETWORK_KEY
  //    uint8_t ant_fs_network_key[] = ANT_FS_NETWORK_KEY;
  //    memcpy(m_ant_fs_network_key, ant_fs_network_key, 8);
  //    sd_ant_network_address_set(0, m_ant_fs_network_key);
  // #endif

  uint8_t channel = 0;
  // do setup of registered profiles
  for (ANTProfileEntry *entry = m_profile_list.m_head; entry != NULL; entry = entry->m_next)
  {
    entry->m_entry->Setup(channel);
    channel++;
  }
  // Create RTOS Semaphore & Task for ANT Event
  _ant_event_sem = xSemaphoreCreateBinary();
  if (_ant_event_sem == NULL) return false;

  TaskHandle_t ant_task_hdl;
  xTaskCreate(adafruit_ant_task, "ANT", CFG_ANT_TASK_STACKSIZE, NULL, TASK_PRIO_HIGH, &ant_task_hdl);

  Bluefruit.setANTprotocolSemaphore(&_ant_event_sem);

  return true;
}

void SdAnt::setANTEventCallback(void (*fp)(ant_evt_t *))
{
  _ant_event_cb = fp;
}

/*------------------------------------------------------------------*/
/* ANT Event handler
 *------------------------------------------------------------------*/
void adafruit_ant_task(void *arg)
{
  (void)arg;

  // malloc buffered is algined by 4
  ant_evt_t *ant_evt = (ant_evt_t *)rtos_malloc(sizeof(ant_evt_t));

  while (1)
  {
    if (xSemaphoreTake(ANTplus._ant_event_sem, portMAX_DELAY))
    {
      uint32_t ret = NRF_SUCCESS;
      while (ret == NRF_SUCCESS)
      {
        ret = sd_ant_event_get(&ant_evt->channel, &ant_evt->event, ant_evt->message.aucMessage);
        if (ret == NRF_SUCCESS)
          ANTplus._ant_handler(ant_evt);
      }
    }
  }
}

/**
 * ANT event handler
 * @param evt event
 */
void SdAnt::_ant_handler(ant_evt_t *evt)
{
  for (ANTProfileEntry *entry = m_profile_list.m_head; entry != NULL; entry = entry->m_next)
  {
    entry->m_entry->ProcessMessage(evt);
  }
}

void SdAnt::AddProfile(ANTProfile *p)
{
  m_profile_list.AddProfile(p);
}
