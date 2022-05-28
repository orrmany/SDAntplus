#ifndef RADAR_H
#define RADAR_H

#include <stdint.h>

#include "ANTProfile.h"


#define RADAR_DEVICE_TYPE     0x28u     ///< Device type reserved for ANT+ radar.
#define RADAR_ANTPLUS_RF_FREQ   0x39u     ///< Frequency, decimal 57 (2457 MHz).

#define RADAR_MSG_PERIOD_4Hz    0x1F86u   ///< Message period, decimal 8070 (4.06 Hz).
#define RADAR_MSG_PERIOD_2Hz    0x3F0Cu   ///< Message period, decimal 16140 (2.03 Hz).
#define RADAR_MSG_PERIOD_1Hz    0x7E18u   ///< Message period, decimal 32280 (1.02 Hz).

#define RADAR_EXT_ASSIGN        0x00                  ///< ANT ext assign. FIXME ?
#define RADAR_DISP_CHANNEL_TYPE CHANNEL_TYPE_SLAVE    ///< Display RADAR channel type.

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

class Radar : public ANTProfile
{
public:
   Radar(ANTTransmissionMode mode);

   void ProcessMessage(ant_evt_t*);
   void SetOnThreatLevelsUpdated(void (*fp)(int, int)) { _OnThreatLevelsUpdated_cb = fp;}
   void SetOnThreatDataUpdated(void (*fp)(int, ant_radar_threats_t[4])) { _OnThreatDataUpdated_cb = fp;}

private:

   /**@brief Radar page number type. */
   typedef enum
   {
      ANT_RADAR_THREATS_A = 48,
      ANT_RADAR_THREATS_B = 49
   } ant_radar_page_t;

   /**@brief Radar message data layout structure. */
   typedef struct
   {
      uint8_t          page_id;
      uint8_t          page_payload[7];
   } ant_radar_message_layout_t;

   RadarThreatsPage threats[2];

   void (*_OnThreatLevelsUpdated_cb) (int, int);
   void (*_OnThreatDataUpdated_cb) (int, ant_radar_threats_t[RADAR_NUM_THREATS_PER_PAGE]);

   void EncodeMessage();
   void DecodeMessage(uint8_t* p_message_payload);
};

#endif
