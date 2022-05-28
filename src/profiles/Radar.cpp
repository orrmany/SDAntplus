#include "Radar.h"
#include "util.h"

RadarThreatsPage::RadarThreatsPage() :
   levels(0),
   threats{}
{
}

void RadarThreatsPage::Decode(uint8_t const* buffer) {
    ant_radar_threats_data_layout_t const * p_incoming_data = (ant_radar_threats_data_layout_t *)buffer;

    levels = p_incoming_data->levels;

    threats[0].level = (ant_radar_threat_level_t)((levels & 0xC0) >> 6);
    threats[1].level = (ant_radar_threat_level_t)((levels & 0x30) >> 4);
    threats[2].level = (ant_radar_threat_level_t)((levels & 0x0C) >> 2);
    threats[3].level = (ant_radar_threat_level_t)(levels & 0x03);

    threats[0].side = (ant_radar_threat_side_t)((p_incoming_data->sides & 0xC0) >> 6);
    threats[1].side = (ant_radar_threat_side_t)((p_incoming_data->sides & 0x30) >> 4);
    threats[2].side = (ant_radar_threat_side_t)((p_incoming_data->sides & 0x0C) >> 2);
    threats[3].side = (ant_radar_threat_side_t)(p_incoming_data->sides & 0x03);

    threats[0].range_meters = (p_incoming_data->ranges[0] & 0xFC) >> 2;
    threats[1].range_meters = ((p_incoming_data->ranges[0] & 0x03) << 4) | ((p_incoming_data->ranges[1] & 0xF0) >> 4);
    threats[2].range_meters = ((p_incoming_data->ranges[1] & 0x0F) << 2) | ((p_incoming_data->ranges[2] & 0xC0) >> 6);
    threats[3].range_meters = (p_incoming_data->ranges[1] & 0x3F);

    threats[0].speed_mps = (p_incoming_data->speeds[0] & 0xF0) >> 4;
    threats[1].speed_mps = (p_incoming_data->speeds[0] & 0x0F);
    threats[2].speed_mps = (p_incoming_data->speeds[1] & 0xF0) >> 4;
    threats[3].speed_mps = (p_incoming_data->speeds[1] & 0x0F);
}

//

RadarStatusPage::RadarStatusPage() :
   status(ANT_RADAR_STATUS_ABORTING_SHUTDOWN)
{
}

void RadarStatusPage::Decode(uint8_t const* buffer) {
    ant_radar_status_data_layout_t const * p_incoming_data = (ant_radar_status_data_layout_t *)buffer;

    status = (ant_radar_status_t)(p_incoming_data->status & 0x03);
}

//

RadarVendorInfoPage::RadarVendorInfoPage() :
   info{}
{
}

void RadarVendorInfoPage::Decode(uint8_t const* buffer) {
    ant_radar_vendor_info_data_layout_t const * p_incoming_data = (ant_radar_vendor_info_data_layout_t *)buffer;

    info.hw_revision = p_incoming_data->hw_revision;
    info.manufacturer_id = (uint32_t)((p_incoming_data->manuf_id_HSB << 8) + p_incoming_data->manuf_id_LSB);
    info.model_number = (uint32_t)((p_incoming_data->model_number_HSB << 8) + p_incoming_data->model_number_LSB);
}

//

RadarDeviceInfoPage::RadarDeviceInfoPage() :
   info{}
{
}

void RadarDeviceInfoPage::Decode(uint8_t const* buffer) {
    ant_radar_device_info_data_layout_t const * p_incoming_data = (ant_radar_device_info_data_layout_t *)buffer;

    if (p_incoming_data->sw_version_supplemental == 0xFF) {
      info.version = float(int(p_incoming_data->sw_version_main) / 10);
    } else {
      info.version = float((int(p_incoming_data->sw_version_main) * 100) + int(p_incoming_data->sw_version_supplemental)) / 1000;
    }
    info.serial = (uint32_t)((p_incoming_data->serial[3] << 24) + (p_incoming_data->serial[2] << 16) + (p_incoming_data->serial[1] << 8) + p_incoming_data->serial[0]);
}

//

Radar::Radar(ANTTransmissionMode mode) : ANTProfile(mode) {
   m_disp_config.channel_type      = RADAR_DISP_CHANNEL_TYPE;
   m_disp_config.ext_assign        = RADAR_EXT_ASSIGN;
   m_disp_config.rf_freq           = RADAR_ANTPLUS_RF_FREQ;
   m_disp_config.transmission_type = 0;
   m_disp_config.device_type       = RADAR_DEVICE_TYPE;
   m_disp_config.channel_period    = RADAR_MSG_PERIOD_8Hz;
   m_disp_config.network_number    = ANTPLUS_NETWORK_NUMBER;
   m_disp_config.device_number     = 0;

  vehicles_detected = false;

  _OnVehicleStatusChanged_cb = NULL;
  _OnThreatDataUpdated_cb = NULL;
  _OnStatusUpdated_cb = NULL;
  _OnVendorInfoUpdated_cb = NULL;
  _OnDeviceInfoUpdated_cb = NULL;
}

void Radar::EncodeMessage() {
}

void Radar::DecodeMessage(uint8_t* buffer) {
    ant_radar_message_layout_t* p_radar_message_payload = (ant_radar_message_layout_t *)buffer;

    int decoded_page_threat_index = -1;
    switch (p_radar_message_payload->page_id) {
        case ANT_RADAR_THREATS_A:
            threats[0].Decode(p_radar_message_payload->page_payload);
            decoded_page_threat_index = 0;
            break;

        case ANT_RADAR_THREATS_B:
            threats[1].Decode(p_radar_message_payload->page_payload);
            decoded_page_threat_index = 1;
            break;

        case ANT_RADAR_STATUS:
            statusPage.Decode(p_radar_message_payload->page_payload);
            if (_OnStatusUpdated_cb != NULL) {
              _OnStatusUpdated_cb(statusPage.GetStatus());
            }
            break;

        case ANT_RADAR_VENDOR_INFO:
            vendorInfoPage.Decode(p_radar_message_payload->page_payload);
            if (_OnVendorInfoUpdated_cb != NULL) {
              _OnVendorInfoUpdated_cb(vendorInfoPage.GetInfo());
            }
            break;

        case ANT_RADAR_DEVICE_INFO:
            deviceInfoPage.Decode(p_radar_message_payload->page_payload);
            if (_OnDeviceInfoUpdated_cb != NULL) {
              _OnDeviceInfoUpdated_cb(deviceInfoPage.GetInfo());
            }
            break;

        default:
            break;
    }

    if (decoded_page_threat_index > -1) {
      if (_OnVehicleStatusChanged_cb != NULL) {
        bool gotVehicles = threats[decoded_page_threat_index].GetLevels() > 0;
        if (gotVehicles != vehicles_detected) {
          vehicles_detected = gotVehicles;
          _OnVehicleStatusChanged_cb(vehicles_detected);
        }
      }
      
      if (_OnThreatDataUpdated_cb != NULL) {
        ant_radar_threats_t thr[RADAR_NUM_THREATS_PER_PAGE];
        for(int i = 0; i < RADAR_NUM_THREATS_PER_PAGE; i ++) thr[i] = threats[decoded_page_threat_index].GetThreats(i);
        _OnThreatDataUpdated_cb(decoded_page_threat_index, thr);
      }

    }

}
