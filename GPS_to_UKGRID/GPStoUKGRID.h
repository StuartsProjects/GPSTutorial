//posted on Arduino forums https://forum.arduino.cc/index.php?topic=592429.0 
//the maths comes from http://www.haroldstreet.org.uk/other/convertosgblatlong/

/*
****************************************************************************
Code originally posted on Arduino forums https://forum.arduino.cc/index.php?topic=592429.0 
The maths comes from http://www.haroldstreet.org.uk/other/convertosgblatlong/

This code takes the Latitude and Longitude, as in a position from a GPS and when converted from 
GPS format into in decimal degrees converts it into British Ordnance Survey grid co-ordinates.

As a test location I chose the spot height on the top of Pen Y Fan in the Brecon Beacons, there
used to be a triangulation pillar there but it was removed in the 1980s. The locatation according 
to Ordnance Survey's own digital map, used with the Memory Map application was;

Latitude 51.88382(N), Longitude -3.43712(W)
Grid reference SO 01182 21562

Enterinng the Latitude and Longitude into the code here produces a converted grid reference of;

(3)01182 (2)21562   -  numbers in brackets are the 100km grid square numbers

So for the particular 100km square the calculated grid reference is

01182 21562 

Which is exact, or more likley within 1M or so. 

100km Grid Square
-----------------

The program reports the grid co-ordiantes as two decimal numbers, the first digit of 
each number is the 100km square that the grid referance is in, the squares have two letter 
identifiers, see here for the map;

https://www.movable-type.co.uk/scripts/latlong-os-gridref.html

This conversion code numbers the 100km grid squares from 0, with 0,0 being the bottom left 
in the picture linked to above.  

So for our test location the 100km square is 4 to the left (0,1,2,3) and 3 up (0,1,2)
which is the SO square. Thus the calculated grid reference is correct; SO 01182 21562

****************************************************************************
*/


//definitions for geometric conversions
#define deg2rad 0.017453292519943295 //(PI / 180)
#define rad2deg 57.29577951308232087 //(180/ PI)

#define a 6377563.396       // OSGB semi-major axis
#define b 6356256.91        // OSGB semi-minor axis
#define e0 400000           // OSGB easting of false origin
#define n0 -100000          // OSGB northing of false origin
#define f0 0.9996012717     // OSGB scale factor on central meridian
#define e2 0.0066705397616  // OSGB eccentricity squared
#define lam0 -0.034906585039886591  // OSGB false east
#define phi0 0.85521133347722145    // OSGB false north
#define af0   6375020.48098897069   //(a * f0)
#define bf0   6353722.49048791244   //(b * f0)
#define n 0.0016732202503250876 //(af0 - bf0) / (af0 + bf0)

#define WGS84_AXIS 6378137 // a
#define WGS84_ECCENTRIC 0.00669438037928458 //e
#define OSGB_AXIS 6377563.396 //a2
#define OSGB_ECCENTRIC 0.0066705397616   //e2
#define _xp -446.448  //OSGB/Airy datums/parameters
#define _yp 125.157
#define _zp -542.06
#define xrot -0.000000728190149026 //_xr -0.1502; (_xr / 3600) * deg2rad;
#define yrot -0.000001197489792340 //_yr -0.247; (_yr / 3600) * deg2rad;
#define zrot -0.000004082616008623 //_zr -0.8421; (_zr / 3600) * deg2rad;
#define _sf 0.0000204894  // s=20.4894 ppm


uint32_t p_os_northings;
uint32_t p_os_eastings;
signed int p_airy_elevation;



float Marc(float phi)   // used in LLtoNE function below
{
   float Marc = bf0 * (((1 + n + ((5 / 4) * (n * n)) + ((5 / 4) * (n * n * n))) * (phi - phi0))
      - (((3 * n) + (3 * (n * n)) + ((21 / 8) * (n * n * n))) * (sin(phi - phi0)) * (cos(phi + phi0)))
      + ((((15 / 8) * (n * n)) + ((15 / 8) * (n * n * n))) * (sin(2 * (phi - phi0))) * (cos(2 * (phi + phi0))))
      - (((35 / 24) * (n * n * n)) * (sin(3 * (phi - phi0))) * (cos(3 * (phi + phi0)))));
   return (Marc);
}


void LLtoNE(float latConv, float lonConv, float heightConv)
{
   latConv*= deg2rad;      // convert latitude to radians
   lonConv*= deg2rad;      // convert longitude to radians

   // Convert WGS84/GRS80 into OSGB36/Airy
   // convert to cartesian
   float v = WGS84_AXIS / (sqrt(1 - (WGS84_ECCENTRIC *(sin(latConv) * sin(latConv)))));
   float x = (v + heightConv) * cos(latConv) * cos(lonConv);
   float y = (v + heightConv) * cos(latConv) * sin(lonConv);
   float z = ((1 - WGS84_ECCENTRIC) * v + heightConv) * sin(latConv);
   // transform cartesian
   float hx = x + (x * _sf) - (y * zrot) + (z * yrot) + _xp;
   float hy = (x * zrot) + y + (y * _sf) - (z * xrot) + _yp;
   float hz = (-1 * x * yrot) + (y * xrot) + z + (z * _sf) + _zp;
   // Convert back to lat, lon
   lonConv = atan(hy / hx);
   float p = sqrt((hx * hx) + (hy * hy));
   latConv = atan(hz / (p * (1 - OSGB_ECCENTRIC)));
   v = OSGB_AXIS / (sqrt(1 - OSGB_ECCENTRIC * (sin(latConv) * sin(latConv))));
   float errvalue = 1.0;
   float lat1 = 0;
   while (errvalue > (1/1024))
   {
      lat1 = atan((hz + OSGB_ECCENTRIC * v * sin(latConv)) / p);
      errvalue = abs(lat1 - latConv);
      latConv = lat1;
   }
   p_airy_elevation = p / cos(latConv) - v;



   // Convert OSGB36/Airy into OS grid eastings and northings
   // easting
   float slat2 = sin(latConv) * sin(latConv);
   float nu = af0 / (sqrt(1 - (e2 * (slat2))));
   float rho = (nu * (1 - e2)) / (1 - (e2 * slat2));
   float eta2 = (nu / rho) - 1;
   float pp = lonConv - lam0;
   float IV = nu * cos(latConv);
   float clat3 = pow(cos(latConv), 3);
   float tlat2 = tan(latConv) * tan(latConv);
   float V = (nu / 6) * clat3 * ((nu / rho) - tlat2);
   float clat5 = pow(cos(latConv), 5);
   float tlat4 = pow(tan(latConv), 4);
   float VI = (nu / 120) * clat5 * ((5 - (18 * tlat2)) + tlat4 + (14 * eta2) - (58 * tlat2 * eta2));
   p_os_eastings = e0 + (pp * IV) + (pow(pp, 3) * V) + (pow(pp, 5) * VI);
   // northing
   float M = Marc(latConv);
   float I = M + (n0);
   float II = (nu / 2) * sin(latConv) * cos(latConv);
   float III = ((nu / 24) * sin(latConv) * pow(cos(latConv), 3)) * (5 - pow(tan(latConv), 2) + (9 * eta2));
   float IIIA = ((nu / 720) * sin(latConv) * clat5) * (61 - (58 * tlat2) + tlat4);
   p_os_northings = I + ((pp * pp) * II) + (pow(pp, 4) * III) + (pow(pp, 6) * IIIA);
}
