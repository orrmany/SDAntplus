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

#ifndef ANT_SDM_H
#define ANT_SDM_H

#include <stdint.h>
//#include <stdbool.h>
#include "../ANTProfile.h"

/* from SDK app_util.h*/
/**@brief Macro for performing rounded integer division (as opposed to truncating the result).
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Rounded (integer) result of dividing A by B.
 */
#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))
/**@brief Function for changing the value unit.
 *
 * @param[in]   value               Value to be rescaled.
 * @param[in]   old_unit_reversal   Reversal of the incoming unit.
 * @param[in]   new_unit_reversal   Reversal of the desired unit.
 *
 * @return      Number of bytes written.
 */
static inline uint64_t value_rescale(uint32_t value, uint32_t old_unit_reversal, uint16_t new_unit_reversal)
{
    return (uint64_t)ROUNDED_DIV((uint64_t)value * new_unit_reversal, old_unit_reversal);
};



//from NRF SDK ant_sdm profile
#define SDM_DEVICE_TYPE       0x7Cu                                         ///< Device type reserved for ANT+ SDM.
#define SDM_ANTPLUS_RF_FREQ   0x39u                                         ///< Frequency, decimal 57 (2457 MHz).

//#define SDM_MSG_PERIOD_4Hz    0x1FC6u                                       ///< Message period, decimal 8134 (4.03 Hz).
//#define SDM_MSG_PERIOD_2Hz    0x3F8Cu                                       ///< Message period, decimal 16268 (2.01 Hz).

#define SDM_EXT_ASSIGN        0x00                                          ///< ANT ext assign (see Ext. Assign Channel Parameters in ant_parameters.h: @ref ant_parameters).
#define SDM_DISP_CHANNEL_TYPE CHANNEL_TYPE_SLAVE                            ///< RX SDM channel type.
#define SDM_SENS_CHANNEL_TYPE CHANNEL_TYPE_MASTER                           ///< TX SDM channel type.

#define SDM_MSG_PERIOD_4Hz    0x1fc6u   ///< Message period, decimal 8134 (32768/8134=4,03 Hz).
#define SDM_MSG_PERIOD_2Hz    0x3f8cu   ///< Message period, decimal 65535 (32768/16268=2.01 Hz).

/*@brief A reversal of time unit.
 *
 * @details According to the ANT SDM specification, the time unit (fractional part) is 1/200 of a second.
 */
#define ANT_SDM_TIME_UNIT_REVERSAL              200
#define ANT_SDM_TIME_DISP_PRECISION             1000
#define ANT_SDM_TIME_RESCALE(VALUE)             value_rescale((VALUE), ANT_SDM_TIME_UNIT_REVERSAL,  \
                                                                 ANT_SDM_TIME_DISP_PRECISION)
#define ANT_SDM_TIME_ROLLOVER                   256

/*@brief A reversal of distance unit.
 *
 * @details According to the ANT SDM specification, the distance unit (page 1 - fractional part) is 1/16 of a meter.
 */
#define ANT_SDM_DISTANCE_UNIT_REVERSAL          16
#define ANT_SDM_DISTANCE_DISP_PRECISION         10
#define ANT_SDM_DISTANCE_RESCALE(VALUE)         value_rescale((VALUE), ANT_SDM_DISTANCE_UNIT_REVERSAL,  \
                                                                 ANT_SDM_DISTANCE_DISP_PRECISION)
#define ANT_SDM_DISTANCE_ROLLOVER               256

/*@brief A reversal of speed unit.
 *
 * @details According to the ANT SDM specification, the speed unit (fractional part) is 1/256 of m/s.
 */
#define ANT_SDM_SPEED_UNIT_REVERSAL             256
#define ANT_SDM_SPEED_DISP_PRECISION            100
#define ANT_SDM_SPEED_RESCALE(VALUE)            value_rescale((VALUE), ANT_SDM_SPEED_UNIT_REVERSAL,  \
                                                                 ANT_SDM_SPEED_DISP_PRECISION)

#define ANT_SDM_STRIDE_ROLLOVER                 256

/*@brief A reversal of update latency unit.
 *
 * @details According to the ANT SDM specification, the update latency unit (fractional part) is 1/32 of a second.
 */
#define ANT_SDM_UPDATE_LATENCY_UNIT_REVERSAL    32
#define ANT_SDM_UPDATE_LATENCY_DISP_PRECISION   1000
#define ANT_SDM_UPDATE_LATENCY_RESCALE(VALUE)   value_rescale((VALUE), ANT_SDM_UPDATE_LATENCY_UNIT_REVERSAL,  \
                                                                 ANT_SDM_UPDATE_LATENCY_DISP_PRECISION)

