/*
Connection:
1.DHT11 & Free Life v1.0 board
    S      ---   Digital Pin9
   VCC     ---   VCC
   GND     ---   GND
   
2.Moisture Sensor(Humidity) & Free Life v1.0 board 
    1S      ---   Analog Pin2
   2GND     ---   GND
   3VCC     ---   VCC
*/

#include "AutoWatering.h"
#include <DHT.h>

#define MaxTemprature 100  //The Maximum Value of the Temperature
#define SensorTest 1
#define CarbonTest 0 // If you have the carbon sensor only

AutoWatering flower;

void setup()
{
  flower.Initialization();//Initialization for the watering kit
  Serial.begin(115200);//Buad Rate is set as 115200bps
}
void loop()
{
  //Power on the pump according to the ambient temperature and humidity
  pumpOn(SensorTest,CarbonTest,MaxTemprature);
  //Power on the pump for testing whether the pump can work properly
  //flower.pumpTestOn();  
  delay(2000);
  //flower.pumpOff();
  delay(2000);
  Serial.println();
  humidity = flower.getHumidity();
  Serial.print("Humidity is     :" );
  Serial.print(humidity);Serial.print("% ");
  Serial.println();
}

//Power on the pump according to the ambient temperature and humidity
void pumpOn(int MoiSensor, int Carbon, int Temperature_max)  
{
   int humidity;
   int humidity_max;
   float dht_t;
   
   //To choose use the Moisture Sensor or the Carbon 
   //to test the soil moisture
   if(MoiSensor==1&&Carbon==0) 
   {
    humidity = flower.MoistureSensor();
   }else if(MoiSensor==0&&Carbon==1)
   {
    humidity = flower.CarbonRod();
   }else{
    humidity = 0;
   }
   
   Serial.print("Soil Moisture is:" );
   Serial.print(humidity);Serial.print("% ");
   Serial.println();
   humidity_max = flower.ADJ_humMax(); 
   Serial.println();
   dht_t = flower.getTemperature();
   Serial.print("Temperature is :");
   //Serial.print(dht_t);Serial.print("°C"); //**For use with Arduino Serial Monitor
   //Serial.print(dht_t);Serial.print(char(176));Serial.print("C");  //Another way to display Temperature symbol
   Serial.print(dht_t);Serial.print("\xB0""C");  //**For use with 3rd party serial monitor program
   Serial.println();
   
   digitalWrite(6,LOW);
   digitalWrite(5,LOW);
   digitalWrite(7,LOW);
   digitalWrite(4,LOW);
   
   if(humidity<=humidity_max&&dht_t<=Temperature_max)
    {
      digitalWrite(6,HIGH);
      digitalWrite(5,HIGH);
      Serial.println();
      Serial.println("Soil moisture critical!!!...");
      Serial.println("Applying water..");
      Serial.println("Pump is switched on!");
     }
   else
    {
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);
      Serial.println();
      Serial.print("Soil moisture levels normal");
      flower.pumpOff();
      Serial.println("Pump is Ready");
   }
}
