#ifndef BICYCLESPEEDCADENCE_H
#define BICYCLESPEEDCADENCE_H

#include <stdint.h>

#include "../ANTProfile.h"

#define CHAN_ID_DEV_NUM         49                          // <o> CHAN_ID_DEV_NUM - Channel ID: Device Number. 
 
#define BSC_ANTPLUS_RF_FREQ      0x39u                      ///< Frequency, decimal 57 (2457 MHz).

#define BSC_MSG_PERIOD_4Hz       1u                         ///< Message period, 4 Hz (in basic period counts, where basic period time = 0.25 s).
#define BSC_MSG_PERIOD_2Hz       2u                         ///< Message period, 2 Hz (in basic period counts).
#define BSC_MSG_PERIOD_1Hz       4u                         ///< Message period, 1 Hz (in basic period counts).
#define BSC_MSG_PERIOD_SPEED     0x1FB6u                    ///< Message period in ticks, decimal 8118 (4.04 Hz).
#define BSC_MSG_PERIOD_CADENCE   0x1FA6u                    ///< Message period in ticks, decimal 8102 (4.04 Hz).
#define BSC_MSG_PERIOD_COMBINED  0x1F96u                    ///< Message period in ticks, decimal 8086 (4.05 Hz).

#define BSC_EXT_ASSIGN           0x00                       ///< ANT ext assign (see Ext. Assign Channel Parameters in ant_parameters.h: @ref ant_parameters).
#define BSC_DISP_CHANNEL_TYPE    CHANNEL_TYPE_SLAVE_RX_ONLY ///< Display BSC channel type.
#define BSC_SENS_CHANNEL_TYPE    CHANNEL_TYPE_MASTER        ///< Sensor BSC channel type.


#define BSC_MF_ID 2
#define BSC_SERIAL_NUMBER 43981
#define BSC_HW_VERSION 5
#define BSC_SW_VERSION 0
#define BSC_MODEL_NUMBER 2

class BSCPage0
{
public:
   BSCPage0();

   uint16_t GetEventTime() { return event_time; };
   void SetEventTime(uint16_t val) { event_time = val; }

   uint16_t GetRevCount() { return rev_count; };
   void SetRevCount(uint16_t val) { rev_count = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint16_t event_time;            ///< Speed or cadence event time in 1/1024 second units.
    uint16_t rev_count;             ///< Speed or cadence wheel or pedal revolution count.

   /**@brief BSC page 0 data layout structure. */
   typedef struct
   {
      uint8_t reserved[3];
      uint8_t bsc_evt_time_LSB;
      uint8_t bsc_evt_time_MSB;
      uint8_t bsc_rev_count_LSB;
      uint8_t bsc_rev_count_MSB;
   }ant_bsc_page0_data_layout_t;

};

class BSCPage1
{
public:
   BSCPage1();

   uint32_t GetOperatingTime() { return operating_time; };
   void SetOperatingTime(uint32_t val) { operating_time = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint32_t operating_time; ///< Operating time... cumulative operating time in 2 second increments.

   /**@brief BSC page 1 data layout structure. */
   typedef struct
   {
      uint8_t cumulative_operating_time[3];
      uint8_t reserved[4];
   }ant_bsc_page1_data_layout_t;

};

class BSCPage2
{
public:
   BSCPage2();

   uint8_t GetManufacturerID() { return manuf_id; };
   void SetManufacturerID(uint8_t val) { manuf_id = val; }

   uint16_t GetSerialNumber() { return serial_num; };
   void SetSerialNumber(uint16_t val) { serial_num = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
   uint8_t manuf_id; ///< Manufacturer ID.
   uint16_t serial_num; ///< Serial number.

   /**@brief BSC page 2 data layout structure. */
   typedef struct
   {
      uint8_t manuf_id;
      uint8_t serial_num_LSB;
      uint8_t serial_num_MSB;
      uint8_t reserved[4];
   }ant_bsc_page2_data_layout_t;

};

class BSCPage3
{
public:
   BSCPage3();