/*@brief A reversal of cadence unit.
 *
 * @details According to the ANT SDM specification, the cadence unit (fractional part) is 1/16 of strides/minute.
 */
#define ANT_SDM_CADENCE_UNIT_REVERSAL           16
#define ANT_SDM_CADENCE_DISP_PRECISION          10
#define ANT_SDM_CADENCE_RESCALE(VALUE)          value_rescale((VALUE), ANT_SDM_CADENCE_UNIT_REVERSAL,  \
                                                                 ANT_SDM_CADENCE_DISP_PRECISION)

#define GET_LOWER_NIBBLE(x) ( x & 0x0F)
#define GET_UPPER_NIBBLE(x) ( (x >> 4)  & 0x0F)


/**@brief SDM page number type. */
typedef enum{
    ANT_SDM_PAGE_1  = 1,    ///< Required data page 1.
    ANT_SDM_PAGE_2  = 2,    ///< Supplementary data page 2.
    ANT_SDM_PAGE_3  = 3,    ///< Supplementary data page 3.
    ANT_SDM_PAGE_16 = 16,   ///< Page 16 (sent on request).
    ANT_SDM_PAGE_22 = 22,   ///< Page 22 (sent on request).
    //ANT_SDM_PAGE_70 = ANT_COMMON_PAGE_70,
    //ANT_SDM_PAGE_80 = ANT_COMMON_PAGE_80,
    //ANT_SDM_PAGE_81 = ANT_COMMON_PAGE_81,
} ant_sdm_page_t;

// calculates pace from DataPage1 and DataPage2 instants speed, i.e., speed in 1/256th meter/sec 
static void inline getPacePerKm(const uint16_t instant_speed, uint8_t* min, uint8_t* sec) { // p=(256*1000/instant_speed) sec/km
        uint32_t tmp = ANT_SDM_SPEED_UNIT_REVERSAL * 1000 / instant_speed; 
        *min = tmp / 60;
        *sec = tmp % 60;
    }; 


//ANT+ Device Profile, Stride Based Speed and Distance Monitor, rev. 1.4, sec.5.4
class SDMPage1
{
public:
    SDMPage1();

    void Decode(uint8_t const *buffer);
    void Encode(uint8_t *buffer);
 
    /* Time is in 1/200 sec units*/
    uint16_t inline getTime(void) { return time; };
    uint16_t inline getTimeInMsec(void) { return (uint16_t) ANT_SDM_TIME_RESCALE(time); };
    uint16_t inline getTimeInSec(void) { return (uint16_t) time/ANT_SDM_TIME_UNIT_REVERSAL; };
    void inline setTimeInMsec(uint16_t p_time) { time = p_time; };
 
    /* Distance is 1/16 meters*/
    uint16_t inline getDistance(void) { return distance; };
    uint16_t inline getDistanceInCm(void) { return (uint16_t) ANT_SDM_DISTANCE_RESCALE(distance); };
    float inline getDistanceInM(void) { return ((float)distance) / ANT_SDM_DISTANCE_UNIT_REVERSAL; };
    void inline setDistance(uint16_t p_distance) { distance= p_distance;};
 
    /* Speed is in 1/256 m/sec*/
    uint16_t inline getSpeed(void) { return instant_speed; };
    uint16_t inline getSpeedInCmps(void) {return  (uint16_t) ANT_SDM_SPEED_RESCALE(instant_speed);};
    void inline setSpeed(uint16_t p_speed) { instant_speed= p_speed;};
    float inline getSpeedInMinPerSec(void) { return ((float) instant_speed) / ANT_SDM_SPEED_UNIT_REVERSAL;};
    float inline getSpeedInKmph(void) { return getSpeedInMinPerSec() * 3.6 ;};
    void inline getPacePerKm(uint8_t& min, uint8_t& sec) { // p=(256*1000/instant_speed) sec/km
        uint32_t tmp = ANT_SDM_SPEED_UNIT_REVERSAL * 1000 / instant_speed; 
        min = tmp / 60;
        sec = tmp % 60;
    }; 

    /* StrideCount is in strides */
    uint16_t inline getStrideCount(void) { return strides; };
    void inline setStrideCount(uint16_t p_strides) { strides = p_strides;};

