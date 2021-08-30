#ifndef BICYCLEPOWER_H
#define BICYCLEPOWER_H

#include <stdint.h>

#include "../ANTProfile.h"

#define PWR_DEVICE_TYPE             0x0Bu     ///< Device type reserved for transmitting ANT+ Power.
#define PWR_DEVICE_NUMBER           0x03E8u   //1000 in hex
#define PWR_RF_CHANNEL              0x39u     ///< Frequency, decimal 57 (2457 MHz).

//#define PWR_MSG_PERIOD_8Hz        0x1F86u   ///< Message period, decimal 8070 (4.06 Hz).
#define PWR_MSG_PERIOD_4Hz          0x1FF6u   ///< Message period, decimal 8182 (4.0049 Hz).
//#define PWR_MSG_PERIOD_2Hz        0x3F0Cu   ///< Message period, decimal 16140 (2.03 Hz).
//#define PWR_MSG_PERIOD_1Hz        0x7E18u   ///< Message period, decimal 32280 (1.02 Hz).

#define PWR_EXT_ASSIGN              0x00                  ///< ANT ext assign.
#define PWR_DISP_CHANNEL_TYPE       CHANNEL_TYPE_SLAVE    ///< Display HRM channel type.
#define PWR_SENS_CHANNEL_TYPE       CHANNEL_TYPE_MASTER   ///< Sensor HRM channel type.
#define PWR_TRANSMISSION_TYPE       0x05      //No shared channel (MSN 0x0 cause no extended Device number LSN 0x5)

class PWRPage10
{
public:
    PWRPage10();

    uint8_t GetPWREventCount() { return pwr_event_count; }
    void SetPWREventCount(uint8_t val) { pwr_event_count = val; }
    
    uint8_t GetPedalPWR() { return pedal_pwr; }
    void SetPedalPWR(uint8_t val) { pedal_pwr = val; }

    uint8_t GetInstandCadence() { return instant_cadence; }
    void SetInstantCadence(uint8_t val) { instant_cadence = val; }

    uint16_t GetAccumulatedPWR() { return accumulated_pwr; }
    void SetAccumulatedPWR(uint16_t val) { accumulated_pwr = val; }

    uint16_t GetInstantPWR() { return instant_pwr; }
    void SetInstantPWR(uint16_t val) { instant_pwr = val; }

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    //Space for variables also in struct
    uint8_t pwr_event_count;
    uint8_t pedal_pwr; //0xFF for not used
    uint8_t instant_cadence; //0xFF for invalid or cadence value in rpm
    uint16_t accumulated_pwr;
    uint16_t instant_pwr;

    typedef struct //Has to be 7 bytes long
    {
        uint8_t pwr_event_count;
        uint8_t pedal_pwr; //0xFF for not used
        uint8_t instant_cadence; //0xFF for invalid or cadence value in rpm
        uint8_t accumulated_pwr_lsb;
        uint8_t accumulated_pwr_msb;
        uint8_t instant_pwr_lsb;
        uint8_t instant_pwr_msb;
    } ant_pwr_page10_data_layout_t;
};

class PWRPage50 //i.e. 0x50 instead of 80
{
public:
    PWRPage50();

    uint8_t GetHwRevision() { return hw_revision; }
    void SetHwRevision(uint8_t val) { hw_revision = val; }

    uint16_t GetManufacturerID() { return manufacturer_id; }
    void SetManufacturerID(uint16_t val) { manufacturer_id = val; }

    uint16_t GetModelNumber() { return model_number; }
    void SetModelNumber(uint16_t val) { model_number = val; }

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    //Space for variables also in struct
    uint8_t hw_revision;
    uint16_t manufacturer_id;
    uint16_t model_number;

    typedef struct //Has to be 7 bytes long
    {
        uint8_t reserved[2]; //0xFF
        uint8_t hw_revision;
        uint8_t manufacturer_id_lsb;
        uint8_t manufacturer_id_msb;
        uint8_t model_number_lsb;
        uint8_t model_number_msb;
    } ant_pwr_page50_data_layout_t;
};

