//Simple_HardwareSerial_GPS_Echo

//Reads characters from a GPS using one of the Arduinos Hardware Serial
//ports and echos them to the Arduino Serial Monitor at 115200 baud.

//The program defaults to reading the GPS on Serial1 at 9600 baud

#include <Arduino.h>


void loop()
{
  while (Serial1.available() > 0)
  {
    Serial.write(Serial1.read());
  };
}


void setup()
{
  Serial.begin(115200);                //setup Serial monitor ouput
  Serial1.begin(9600);                 //start Serial for GPS at defined baud rate
}
