#define programname "GPS_Sleep_Tester_UBLOX"
#define programversion "V1.0"
#define authorname "Stuart Robinson"
#include <Arduino.h>

/*
********************************************************************************************************************************

  Copyright of the author Stuart Robinson - 26/11/2018

  http://www.LoRatracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit
  permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended
  purpose and free from errors.

  This program uses software methods to put a UBLOX GPS to sleep\standby. 
  
  The purpose of the test is to be able to measure the current the GPS consumes in sleep\standby mode. The program 
  also prints the output of the GPS during the period it is on, so can be used to check a GPS is working.
  
  If the GPS can be put to sleep it you can then wake it up and it will can often get a new fix within a few seconds,
  this is called 'Hot Fix' mode and can save considerable amounts of power. However since the GPS receiver is off in 
  this mode it cannot keep the almanac data on satellites updated and ocaisionaly the hot fix can take 20,30 or more
  seconds. 

  The time in mS the GPS is on and then off is defined by the const definitions for 'GPSDebugTimeSeconds' and 
  'GPSSleepTimeSeconds' at the top of the program.
   

  At the top of the program you will also need to define the pins for the GPS, LED and GPS power control. The power control
  normally needs additional components such a MOSFET controlling the VCC supply to the GPS.  

  ********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  To Do:

********************************************************************************************************************************
*/

#define GPSTX A2                                    //this is the pin that the Arduino uses to send data to the GPS  
#define GPSRX A3                                    //this is the pin that the Arduino receives data from the GPS                              
#define LED1 8                                      //LED is on the pin, when driven high LED is on 
#define GPSPOWER 7                                  //When high this pin turns GPS off. Needs aditional hardware.
                                                    //Set to -1 if you dont have a method of turniong off GPS power

#define Serial_Monitor_Baud 115200                  //this is baud rate used for the Arduino IDE Serial Monitor
#define GPSBaud 9600                                //GPS baud rate

const uint32_t GPSDebugTimeSeconds = 10;            //time in seconds that the GPS output is printed to screen  
const uint32_t GPSSleepTimeSeconds = 10;            //time in seconds that the GPS is off                                                    

#include <SoftwareSerial.h>
SoftwareSerial GPSserial(GPSRX, GPSTX);

uint32_t GPSDebugOutputEndmS;


void loop()
{
  Serial.println(F("Wakeup GPS"));
  Serial.println();
 
  UBLOX_GPS_Wakeup();                               //wakeup GPS   
  digitalWrite(LED1, HIGH);                         //LED on indicates GPS on
  
  Serial.println(F("GPS output"));                  //print some output from the GPS so we know its working and can measure the current consumption 
  
  GPSDebugOutputEndmS = millis() + (GPSDebugTimeSeconds * 1000);
  while (millis() <= GPSDebugOutputEndmS)
  {
    while (GPSserial.available() > 0)
      Serial.write(GPSserial.read());
  }
  
  Serial.println();
  Serial.println();
  Serial.println(F("Putting GPS in software shutdown"));

  UBLOX_GPS_Shutdown();
        
  Serial.println();
  Serial.println(F("GPS Sleeping .... zzzzz"));
  Serial.println();
  digitalWrite(LED1, LOW);
  delay(GPSSleepTimeSeconds * 1000);
}


void UBLOX_GPS_Shutdown()
{
  //sends command over serial interface to GPS to put it in PMREQ backup mode
  uint8_t index;
  uint8_t UBLOX_GPSStandby[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B}; 

  for (index = 0; index < sizeof(UBLOX_GPSStandby); index++)
  {
    GPSserial.write(UBLOX_GPSStandby[index]);
    Serial.print(UBLOX_GPSStandby[index], HEX);
    Serial.print(" ");
  }
}


void UBLOX_GPS_Wakeup()
{
  GPSserial.println();                                   //send some characters to GPS to wake it up
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);                    //setup Serial console ouput

  pinMode(LED1, OUTPUT);                                //pin for LED to output
  digitalWrite(LED1, LOW);                              //make sure LED is off
    
  pinMode(GPSPOWER, OUTPUT);                            //pin for GPS power control if fitted
  digitalWrite(GPSPOWER, LOW);                          //turns on GPS power 
  
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(authorname));
  Serial.println();
  Serial.println();
  GPSserial.begin(GPSBaud);                             //start softserial for GPS at defined baud rate
}
