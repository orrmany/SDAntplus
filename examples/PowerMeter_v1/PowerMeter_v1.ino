
//#include <bluefruit52.h>
//#include <Adafruit_LittleFS.h>
//#include <InternalFileSystem.h>

#include "src/PowerMeter/PowerMeter.h"

//BicyclePower pwr(Both);
//HX711_ADC dms(A0, A1);

powermeter_config PWRconfig = 
{
  A0,
  A1,
  A7,
  10775.02,
  250/*,
  &pwr,
  &dms*/
};

PowerMeter power(&PWRconfig);

//const int HX711_dout = A0; //mcu > HX711 dout pin, must be external interrupt capable!
//const int HX711_sck = A1; //mcu > HX711 sck pin

uint16_t SetRPM = 75;
unsigned long t = 0, newmessage = 0;
//volatile boolean newDataReady;


//HeartRateMonitor hrm(TX);
//EnvironmentSensor env(RX);
//SpeedDistanceMonitor sdm(RX);

//uint16_t currPWR = 100, goalPWR = 100; 
//uint16_t accumulatedPWR = 0;
//uint8_t currCAD = 0;
//uint8_t PWR_EventCounter = 0;


void setup(void)
{
//  bool ret = false;
  Serial.begin(115200);
  //delay(5000);
  while (!Serial);

  power.begin();
  attachInterrupt(digitalPinToInterrupt(power.Get_dms_dout_Pin()), dataReadyISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(power.Get_hall_out_Pin()), hallISR, RISING);
  
  newmessage = millis() + 1000;
  Serial.println("Setup is finished!");
}

void dataReadyISR() {
  power.checkLoadCell();
}

void hallISR() {
  power.hallInterrupt();
}

void loop(void)
{
  //const int serialPrintInterval = 0;
  power.update();
  if (millis() > newmessage) {
    
    power.hallInterrupt();
    newmessage += round((float)60000 / (float)SetRPM);
  }
  if (Serial.available()) {
    uint8_t temp = Serial.parseInt();
    SetRPM = temp;
    Serial.printf("New RPM: %d\n", SetRPM);
    switch(temp) {
      case 0:
        //SetRPM--;
        //Serial.printf("New RPM: %d\n", SetRPM);
//        Serial.printf("Samples in Use: %d\n", LoadCell.getSamplesInUse());
//        Serial.printf("SPS: %.2f\n", LoadCell.getSPS());
        break;
      case 1:
        //SetRPM++;
        //Serial.printf("New RPM: %d\n", SetRPM);
        break;
      case 2:
        
        break;
      case 3:
        break;
      default:
        break;
    }
  }
  
} //loop