   uint8_t GetHardwareVersion() { return hw_version; };
   void SetHardwareVersion(uint8_t val) { hw_version = val; }

   uint8_t GetSoftwareVersion() { return sw_version; };
   void SetSoftwareVersion(uint8_t val) { sw_version = val; }

   uint8_t GetModelNumber() { return model_num; };
   void SetModelNumber(uint8_t val) { model_num = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
   uint8_t hw_version; ///< Hardware version.
   uint8_t sw_version; ///< Software version.
   uint8_t model_num;  ///< Model number.

   /**@brief BSC page 3 data layout structure. */
   typedef struct
   {
      uint8_t hw_version;
      uint8_t sw_version;
      uint8_t model_num;
      uint8_t reserved[4];
   }ant_bsc_page3_data_layout_t;

};

class BSCPage4
{
public:
   BSCPage4();

   typedef enum
   {
      RESERVED0               = 0,    ///< Reserved.
      BSC_BAT_STATUS_NEW      = 1,    ///< Battery status: new.
      BSC_BAT_STATUS_GOOD     = 2,    ///< Battery status: good.
      BSC_BAT_STATUS_OK       = 3,    ///< Battery status: ok.
      BSC_BAT_STATUS_LOW      = 4,    ///< Battery status: low.
      BSC_BAT_STATUS_CRITICAL = 5,    ///< Battery status: critical.
      RESERVED1               = 6,    ///< Reserved.
      BSC_BAT_STATUS_INVALID  = 7     ///< Invalid battery status.
   } ant_bsc_bat_status_t;   

   uint8_t GetFractionalBatteryVoltage() { return fract_bat_volt; };
   void SetFractionalBatteryVoltage(uint8_t val) { fract_bat_volt = val; }

   uint8_t GetCoarseBatteryVoltage() { return coarse_bat_volt; };
   void SetCoarseBatteryVoltage(uint8_t val) { coarse_bat_volt = val; }

   ant_bsc_bat_status_t GetBatteryStatus() { return bat_status; };
   void SetBatteryStatus(ant_bsc_bat_status_t val) { bat_status = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint8_t  fract_bat_volt;                 ///< Fractional battery voltage.
    uint8_t  coarse_bat_volt;                ///< Coarse battery voltage.
    ant_bsc_bat_status_t  bat_status;        ///< Battery status.

   /**@brief BSC page 4 data layout structure. */
   typedef struct
   {
      uint8_t reserved_byte;
      uint8_t fract_bat_volt;
      uint8_t coarse_bat_volt   : 4;
      uint8_t bat_status        : 3;
      uint8_t bitfield_reserved : 1;
      uint8_t reserved[4];
   }ant_bsc_page4_data_layout_t;

};

class BSCPage5
{
public:
   BSCPage5();

   uint8_t GetStopIndicator() { return stop_indicator; };
   void SetStopIndicator(uint8_t val) { stop_indicator = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint8_t  stop_indicator;      ///< Stop indication bit. 1 indicates stopped
    uint8_t  reserved;            ///< Reserved.

   /**@brief BSC page 5 data layout structure. */
   typedef struct
   {
      uint8_t flags;
      uint8_t reserved[6];
   }ant_bsc_page5_data_layout_t;
};

class BSCCombinedPage0
{
public:
   BSCCombinedPage0();

   uint16_t GetCadenceEventTime() { return cadence_event_time; };
   void SetCadenceEventTime(uint16_t val) { cadence_event_time = val; }

   uint16_t GetCadenceRevCount() { return cadence_rev_count; };
   void SetCadenceRevCount(uint16_t val) { cadence_rev_count = val; }

   uint16_t GetSpeedEventTime() { return speed_event_time; };
   void SetSpeedEventTime(uint16_t val) { speed_event_time = val; }

   uint16_t GetSpeedRevCount() { return speed_rev_count; };
   void SetSpeedRevCount(uint16_t val) { speed_rev_count = val; }

