#include "HeartRateMonitor.h"
#include "util.h"

HRMPage0::HRMPage0() :
   beat_count(0),
   computed_heart_rate(0),
   beat_time(0)
{
}

void HRMPage0::Decode(uint8_t const* buffer)
{
    ant_hrm_page0_data_layout_t const * p_incoming_data = (ant_hrm_page0_data_layout_t *)buffer;

    beat_count          = p_incoming_data->heart_beat_count;
    computed_heart_rate = p_incoming_data->computed_heart_rate;
    beat_time           = beat_time;
}

void HRMPage0::Encode(uint8_t* buffer)
{
    ant_hrm_page0_data_layout_t * p_outcoming_data = (ant_hrm_page0_data_layout_t *)buffer;

    p_outcoming_data->reserved[0]             = UINT8_MAX;
    p_outcoming_data->reserved[1]             = UINT8_MAX;
    p_outcoming_data->reserved[2]             = UINT8_MAX;
    p_outcoming_data->heart_beat_evt_time_LSB = (uint8_t)(beat_time & UINT8_MAX);
    p_outcoming_data->heart_beat_evt_time_MSB = (uint8_t)((beat_time >> 8) & UINT8_MAX);
    p_outcoming_data->heart_beat_count        = (uint8_t)beat_count;
    p_outcoming_data->computed_heart_rate     = (uint8_t)computed_heart_rate;
}


HRMPage1::HRMPage1() :
   operating_time(0)
{
}

void HRMPage1::Decode(uint8_t const* buffer)
{
    ant_hrm_page1_data_layout_t const * p_incoming_data = (ant_hrm_page1_data_layout_t *)buffer;

    operating_time          = uint24_decode(p_incoming_data->cumulative_operating_time);
}

void HRMPage1::Encode(uint8_t* buffer)
{
    ant_hrm_page1_data_layout_t * p_outcoming_data = (ant_hrm_page1_data_layout_t *)buffer;

    UNUSED_PARAMETER(uint24_encode(operating_time, p_outcoming_data->cumulative_operating_time));
}


HRMPage2::HRMPage2() :
   manuf_id(0),
   serial_num(0)
{
}

void HRMPage2::Decode(uint8_t const* buffer)
{
    ant_hrm_page2_data_layout_t const * p_incoming_data = (ant_hrm_page2_data_layout_t *)buffer;

    serial_num = (uint32_t)((p_incoming_data->serial_num_MSB << 8) + p_incoming_data->serial_num_LSB);
    manuf_id   = (uint32_t)p_incoming_data->manuf_id;
    serial_num = serial_num;
}

void HRMPage2::Encode(uint8_t* buffer)
{
    ant_hrm_page2_data_layout_t * p_outcoming_data = (ant_hrm_page2_data_layout_t *)buffer;

    p_outcoming_data->manuf_id       = (uint8_t)manuf_id;
    p_outcoming_data->serial_num_LSB = (uint8_t)(serial_num & UINT8_MAX);
    p_outcoming_data->serial_num_MSB = (uint8_t)((serial_num >> 8) & UINT8_MAX);
}

HRMPage3::HRMPage3() :
   hw_version(0),
   sw_version(0),
   model_num(0)
{
}

void HRMPage3::Decode(uint8_t const* buffer)
{
    ant_hrm_page3_data_layout_t const * p_incoming_data = (ant_hrm_page3_data_layout_t *)buffer;

    hw_version = (uint32_t)p_incoming_data->hw_version;
    sw_version = (uint32_t)p_incoming_data->sw_version;
    model_num  = (uint32_t)p_incoming_data->model_num;
}

void HRMPage3::Encode(uint8_t* buffer)
{
    ant_hrm_page3_data_layout_t * p_outcoming_data = (ant_hrm_page3_data_layout_t *)buffer;

    p_outcoming_data->hw_version = (uint8_t)hw_version;
    p_outcoming_data->sw_version = (uint8_t)sw_version;
    p_outcoming_data->model_num  = (uint8_t)model_num;
}

HRMPage4::HRMPage4() :
   manuf_spec(0),
   prev_beat(0)
{
}

void HRMPage4::Decode(uint8_t const* buffer)
{
    ant_hrm_page4_data_layout_t const * p_incoming_data = (ant_hrm_page4_data_layout_t *)buffer;

    manuf_spec = p_incoming_data->manuf_spec;
    prev_beat  = (uint16_t)((p_incoming_data->prev_beat_MSB << 8) + p_incoming_data->prev_beat_LSB);
}

void HRMPage4::Encode(uint8_t* buffer)
{
    ant_hrm_page4_data_layout_t * p_outcoming_data = (ant_hrm_page4_data_layout_t *)buffer;

    p_outcoming_data->manuf_spec    = manuf_spec;
    p_outcoming_data->prev_beat_LSB = (uint8_t)(prev_beat & UINT8_MAX);
    p_outcoming_data->prev_beat_MSB = (uint8_t)((prev_beat >> 8) & UINT8_MAX);
}

