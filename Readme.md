## GPS Tutorial 

On public forums, such as Arduino, I see great many posts along the lines of 'My GPS does not work'. Most often constructors connect a GPS to an Arduino,  load up a complete application and find it does not work, there is no GPS location reported. Sometimes they do the sensible thing and load one of the simple examples that comes with an Arduino library, but still they find their GPS 'does not work'

The first troubleshooting step is to realise that the GPS will probably only work when it's outside with a good view of the sky. It matters little how much you think you want or need the GPS to work indoors, take it outside.  

If that does not solve your 'not working GPS' try a simple GPS echo program such as listed below. This reads character from the GPS and sends them to the Arduino IDE serial monitor so you can see what the GPS is up to. Remember to take your GPS outdoors. 
#####Note that you will need to tell the program what pin numbers you have the GPS TX and GPS RX pins connected to on the Arduino. 


    //Simple_SoftwareSerial_GPS_Echo
    
    //Reads characters from a GPS using Software Serial and echoes them to the Arduino Serial Monitor at 115200 baud.   
    
    //Important Note
    //--------------
    //Make sure the #defines below for the GPS TX and GPS RX pin numbers below match what you have connected and that the GPS
    //baud rate used by the GPS is correct, its often 9600 baud but not always. 
        
    #define GPSTX A2 //pin number for GPS TX output - data from Arduino into GPS
    #define GPSRX A3 //pin number for GPS RX input - to Arduino from GPS
    #define GPSBaud 9600 //GPS Baud rate
    #define Serial_Monitor_Baud 115200   //this is baud rate used for the Arduino IDE Serial Monitor
    
    #include <Arduino.h>
    #include <SoftwareSerial.h> 
    SoftwareSerial GPSserial(GPSRX, GPSTX);  
        
    void loop()
    {
      while (GPSserial.available() > 0)
      Serial.write(GPSserial.read());
    }
        
    void setup()
    {
     Serial.begin(Serial_Monitor_Baud);   //start Serial console ouput
     GPSserial.begin(GPSBaud);//start softserial for GPS at defined baud rate
    }
    
<br><br>
This first printout below is what the GPS output will look like for a GPS that is typical when the has just been turned on;

    
    $GPTXT,01,01,02,u-blox ag - www.u-blox.com*50
    $GPTXT,01,01,02,HW  UBX-G60xx  00040007 FF7FFFFFp*53
    $GPTXT,01,01,02,ROM CORE 7.03 (45969) Mar 17 2011 16:18:34*59
    $GPTXT,01,01,02,ANTSUPERV=AC SD PDoS SR*20
    $GPTXT,01,01,02,ANTSTATUS=DONTKNOW*33
    $GPRMC,,V,,,,,,,,,,N*53
    $GPVTG,,,,,,,,,N*30
    $GPGGA,,,,,,0,00,99.99,,,,,,*48
    $GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30
    $GPGSV,1,1,00*79
    $GPGLL,,,,,,V,N*64
    $GPRMC,,V,,,,,,,,,,N*53


A few seconds later you should see something like this;

    $GPGSV,1,1,01,03,,,19*73
    $GPGLL,,,,,,V,N*64
    $GPRMC,,V,,,,,,,,,,N*53
    $GPVTG,,,,,,,,,N*30
    $GPGGA,,,,,,0,00,99.99,,,,,,*48
    $GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30


Note that the line reporting the satellites in view, the GPS sentence $GPGSV, has changed from $GPGSV,1,1,00\*79 (meaning no satellites in view)\ to  $GPGSV,1,1,01,03,,,19*73 which shows there is one satellite in view. The satellite is number 03 and the signal strength is 19, which is very weak. To eventually get a good GPS fix you will need several satellites in view with a signal strength of 25+.

The next thing that should happen is that the GPS starts reporting the time, 11:39:41 in this case;

    $GPGSV,1,1,01,03,,,25*7C
    $GPGLL,,,,,113940.00,V,N*44
    $GPRMC,113941.00,V,,,,,,,,,,N*72
    $GPVTG,,,,,,,,,N*30
    $GPGGA,113941.00,,,,,0,00,99.99,,,,,,*69

Within a minute or so most GPS with decent antennas and a good view of the sky should then start reporting the position, the content of the position sentences $GPRMC and $GPGGA should look something like this;
    
    $GPRMC,111218.00,A,5133.58788,N,00313.12853,W,0.205,,261018,,,A*61
    $GNGGA,111218.00,5133.58788,N,00313.12853,W,1,06,1.36,241.1,M,49.4,M,,*55


Some GPS, particularly the types used in high altitude ballooning that have small ceramic stick antennas or a simple wire, can take 5 minutes or more getting a fix. 

If a GPS is reporting a number of satellites in view such as a $GPGSV  sentence that looks like this (06 satellites in view) but does not get a fix;

    $GPGSV,2,1,06,03,79,240,27,11,44,145,30,17,49,291,31,18,34,116,20*78
    $GPGSV,2,2,06,23,26,180,30,32,,,29*4A

Then its either got a poor view of the sky, has a poor antenna or is faulty. There is no magic bit of code that will fix this problem. 
<br><br>
##Note on Ublox GPSs

Some Ublox GPS will startup with the position sentences $GPRMC and $GPGGA in $GNRMC and $GNGGA format like this;

    $GNRMC,111218.00,A,5133.58788,N,00313.12853,W,0.205,,261018,,,A*7F
    $GNGGA,111218.00,5133.58788,N,00313.12853,W,1,06,1.36,241.1,M,49.4,M,,*55 

Check that the Arduino GPS library you are using supports the $GPGN format, the latest copy of TinyGPSPlus on github does;


[https://github.com/mikalhart/TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
<br><br>
 

##Stuart Robinson
##www.LoRaTracker.uk
##October 2018
 