   void Decode(uint8_t const* buffer);
   void Encode(uint8_t* buffer);


private:
    uint16_t cadence_event_time;          ///< Cadence event time (time of last cadence event).
    uint16_t cadence_rev_count;           ///< Cadence revolution count. (pedal revolution count)
    uint16_t speed_event_time;            ///< Speed event time. (time of last speed event)
    uint16_t speed_rev_count;             ///< Speed revolution count. (count of wheel revolutions)

   /**@brief BSC page 0 data layout structure. */
   typedef struct
   {
      uint8_t cadence_evt_time_LSB;
      uint8_t cadence_evt_time_MSB;
      uint8_t cadence_rev_count_LSB;
      uint8_t cadence_rev_count_MSB;
      uint8_t speed_evt_time_LSB;
      uint8_t speed_evt_time_MSB;
      uint8_t speed_rev_count_LSB;
      uint8_t speed_rev_count_MSB;
   }ant_bsc_combined_page0_data_layout_t;

};

typedef enum{
   Speed = 0x7B,
   Cadence = 0x7A,
   SpeedAndCadence = 0x79
} BSCDeviceType;

class BicycleSpeedCadence : public ANTProfile
{
public:
   BicycleSpeedCadence(BSCDeviceType d_t, ANTTransmissionMode mode);

   void ProcessMessage(ant_evt_t*);

   // void SetSpeed(float val);
   // float GetSpeed() { return m_speed; };

   void AddCadenceRevolution();
   void AddSpeedRevolution();

   void SetWheelCircumference(float len) { m_wheel_circumference = len; };
   float GetDistanceTravelled() { return m_distance_meters; };

   void SetStopped();
private:

   static const uint8_t DeviceNumber = 49;

   float m_wheel_circumference;
   float m_distance_meters;
   float m_speed;
   uint32_t m_time_of_last_cadence;
   uint32_t m_time_of_last_speed;
   uint32_t m_time_of_last_encode;

   /**@brief BSC page number type. */
   typedef enum{
      ANT_BSC_PAGE_0,                         ///< Main data page number 0.
      ANT_BSC_PAGE_1,                         ///< Background data page number 1. This page is optional.
      ANT_BSC_PAGE_2,                         ///< Background data page number 2.
      ANT_BSC_PAGE_3,                         ///< Background data page number 3.
      ANT_BSC_PAGE_4,                         ///< Background data page number 4. This page is optional.
      ANT_BSC_PAGE_5,                         ///< Main data page number 5. This page is optional.
      ANT_BSC_COMB_PAGE_0                     ///< Main data page number 0 for combined speed and cadence sensor.
   } ant_bsc_page_t;

   /**@brief BSC message data layout structure. */
   typedef struct
   {
      ant_bsc_page_t  page_number         : 7;
      uint8_t         toggle_bit          : 1;
      uint8_t         page_payload[7];
   }ant_bsc_single_message_layout_t;

   typedef struct
   {
      uint8_t         page_payload[8];
   }ant_bsc_combined_message_layout_t;

   typedef union
   {
      ant_bsc_single_message_layout_t     speed_or_cadence;
      ant_bsc_combined_message_layout_t   combined;
   }ant_bsc_message_layout_t;


   BSCPage0 page0;
   BSCPage1 page1;
   BSCPage2 page2;
   BSCPage3 page3;
   BSCPage4 page4;
   BSCPage5 page5;

   BSCCombinedPage0 combined_page0;

   BSCDeviceType  device_type; ///< Device type
   bool m_mode;
   bool page_1_present;
   bool page_4_present;
   bool toggle_bit;
   ant_bsc_page_t main_page_number;
   ant_bsc_page_t bkgd_page_number;
   uint8_t         message_counter;


   void SetNextBackgroundPageNumber();
   ant_bsc_page_t GetNextPageNumber();

   void EncodeMessage();
   void DecodeMessage(uint8_t* p_message_payload);

   // void (*_OnComputedHeartRate_cb) (int);
};



#endif