HeartRateMonitor::HeartRateMonitor(ANTTransmissionMode mode) :
    ANTProfile(mode)
{
   m_disp_config.channel_type      = HRM_DISP_CHANNEL_TYPE;
   m_disp_config.ext_assign        = HRM_EXT_ASSIGN;
   m_disp_config.rf_freq           = HRM_ANTPLUS_RF_FREQ;
   m_disp_config.transmission_type = 0;
   m_disp_config.device_type       = HRM_DEVICE_TYPE;
   m_disp_config.channel_period    = HRM_MSG_PERIOD_4Hz;
   m_disp_config.network_number    = ANTPLUS_NETWORK_NUMBER;
   m_disp_config.device_number     = 0;


   m_channel_sens_config.channel_type       = HRM_SENS_CHANNEL_TYPE;
   m_channel_sens_config.ext_assign         = HRM_EXT_ASSIGN;
   m_channel_sens_config.rf_freq            = HRM_ANTPLUS_RF_FREQ;
   m_channel_sens_config.transmission_type  = 1;
   m_channel_sens_config.device_type        = HRM_DEVICE_TYPE;
   m_channel_sens_config.channel_period     = HRM_MSG_PERIOD_4Hz;
   m_channel_sens_config.device_number      = DeviceNumber;
   m_channel_sens_config.network_number     = ANTPLUS_NETWORK_NUMBER;


   toggle_bit = true;
   main_page_number = ANT_HRM_PAGE_0;
   page_1_present = true;
   ext_page_number = ANT_HRM_PAGE_1;
   message_counter = 0;

  _OnComputedHeartRate_cb = NULL;
}

/**@brief Function for getting next page number to send.
 *
 * @param[in]  p_profile        Pointer to the profile instance.
 *
 * @return     Next page number.
 */
HeartRateMonitor::ant_hrm_page_t HeartRateMonitor::GetNextPageNumber()
{
   ant_hrm_page_t      page_number;

   if (message_counter == (BACKGROUND_DATA_INTERVAL))
   {
      page_number = ext_page_number;

      message_counter = 0;

      ext_page_number = (ant_hrm_page_t)((int)ext_page_number + 1);

      if (ext_page_number > ANT_HRM_PAGE_3)
      {
         ext_page_number = page_1_present ? ANT_HRM_PAGE_1 : ANT_HRM_PAGE_2;
      }
   }
   else
   {
      page_number = main_page_number;
   }

   if (message_counter % TX_TOGGLE_DIVISOR == 0)
   {
      toggle_bit ^= 1;
   }

   message_counter++;

   return page_number;
}

void HeartRateMonitor::EncodeMessage()
{
    ant_hrm_message_layout_t * p_hrm_message_payload = (ant_hrm_message_layout_t *)m_message_payload;

      p_hrm_message_payload->page_number = GetNextPageNumber();
      p_hrm_message_payload->toggle_bit  = toggle_bit;

      page0.Encode(p_hrm_message_payload->page_payload);

      switch (p_hrm_message_payload->page_number)
      {
         case ANT_HRM_PAGE_0:
               // No implementation needed
               break;
         case  ANT_HRM_PAGE_1:
               page1.Encode(p_hrm_message_payload->page_payload);
               break;
         case ANT_HRM_PAGE_2:
               page2.Encode(p_hrm_message_payload->page_payload);
               break;
         case ANT_HRM_PAGE_3:
               page3.Encode(p_hrm_message_payload->page_payload);
               break;
         case ANT_HRM_PAGE_4:
               page4.Encode(p_hrm_message_payload->page_payload);
               break;
         default:
               // No implementation needed
               break;
      }
}

void HeartRateMonitor::DecodeMessage(uint8_t* buffer)
{
    ant_hrm_message_layout_t* p_hrm_message_payload = (ant_hrm_message_layout_t *)buffer;

    page0.Decode(p_hrm_message_payload->page_payload);
    if (_OnComputedHeartRate_cb != NULL)
    {
        _OnComputedHeartRate_cb(page0.GetComputedHeartRate());
    }

    switch (p_hrm_message_payload->page_number)
    {
        case ANT_HRM_PAGE_0:
            // No implementation needed
            break;

        case ANT_HRM_PAGE_1:
            page1.Decode(p_hrm_message_payload->page_payload);
            break;

        case ANT_HRM_PAGE_2:
            page2.Decode(p_hrm_message_payload->page_payload);
            break;

        case ANT_HRM_PAGE_3:
            page3.Decode(p_hrm_message_payload->page_payload);
            break;

        case ANT_HRM_PAGE_4:
            page4.Decode(p_hrm_message_payload->page_payload);
            break;

        default:
            break;
    }
}