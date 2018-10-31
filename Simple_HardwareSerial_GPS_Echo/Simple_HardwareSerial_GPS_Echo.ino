//Simple_HardwareSerial_GPS_Echo

//Reads characters from a GPS using one of the Arduinos Hardware Serial
//ports and echos them to the Arduino Serial Monitor at 115200 baud.

//Define the hardware serial port and the baud rate the GPS is using below, 9600 baud is common.

#include <Arduino.h>

#define GPSserial Serial1           //define hardware serial port the GPS is connected to, can be Serial1, Serial2, Serial3 or Serial4
#define GPSBaud 9600                //GPS Baud rate
#define Serial_Monitor_Baud 115200  //this is baud rate used for the Arduino IDE Serial Monitor


void loop()
{
  while (GPSserial.available() > 0)
    Serial.write(GPSserial.read());
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);   //setup Serial monitor ouput
  GPSserial.begin(GPSBaud);            //start Hardware for GPS at defined baud rate
