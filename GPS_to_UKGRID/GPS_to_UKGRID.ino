#define programname "GPS_to_UKGRID.h"
#define programversion "V1.0"
#define dateproduced "28/1/2019"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>

//Code originally posted on Arduino forums https://forum.arduino.cc/index.php?topic=592429.0 
//The maths comes from http://www.haroldstreet.org.uk/other/convertosgblatlong/

//See the comments at the top of the GPStoUKGRID.h file for details of program operation

/*
*****************************************************************************************************************************
Tracker Test Programs.

Copyright of the author Stuart Robinson - 28/1/2019

These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
free from errors.
*****************************************************************************************************************************
*/

#define Serial_Monitor_Baud 115200          //this is baud rate used for the Arduino IDE Serial Monitor

//Reference location; spot height at Pen Y Fan summit, Brecon Beacons, UK.
const float TestLatitude = 51.88382;        
const float TestLongitude = -3.43712;
const float TestAltitude = 886;

#include "GPStoUKGRID.h"

void loop() 
{

LLtoNE(TestLatitude, TestLongitude, TestAltitude);

Serial.print("Eastings ");
Serial.println(p_os_eastings); 
Serial.print("Northing ");
Serial.println(p_os_northings);
Serial.print("Elevation ");
Serial.println(p_airy_elevation);
Serial.println();
delay(2000);
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);          //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();
}