class PWRPage51
{
public:
    PWRPage51();

    uint8_t GetSWRevisionSuppl() { return sw_revision_supplemental; }
    void SetSWRevisionSuppl(uint16_t val) { sw_revision_supplemental = val; }

    uint8_t GetSWRevisionMain() { return sw_revision_main; }
    void SetSWRevisionMain(uint8_t val) { sw_revision_main = val; }

    uint32_t GetSerialNumber() { return serial_number; }
    void SetSerialNumber(uint32_t val) { serial_number = val; }

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    //Space for variables also in struct
    uint8_t sw_revision_supplemental; //0xFF
    uint8_t sw_revision_main;
    uint32_t serial_number;

    typedef struct //Has to be 7 bytes long
    {
        uint8_t reserved; //0xFF
        uint8_t sw_revision_supplemental; //0xFF
        uint8_t sw_revision_main;
        uint8_t serial_number[4]; //all 0xFF if not defined
    } ant_pwr_page51_data_layout_t;
};

class PWRPage01 //Calibration Page
{
public:
    PWRPage01();

    uint8_t GetCalibrationID() { return calibration_id; }
    void SetCalibrationID(uint8_t val) { calibration_id = val; }

    uint8_t GetAutoZeroStatus() { return auto_zero_status; }
    void SetAutoZeroStatus(uint8_t val) { auto_zero_status = val; }

    uint16_t GetCalibrationData() { return calibration_data; }
    void SetCalibrationData(uint16_t val) { calibration_data = val; }

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    //Space for variables also in struct
    uint8_t calibration_id;
    uint8_t auto_zero_status; //0x00 OFF    0x01 ON     0xFF Auto Zero Not Supported ----> Should be OFF 0x00
    uint16_t calibration_data;

    typedef struct //Has to be 7 bytes long
    {
        uint8_t calibration_id;
        uint8_t auto_zero_status; //0x00 OFF    0x01 ON     0xFF Auto Zero Not Supported ----> Should be OFF 0x00
        uint8_t reserved[3];
        uint8_t calibration_data_lsb;
        uint8_t calibration_data_msb;
    } ant_pwr_page01_data_layout_t;
};

class PWRPage46 //Request Page Page
{
public:
    PWRPage46();

    uint8_t GetRequestedPageNumber() { return requested_page_number; }
    void SetRequestedPageNumber(uint8_t val) { requested_page_number = val; }
    
    uint8_t GetRequestedResponse() { return requested_transmission_response; }
    uint8_t GetRequestedNumberOfResponses() { return requested_transmission_response & 0xEF; }
    void SetRequestedResponse(uint8_t val) { requested_transmission_response = val; }

    uint8_t GetDescriptorByte1() { return descriptor_byte_1; }

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    uint8_t reserved[2];
    uint8_t descriptor_byte_1;
    uint8_t descriptor_byte_2;
    uint8_t requested_transmission_response;
    uint8_t requested_page_number;
    uint8_t command_type;
    typedef struct //Has to be 7 bytes long
    {
        uint8_t reserved[2];
        uint8_t descriptor_byte_1;
        uint8_t descriptor_byte_2;
        uint8_t requested_transmission_response;
        uint8_t requested_page_number;
        uint8_t command_type;
    } ant_pwr_page46_data_layout_t;
};

class PWRPage56 //Paired Devices Page
{
public:
    PWRPage56();

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    uint8_t peripheral_device_index;
    uint8_t total_number_devices;
    uint8_t channel_state;
    uint16_t peripheral_device_number;
    uint8_t peripheral_device_transmission_type;
    uint8_t peripheral_device_type;
    typedef struct //Has to be 7 bytes long
    {
        uint8_t peripheral_device_index;
        uint8_t total_number_devices;
        uint8_t channel_state;
        uint8_t peripheral_device_number_lsb;
        uint8_t peripheral_device_number_msb;
        uint8_t peripheral_device_transmission_type;
        uint8_t peripheral_device_type;
    } ant_pwr_page56_data_layout_t;
};

