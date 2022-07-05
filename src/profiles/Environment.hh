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

#ifndef ANT_ENVIRONMENT_PROFILE_H
#define ANT_ENVIRONMENT_PROFILE_H

// Based on "ANT+ Device Profile Environment", D00001502 Rev 1.0
// thisisant.com

#include <stdint.h>

#include "../ANTProfile.h"


#define ENV_CHANNEL_NUM 1
#define CHAN_ID_TRANS_TYPE 0
#define CHAN_ID_DEV_NUM 0
#define ANTPLUS_NETWORK_NUM 0
#define ENV_DISP_CHANNEL_TYPE CHANNEL_TYPE_SLAVE    ///< Display HRM channel type.
#define ENV_SENS_CHANNEL_TYPE CHANNEL_TYPE_MASTER   ///< Sensor HRM channel type.


#define ENV_DEVICE_TYPE       0x19u     ///< 25 (0x19) – indicates search for an ANT+ environment device.
#define ENV_ANTPLUS_RF_FREQ   0x39u     ///< Frequency, decimal 57 (2457 MHz).
#define ENV_EXT_ASSIGN        0x00      ///< ANT ext assign.

#define ENV_MSG_PERIOD_4Hz    0x2000u   ///< Message period, decimal 8192 (32768/8192=4 Hz).
#define ENV_MSG_PERIOD_0p5Hz  0xFFFFu   ///< Message period, decimal 65535 (32768/65535=0.5 Hz).

#define ENV_SEARCH_TIMEOUT    45        ///< The recommended search timeout is set to 45 seconds in the receiver. This allows sufficient time for a 0.5 Hz master to be found. This timeout is implementation specific and can be set by the designer to the appropriate value for the system.

//bitfield access
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)
#define BITMASK_1B 0x01 // 0000 0001
#define BITMASK_2B 0x03 // 0000 0011
#define BITMASK_3B 0x07 // 0000 0111
#define BITMASK_4B 0x0F // 0000 1111
#define BITMASK_LSN 0x0F // Least Significant Nibble
#define BITMASK_MSN 0xF0 // Most Signifnicant Nibble
#define BITMASK_BITS_1_0 0x03 // 0000 0011
#define BITMASK_BITS_3_2 0x0C // 0000 1100
#define BITMASK_BITS_5_4 0x30 // 0011 0000
#define BITMASK_BITS_7_6 0xC0 // 1100 0000
#define BITSHIFT_MSN 4 // <<4
#define BITSHIFT_BYTE 8 // <<4
#define BITMASK_BYTE UINT8_MAX

//Table 6-3. Transmission Info Bit Field Description
#define TRANSMISSION_INFO_BITS_RESERVED 0 //6:7 Reserved Set to 0.
#define TRANSMISSION_INFO_BITS_LOCAL_TIME_NotSupported    0              //4:5 Local Time  00 – Local Time Not Supported 
#define TRANSMISSION_INFO_BITS_LOCAL_TIME_SupportedNotSet BIT0           //4:5 Local Time  01 – Local Time Supported, Not Set 
#define TRANSMISSION_INFO_BITS_LOCAL_TIME_SupportedSet    BIT1           //4:5 Local Time  10 – Local Time Supported and Set 
#define TRANSMISSION_INFO_BITS_LOCAL_TIME_Reserved        (BIT1 | BIT0)  //4:5 Local Time  11 – Reserved 
#define TRANSMISSION_INFO_BITS_LOCAL_TIME_bitshift        4              //4:5 Local Time field - 4 bits shift
#define TRANSMISSION_INFO_BITS_LOCAL_TIME_bitmask      BITMASK_BITS_5_4  //4:5 Local Time field - 4 bits shift
#define TRANSMISSION_INFO_BITS_UTC_TIME_NotSupported      0              //2:3 UTC Time  00 – System Time  Not Supported 
#define TRANSMISSION_INFO_BITS_UTC_TIME_SupportedNotSet   BIT0           //2:3 UTC Time  01 – UTC Supported, Not Set 
#define TRANSMISSION_INFO_BITS_UTC_TIME_SupportedSet      BIT1           //2:3 UTC Time  10 – UTC  Supported and Set 
#define TRANSMISSION_INFO_BITS_UTC_TIME_Reserved          (BIT1 | BIT0)  //2:3 UTC Time  11 – Reserved 
#define TRANSMISSION_INFO_BITS_UTC_TIME_bitshift          2              //2:3 UTC Time field - 2  bits shift
#define TRANSMISSION_INFO_BITS_UTC_TIME_bitmask         BITMASK_BITS_3_2 //2:3 Local Time field - 4 bits shift
#define TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_0p5Hz        0             //0:1 Default Transmission Rate 00 – default transmission rate of 0.5 Hz
#define TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_4Hz          BIT0          //0:1 Default Transmission Rate 01 – default transmission rate of 4 Hz
#define TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_Reserved10   BIT1          //0:1 Default Transmission Rate 10 – Reserved
#define TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_Reserved11   (BIT1 | BIT0) //0:1 Default Transmission Rate 10 – Reserved
#define TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitshift     0             //0:1 Default Transmission Rate field  – no bitshift
#define TRANSMISSION_INFO_BITS_DEFAULT_TRANSMISSION_RATE_bitmask   BITMASK_BITS_1_0  //4:5 Local Time field - 4 bits shift


