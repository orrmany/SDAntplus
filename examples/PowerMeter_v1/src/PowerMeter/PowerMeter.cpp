#include "./PowerMeter.h"

void PrintUnhandledANTEvent(ant_evt_t *evt)
{
  Serial.printf("Channel #%d for %s: event %s\n", evt->channel, ANTplus.getAntProfileByChNum(evt->channel)->getName(), AntEventTypeDecode(evt)); 
  if (evt->event != EVENT_CHANNEL_COLLISION 
    && evt->event != EVENT_RX_FAIL
    && evt->event != EVENT_CHANNEL_CLOSED
    )
    Serial.printf("  (%s)\n", AntEventType2LongDescription(evt));
}
void ReopenANTChannel(ant_evt_t *evt)
{
  if (evt->event == EVENT_CHANNEL_CLOSED ) {
    Serial.printf("Channel #%d closed for %s\n", evt->channel,ANTplus.getAntProfileByChNum(evt->channel)->getName()); 
    Serial.printf("Reopening...");
    uint32_t ret;
    ret = sd_ant_channel_open(evt->channel);
    if (ret == NRF_SUCCESS) Serial.println("success!");
    else Serial.printf("failed with code:%#x\n", ret);
  }
}

PowerMeter::PowerMeter(powermeter_config * cfg) {
    config.dms_dout = cfg->dms_dout;
    config.dms_sck = cfg->dms_sck;
    config.hall_out = cfg->hall_out;
    config.calibration = cfg->calibration;
    config.profileUpdateCycle = cfg->profileUpdateCycle;
    //config.p_power_profile = cfg->p_power_profile;
    //config.p_loadcell = cfg->p_loadcell;
    pwr = new BicyclePower(Both); //config.p_power_profile;
    LoadCell = new HX711_ADC(config.dms_dout, config.dms_sck); //LoadCell = config.p_loadcell;
}

void PowerMeter::begin() {
    Serial.println("Starting PowerMeter Setup...");
    Serial.printf("Config: %d\t%d\t%d\t%f\n", config.dms_dout, config.dms_sck, config.hall_out, config.calibration);
    Serial.println("Adding PWR profile");
    pwr->setUnhandledEventListener(PrintUnhandledANTEvent);
    pwr->setAllEventListener(ReopenANTChannel);
    pwr->setName("PWR");
    ANTplus.AddProfile(pwr);

    int pin = PIN_A7;
    Serial.println("Bluefruit52 BLEUART Startup");
    Serial.println("---------------------------\n");
    Bluefruit.autoConnLed(false);
    Bluefruit.configPrphBandwidth(BANDWIDTH_LOW);

    Serial.print("Starting BLE stack. Expecting 'true':");
    bool ret = Bluefruit.begin(0, 0);
    Serial.println(ret);
    Serial.print("Starting ANT stack. Expecting 'true':");
    ret = ANTplus.begin(1);
    Serial.println(ret);
    ANTProfile* profiles[] = {pwr}; //{&hrm, &env, &sdm};
    for (auto i: profiles)
    {
        Serial.printf("Channel number for %s became %d\n", i->getName(), i->getChannelNumber());
    }
    
    LoadCell->begin(); //begin with specified data and sck pins
    Serial.printf("Loadcell begin done.\n");
    unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
    bool _tare = true; //set this to false if you don't want tare to be performed in the next step
    LoadCell->start(stabilizingtime, _tare);
    Serial.printf("Loadcell start done.\n");
    if (LoadCell->getTareTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        //while (1);
    }
    else {
        LoadCell->setCalFactor(config.calibration); // set calibration value (float)
        Serial.printf("Calibration factor %f set!\n", config.calibration);
        loadcelldataReady = false;
    }
    nextProfileUpdate = millis(); //to start with the right interval already in first update() cycle;
    Serial.printf("Startup is complete.\n");
}

void PowerMeter::checkLoadCell() //Now just set ready, later add value to weightSum and increment numberofmeasurements
{
    if(LoadCell->update()) 
    {
        loadcelldataReady = true;
    }
}

void PowerMeter::hallInterrupt() //Calc new cadence, new power and set other values to 0;
{
    currentHallInterrupt = millis();//Update Current Hall time
    lastrotationDuration = currentHallInterrupt - lastHallInterrupt;    //Calculate time of last rotation
    float rps = 1000.0/(float)lastrotationDuration; //Last Rotation cadence in U/s
    instCAD = (uint8_t)round(60 * rps);      //Calc cadence of last rotation in U/min
    lastHallInterrupt = currentHallInterrupt;//Now set old Hall time to now and wait for next one
    float avgTorque = (weightSum/(float)numberOfMeasurements)*1.71675;  //9.81 * 0.175 which is to get N instead of kg and then multiplying it with the Crank Length
    weightSum = numberOfMeasurements = 0; //Set back to 0 for next rotation
    instPWR = (uint16_t)round(avgTorque * TWO_PI * rps);    //calc instant pwr
    accPWR += instPWR;
    PWREventCount++;
}

void PowerMeter::update()
{   
    //TODO -> Move this to checkLoadCell...
    if (loadcelldataReady) //First check if a New measurement from hx711 is available
    {   
        float currentWeight = LoadCell->getData();
        weightSum += currentWeight > 0.0f ? currentWeight : 0.0f;
        numberOfMeasurements++;
        loadcelldataReady = false;
        //Serial.printf("LoadCell Data: %f\n", weightSum);
        //weightValues.push_back((uint16_t)round(LoadCell->getData()*10.0)); //add Current weight to the end of the list
    }
    if (millis() > nextProfileUpdate)
    {
        nextProfileUpdate += config.profileUpdateCycle;
        pwr->SetInstantPWR(instPWR);
        pwr->SetAccumulatedPWR(accPWR);
        pwr->SetPWREventCount(PWREventCount);
        pwr->SetInstantCadence(instCAD);
        //Serial.printf("Values for Profile:\t%d\t%d\t%d\t%d\n", instPWR, accPWR, PWREventCount, instCAD);
    }
}