class PWRPage52 //Battery Page
{
public:
    PWRPage52();

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    uint8_t reserved;
    uint8_t battery_identifier;
    uint32_t cumulative_operating_time;
    uint8_t fractional_battery_voltage;
    uint8_t descriptive_bitfield;
    typedef struct //Has to be 7 bytes long
    {
        uint8_t reserved;
        uint8_t battery_identifier;
        uint8_t cumulative_operating_time[3];
        uint8_t fractional_battery_voltage;
        uint8_t descriptive_bitfield;
    } ant_pwr_page52_data_layout_t;
};

class PWRPage02 //Get/Set Parameters Page
{
public:
    PWRPage02();

    uint8_t GetSubpageNumber() { return subpage_number; }
    void SetSubPageNumber(uint8_t val) { subpage_number = val; }

    uint8_t GetSubpageData(uint8_t index) { return subpage_data[index]; }
    void SetSubpageData(uint8_t index, uint8_t val) { subpage_data[index] = val; }

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);
private:
    uint8_t subpage_number;
    uint8_t subpage_data[6];
    typedef struct //Has to be 7 bytes long
    {
        uint8_t subpage_number;
        uint8_t subpage_data[6];
    } ant_pwr_page02_data_layout_t;
};

class BicyclePower : public ANTProfile
{
public:
    BicyclePower(ANTTransmissionMode mode);

    void ProcessMessage(ant_evt_t*);

    void SetInstantPWR(uint16_t val) { page10.SetInstantPWR(val); }
    void SetAccumulatedPWR(uint16_t val) { page10.SetAccumulatedPWR(val); }
    void SetPWREventCount(uint8_t val) { page10.SetPWREventCount(val); }
    void SetInstantCadence(uint8_t val) { page10.SetInstantCadence(val); }

private:

    typedef enum
    {
        ANT_PWR_PAGE_01 = 1, ///< Manual Calibration Data Page 0x01.
        ANT_PWR_PAGE_10 = 0x10, ///< Power Main data page number 0x10. 
        ANT_PWR_PAGE_50 = 0x50, ///< Req. Common data page number 0x50. Manufacturer Info
        ANT_PWR_PAGE_51 = 0x51, ///< Req. Common data page number 0x51. Product Info
        ANT_PWR_PAGE_52 = 0x52, ///< Optional Common data page number 0x52. Battery Voltage
        ANT_PWR_PAGE_46 = 0x46,
        ANT_PWR_PAGE_56 = 0x56,
        ANT_PWR_PAGE_02 = 0x02
    } ant_pwr_page_t;

    typedef struct
    {
        ant_pwr_page_t page_number;
        uint8_t        page_payload[7];
    } ant_pwr_message_layout_t;

    PWRPage10 page10;
    PWRPage50 page50;
    PWRPage51 page51;
    PWRPage01 page01;
    PWRPage46 page46;
    PWRPage52 page52;
    PWRPage56 page56;
    PWRPage02 page02;

    ant_pwr_page_t GetNextPageNumber();

    void EncodeMessage();
    void DecodeMessage(uint8_t* p_message_payload);


    //uint8_t        toggle_bit;
    ant_pwr_page_t  main_page_number;
    ant_pwr_page_t  bkgd_page0_number;
    ant_pwr_page_t  bkgd_page1_number;
    ant_pwr_page_t  bkgd_page2_number;
    ant_pwr_page_t  bkgd_page3_number;
    ant_pwr_page_t  calibration_page_number;
    uint8_t         message_counter;
    uint8_t         background_page_offset;
    uint8_t         non_main_messages;
    uint8_t         cal_id;
    uint8_t         requested_page;
    uint8_t         requested_subpage;
    bool            need_answer;
    bool            calibration_request;
    
    // uint8_t        page_52_present;
    // ant_pwr_page_t ext_page_number;
    
};
#endif


/*
TODO:

Page 0x52
Page 0x56
Page 0x46

GetNextPageNumber anpassen

(Page 0x02)



*/