    /* Latency is in 1/32 sec */
    uint16_t inline getLatency(void) { return update_latency; };
    uint16_t inline getLatencyinMsec(void) { return (uint16_t) ANT_SDM_UPDATE_LATENCY_RESCALE(update_latency); };
    void inline setLatency(uint16_t p_latency) { update_latency = p_latency;};
     
private:
    /* message values */
    uint16_t time ;             // Stored  as 1/ANT_SDM_TIME_UNIT_REVERSAL seconds (i.e., per 200msec)
                                // SDM time of the last distance and/or speed computation. 
                                // Time starts when SDM is powered ON and continues 
                                // until it is powered OFF. Set to 0x00 when unused.
                                // Rolls over at 256sec. 
    uint16_t distance;          // Stored  as 1/ANT_SDM_DISTANCE_UNIT_REVERSAL meters (i.e., per 1/16 meters)
                                // Accumulated distance. Set to 0x00 when unused
                                // Rolls over at 256m. 
    uint16_t instant_speed;     ///Stored as 1/ANT_SDM_SPEED_UNIT_REVERSAL (i.e., per 1/256 m/s)
                                // Instantaneous speed is intended to be appropriately filtered by the SDM, 
                                // such that the receiving unit can directly display this value to the user. 
                                // Set to 0x00 when unused.
    uint8_t strides;            // Accumulated strides. This value is incremented once for every two footfalls.
                                // This is a required field. Rolls over at 256steps
    uint8_t update_latency;     // Stored as 1/ANT_SDM_UPDATE_LATENCY_UNIT_REVERSAL (i.e, 1/32 sec)
                                // The time elapsed between the last speed and distance computation 
                                // and the transmission of this message. Set to 0x00 when unused.

    /**@brief SDM page 1 data layout structure. */
    typedef struct
    {
        //uint8_t page_number;      //byte #0
        uint8_t time_fractional;         //byte #1: 1/200 sec
        uint8_t time_integer;            //byte #2: seconds
        uint8_t distance_integer;        //byte #3:
        uint8_t instant_speed_integer : 4;           //byte #4 LSN: Instantenous speed in m/s
        uint8_t distance_fractional : 4; //byte #4 MSN:  1/16 meters
        uint8_t instant_speed_fractional;               //byte #5: 1/256 m/sec
        uint8_t strides;                 //byte #6
        uint8_t update_latency;          //byte #7 1/32sec
    } ant_sdm_page1_data_layout_t;
};

/* Table 5.5. */
typedef enum {
    laces       = 0,
    midsole     = 1,
    other       = 2,
    ankle       = 3
} ant_sdm_location_t;
typedef enum {
    ok_new      = 0,
    ok_good     = 1,
    just_ok     = 2,
    low_battery = 3
} ant_sdm_batttery_status_t;
typedef enum {
    ok          = 0,
    error       = 1,
    warning     = 2,
    reserved0    = 3
} ant_sdm_health_t;
typedef enum {
    inactive    = 0,
    active      = 1,
    reserved1   = 2,
    reserved2   = 3
} ant_sdm_use_state_t;

typedef struct 
{
    ant_sdm_use_state_t         state   :2;
    ant_sdm_health_t            health  :2;
    ant_sdm_batttery_status_t   battery :2;
    ant_sdm_location_t          location:2;
 } ant_sdm_status_flags_t;

typedef union 
{
    ant_sdm_status_flags_t  flags;
    uint8_t                 byte;
} ant_sdm_status_union_t;

//ANT+ Device Profile, Stride Based Speed and Distance Monitor, rev. 1.4, sec.5.5.1
class SDMPage2
{
public:
    SDMPage2();

    void Decode(uint8_t const *buffer);
    void Encode(uint8_t *buffer);
 
    /* Speed is in 1/256 m/sec*/
    uint16_t inline getSpeed(void) { return instant_speed; };
    void inline setSpeed(uint16_t p_speed) { instant_speed= p_speed;};
    uint16_t inline getSpeedInCmps(void) {return  (uint16_t) ANT_SDM_SPEED_RESCALE(instant_speed);};
    float inline getSpeedInMinPerSec(void) { return ((float) instant_speed) / ANT_SDM_SPEED_UNIT_REVERSAL;};
    float inline getSpeedInKmph(void) { return getSpeedInMinPerSec() * 3.6 ;};

    /* Cadence is in 1/16 strides/min*/
    uint16_t inline getCadence(void) { return cadence; };
    void inline setCadence(uint16_t p_strides) { cadence = p_strides;};
    uint16_t inline getCadenceInTenthSpm(void) { return (uint16_t) ANT_SDM_CADENCE_RESCALE(cadence);};
    uint8_t inline getCadenceInSpm(void) { return (uint8_t) ROUNDED_DIV(cadence, ANT_SDM_CADENCE_UNIT_REVERSAL);};

