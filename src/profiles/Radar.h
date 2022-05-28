#ifndef RADAR_H
#define RADAR_H

#include <stdint.h>

#include "../ANTProfile.h"


#define RADAR_DEVICE_TYPE       40     // Device type reserved for ANT+ radar
#define RADAR_ANTPLUS_RF_FREQ   57     // Frequency, 2457 MHz
#define RADAR_MSG_PERIOD_8Hz    0xFF4u // Message period, ~8hz

#define RADAR_EXT_ASSIGN        0x00                          // ANT ext assign
#define RADAR_DISP_CHANNEL_TYPE CHANNEL_TYPE_SLAVE_RX_ONLY    // Display RADAR channel type.

#define RADAR_NUM_THREATS_PER_PAGE 4

typedef enum {
  ANT_RADAR_NO_THREAT = 0,
  ANT_RADAR_VEHICLE_APPROACH = 1,
  ANT_RADAR_VEHCILE_FAST_APPROACH = 2,
  ANT_RADAR_RESERVED_THREAT = 3
} ant_radar_threat_level_t;

typedef enum {
  ANT_RADAR_NO_SIDE = 0,
  ANT_RADAR_RIGHT_SIDE = 1,
  ANT_RADAR_LEFT_SIDE = 2,
  ANT_RADAR_RESERVED_SIDE = 3
} ant_radar_threat_side_t;

typedef struct {
  ant_radar_threat_level_t level;
  ant_radar_threat_side_t side;
  uint32_t range_meters;
  uint32_t speed_mps; // meters per second
} ant_radar_threats_t;

class RadarThreatsPage
{
public:
   RadarThreatsPage();

   uint8_t GetLevels() { return levels; };
   ant_radar_threats_t GetThreats(int index) { return threats[index]; };

   void Decode(uint8_t const* buffer);

private:
    uint8_t  levels;
    ant_radar_threats_t threats[RADAR_NUM_THREATS_PER_PAGE];

   /**@brief Radar threats data layout structure. */
   typedef struct
   {
      uint8_t levels;
      uint8_t sides;
      uint8_t ranges[3];
      uint8_t speeds[2];
   }ant_radar_threats_data_layout_t;

};

typedef enum {
  ANT_RADAR_STATUS_ABORTING_SHUTDOWN = 0,
  ANT_RADAR_STATUS_SHUTDOWN = 1
} ant_radar_status_t;

class RadarStatusPage {
public:
   RadarStatusPage();
   ant_radar_status_t GetStatus() { return status; };
   void Decode(uint8_t const* buffer);

private:
    ant_radar_status_t status;

   typedef struct
   {
      uint8_t status;
      uint8_t reserved[6];
   }ant_radar_status_data_layout_t;
};

typedef struct
{
  uint8_t hw_revision;
  uint16_t manufacturer_id;
  uint16_t model_number;
}ant_radar_vendor_info_t;

class RadarVendorInfoPage {
public:
   RadarVendorInfoPage();
   ant_radar_vendor_info_t GetInfo() { return info; };
   void Decode(uint8_t const* buffer);

private:
    ant_radar_vendor_info_t info;

   typedef struct
   {
      uint8_t reserved[2];
      uint8_t hw_revision;
      uint8_t manuf_id_LSB;
      uint8_t manuf_id_HSB;
      uint8_t model_number_LSB;
      uint8_t model_number_HSB;
   }ant_radar_vendor_info_data_layout_t;
};

typedef struct
{
  float version;
  uint32_t serial;
}ant_radar_device_info_t;

class RadarDeviceInfoPage {
public:
   RadarDeviceInfoPage();
   ant_radar_device_info_t GetInfo() { return info; };
   void Decode(uint8_t const* buffer);

private:
    ant_radar_device_info_t info;

   typedef struct
   {
      uint8_t reserved;
      uint8_t sw_version_supplemental;
      uint8_t sw_version_main;
      uint8_t serial[4];
   }ant_radar_device_info_data_layout_t;
};

class Radar : public ANTProfile
{
public:
   Radar(ANTTransmissionMode mode);

   void ProcessMessage(ant_evt_t*);
   void SetOnVehicleStatusChanged(void (*fp)(bool)) { _OnVehicleStatusChanged_cb = fp; }
   void SetOnThreatDataUpdated(void (*fp)(int, ant_radar_threats_t[4])) { _OnThreatDataUpdated_cb = fp; }
   void SetOnStatusUpdated(void (*fp)(ant_radar_status_t)) { _OnStatusUpdated_cb = fp; }
   void SetOnVendorInfoUpdated(void (*fp)(ant_radar_vendor_info_t)) { _OnVendorInfoUpdated_cb = fp; }
   void SetOnDeviceInfoUpdated(void (*fp)(ant_radar_device_info_t)) { _OnDeviceInfoUpdated_cb = fp; }

private:

   /**@brief Radar page number type. */
   typedef enum
   {
      ANT_RADAR_THREATS_A = 48,
      ANT_RADAR_THREATS_B = 49,
      ANT_RADAR_STATUS = 1,
      ANT_RADAR_VENDOR_INFO = 80,
      ANT_RADAR_DEVICE_INFO = 81
//      ANT_RADAR_BATT_INFO = 82
   } ant_radar_page_t;

   /**@brief Radar message data layout structure. */
   typedef struct
   {
      uint8_t          page_id;
      uint8_t          page_payload[7];
   } ant_radar_message_layout_t;

   RadarThreatsPage threats[2];
   RadarStatusPage statusPage;
   RadarVendorInfoPage vendorInfoPage;
   RadarDeviceInfoPage deviceInfoPage;

   bool vehicles_detected;

   void (*_OnVehicleStatusChanged_cb) (bool);
   void (*_OnThreatLevelsUpdated_cb) (int, int);
   void (*_OnThreatDataUpdated_cb) (int, ant_radar_threats_t[RADAR_NUM_THREATS_PER_PAGE]);
   void (*_OnStatusUpdated_cb) (ant_radar_status_t);
   void (*_OnVendorInfoUpdated_cb) (ant_radar_vendor_info_t);
   void (*_OnDeviceInfoUpdated_cb) (ant_radar_device_info_t);

   void EncodeMessage();
   void DecodeMessage(uint8_t* p_message_payload);
};


#endif