//Table 6-2. Data Page 0 Format – General Information
#define SUPPORTED_ENVIRONMENT_DATA_PAGES_0  BIT0 // Data page 0: General info
#define SUPPORTED_ENVIRONMENT_DATA_PAGES_1  BIT1 // Data page 1: Temperature

#define DATA_PAGE_NUMBER0 = 0x00
#define DATA_PAGE_NUMBER1 = 0x01

typedef enum {
        not_supported,     // 00
        supported_not_set, // 01
        supported_and_set, // 10
        reserved           // 00
    } ant_env_time_support_t;

typedef enum {
        half_Hz,  // 0.5 Hz
        four_Hz,   // 4 Hz
        reserved10,  //reserved
        reserved11  //reserved
} ant_env_transmission_rate_t;


// 6.3 Data Page 0 – General Information
class EnvironmentPage0 {
    public:
    EnvironmentPage0(ant_env_transmission_rate_t p_default_transmission = four_Hz);

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);

    ant_env_time_support_t GetLocalTimeSupport() { return local_time_support; } ;
    void SetLocalTimeSupport(ant_env_time_support_t p_support) { local_time_support = p_support; } ;

    ant_env_time_support_t GetUtcTimeSupport() { return utc_time_support; } ;
    void SetUtcTimeSupport(ant_env_time_support_t p_support) { utc_time_support = p_support; } ;

    bool GetPage1Support() { return page1_support; };
    void SetPage1Support(bool p) { page1_support = p; };

    private:
    //const uint8_t page_number =0;             //page number for general info
    ant_env_time_support_t local_time_support; //support status for local time
    ant_env_time_support_t utc_time_support;   //support status for local time
    ant_env_transmission_rate_t default_transmission_rate;
    //bool page0_support; //mandatory
    bool page1_support; //optional


    typedef struct 
    {
        //uint8_t page_number; //byte #0
        uint8_t reserved[2]; //bytes #1-#2
        uint8_t transmission_info; //byte #3
        uint8_t supported_pages[4]; /* byte #4: [0]=LSB
                                    *  ...
                                    *  byte #7: [3]=MSB
                                    * */
    } ant_env_page0_data_layout_t;
    
};

//6.4 Data Page 1 – Temperature
class EnvironmentPage1 {
    public:
    EnvironmentPage1();
    uint8_t GetEventCount(void) {return p_event_count;};
    void SetEventCout(uint8_t p) {p_event_count = p ;};
    int16_t GetTempMin() {return p_temp_min; };
    void SetTempMin(int16_t p) {p_temp_min=p;}; 
    int16_t GetTempMax() {return p_temp_max; };
    void SetTempMax(int16_t p) {p_temp_max=p; } 
    int16_t GetCurrTemp() {return p_temp_current; };
    void SetCurrTemp(int16_t p) {p_temp_current=p; } 

    void Decode(uint8_t const* buffer);
    void Encode(uint8_t* buffer);

    private:
    uint8_t p_event_count = 0;
    int16_t p_temp_min = 0;
    int16_t p_temp_max = 0;
    int16_t p_temp_current = 0;


    typedef struct 
    {
        //uint8_t page_number; //byte #0
        uint8_t reserved; //byte #1
        uint8_t event_count; // byte #2
        uint8_t temp_Min_Max24h[3];   // byte #3 & byte #4 MSN: 24h Temp. Min. -204.7..+204.7; 0x800 invalid
                                    // byte #4 LSN & byte #5: 24h Temp. Max. -204.7..+204.7; 0x800 invalid
        uint8_t current_temp_lsb; //bytes #6-#7: Signed Integer representing the current Temperature (0x8000 invalid)
        uint8_t current_temp_msb;
    } ant_env_page1_data_layout_t;
};

class EnvironmentSensor : public ANTProfile
{
private:
    //uint8_t channel_number;
    static const uint8_t DeviceNumber =1; //TODO

    #if false 
    //NOTE: Ryan used an enum here, but enums are NOT guaranteed to be uint8_t equivalent!!! 
    typedef enum {
        ANT_ENV_PAGE_0, // Data Page 0 – General Information
        ANT_ENV_PAGE_1  // Data Page 1 – Temperature
    } ant_env_page_t;
    #endif
    //#define ANT_ENV_PAGE_0 0  // Data Page 0 – General Information
    //#define ANT_ENV_PAGE_1 1  // Data Page 1 – Temperature

    typedef enum {
        ANT_ENV_PAGE_0,
        ANT_ENV_PAGE_1
    } ant_env_page_t;
    
    typedef struct 
    {
        uint8_t page_number; //common for all pages
        uint8_t page_payload[7];
    } ant_env_message_layout_t;
    

    EnvironmentPage0 page0;
    EnvironmentPage1 page1;

    void (*_OnTemperatureData_cb) (int16_t);
    ant_channel_config_t m_ant_env_channel_env_disp_config;
    void EncodeMessage();
    void DecodeMessage(uint8_t* p_message_payload);

public:
    EnvironmentSensor(ANTTransmissionMode mode);
    //~EnvironmentRx();
    //bool Setup();
    void ProcessMessage(ant_evt_t*) override;
    void SetOnTemperatureData(void (*fp)(int16_t)) {_OnTemperatureData_cb = fp;}
};


#endif //#ifndef ANT_ENVIRONMENT_PROFILE_H