    ant_sdm_use_state_t inline getSdmUseState(void) { return status.flags.state; };
    void inline setSdmUseState(ant_sdm_use_state_t p_state) {status.flags.state = p_state;};
 
    ant_sdm_health_t inline getSdmHealth(void) {return status.flags.health;};
    void inline setSdmHealth(ant_sdm_health_t p_health) {status.flags.health = p_health;};

    ant_sdm_batttery_status_t inline getSdmBatteryStatus(void) {return status.flags.battery; };
    void inline setSdmBatteryStatus(ant_sdm_batttery_status_t p_battery) {status.flags.battery = p_battery;};

    ant_sdm_location_t inline getSdmLocation(void) {return status.flags.location;};
    void getSdmLocation(ant_sdm_location_t p_location) { status.flags.location = p_location;};
 
     
private:
    
    uint16_t cadence;           // Stored as strides per minute
                                // Set to 0x00 when unused. Rolls over at 256spm. 
    uint16_t instant_speed;     ///Stored as 1/ANT_SDM_SPEED_UNIT_REVERSAL (i.e., per 1/256 m/s)
                                // Instantaneous speed is intended to be appropriately filtered by the SDM, 
                                // such that the receiving unit can directly display this value to the user. 
                                // Set to 0x00 when unused.
    ant_sdm_status_union_t status;

    /**@brief SDM page 1 data layout structure. */
    typedef struct
    {
        //uint8_t page_number;              //byte #0
        uint8_t reserved0[2];                //byte #1-2: Value=0xFF
        uint8_t cadence_integer;            //byte #3: Measure of strides accumulated in time.
                                            //         Set to 0x00 when unused. strides/min, rolls over 256 
        uint8_t instant_speed_integer : 4;  //byte #4 LSN: Instantenous speed in m/s
        uint8_t cadence_fractional : 4;     //byte #4 MSN:  1/16 spm
        uint8_t instant_speed_fractional;   //byte #5: 1/256 m/sec
        uint8_t reserved1;                  //byte #6
        uint8_t status_flags;             //byte #7 1/32sec
    } ant_sdm_page2_data_layout_t;
};

class SpeedDistanceMonitor : public ANTProfile
{
private:
   //uint8_t channel_number;
    static const uint8_t DeviceNumber =1; //TODO

    /* previous measurement values */
    uint16_t    prev_time; 
    uint16_t    prev_distance;
    uint16_t    prev_strides;
    
    /* cumulative values */
    uint16_t    cumulative_time =0; 
    uint16_t    cumulative_distance =0;
    uint16_t    cumulative_strides =0;

    /* message values */
    uint16_t    current_time; 
    uint16_t    current_distance;
    uint16_t    current_strides;
    uint8_t     pace_min =0;
    uint8_t     pace_sec =0;


    typedef enum {
        ANT_SDM_DATA_PAGE1 =1,
        ANT_SDM_DATA_PAGE2 =2
    } and_sdm_page_t;

    //TODO: to move ANTProfile 
    typedef struct 
    {
        uint8_t page_number; //common for all pages 
        uint8_t page_payload[7];
    } ant_sdm_message_layout_t;
    


    SDMPage1 page1;
    SDMPage2 page2;

    void AdjustCumulativeDataWith(SDMPage1* p_page);

    /* Callback for stride count*/
    void (*_OnStrideCountData_cb) (int16_t);
    /* Callback for instantenous cadence */
    void (*_OnCadenceData_cb) (uint8_t);
    /* Call back for instantenous speed in km/h */
    void (*_OnSpeedData_cb) (float);
    /* Call back for instantenous pace for 1km */
    void (*_OnPaceData_cb) (uint8_t min, uint8_t sec);
    
    ant_channel_config_t m_ant_dsp_channel_env_disp_config;
    void EncodeMessage();
    void DecodeMessage(uint8_t* p_message_payload);


public:
    SpeedDistanceMonitor (ANTTransmissionMode mode);
    //~SpeedDistanceMonitor ();
    void setOnStrideCountData(void (*fp)(int16_t)) {_OnStrideCountData_cb = fp;};
    void setOnCadenceData(void (*fp)(uint8_t)) {_OnCadenceData_cb = fp;} ;
    void setOnSpeedData(void (*fp)(float)) {_OnSpeedData_cb = fp;} ;
    void setOnPaceData(void (*fp)(uint8_t min, uint8_t sec)) {_OnPaceData_cb = fp;};
    

};


#endif //ANT_SDM_H 