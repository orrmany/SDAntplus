#ifndef PowerMeter_h
#define PowerMeter_h

#define USE_TINYUSB


#include "sdant.h"
#include "profiles/BicyclePower.h"
#include <bluefruit.h>
#include "stdint-gcc.h"
#include "HX711_ADC.h"

//#include "list"

typedef struct powermeter_config
{
    uint8_t dms_dout;
    uint8_t dms_sck;
    uint8_t hall_out;
    float calibration;
    uint16_t profileUpdateCycle;
    BicyclePower* p_power_profile;
    HX711_ADC* p_loadcell;
} powermeter_config;

class PowerMeter
{
public:

    

    PowerMeter(powermeter_config*);
    void begin();
    void update();
    void checkLoadCell();
    void hallInterrupt();

    uint8_t Get_dms_dout_Pin()          { return config.dms_dout; }
    uint8_t Get_dms_sck_Pin()           { return config.dms_sck; }
    uint8_t Get_hall_out_Pin()           { return config.hall_out; }

    void SetAccPWR(uint16_t val)        { accPWR = val; }
    void SetInstPWR(uint16_t val)       { instPWR = val; }
    void SetInstCAD(uint8_t val)        { instCAD = val; }
    void SetPWREventCount(uint8_t val)  { PWREventCount = val; }

private:
    BicyclePower* pwr;
    HX711_ADC* LoadCell;
    powermeter_config config;
    uint16_t accPWR, instPWR;
    uint8_t instCAD, PWREventCount;

    uint32_t currentHallInterrupt, lastHallInterrupt, lastrotationDuration;
    uint32_t nextProfileUpdate;
    //std::list<uint16_t> weightValues;
    float weightSum; //all measurements together in units of 100grams
    uint16_t numberOfMeasurements;
    bool loadcelldataReady;

};


#endif