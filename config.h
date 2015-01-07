/*
project_antenna_tracking 
by: tinnakon kheowree  
tinnakon_za@hotmail.com
tinnakonza@gmail.com
http://quad3d-tin.lnwshop.com/
https://www.facebook.com/tinnakonza
*/
#define PI 3.14159265
//GPS CN-06 GPS Receiver  NMEA frames
//GPS Distance error +- 11 m
//home non           270       90             180          0            45            315
//138669483	13.8670250  13.8670250     13.8696637   13.8646637   13.8654345   13.8656637
//1004832133    100.4852133 100.4812133    100.4832133  100.4832133  100.4818690  100.4845941
float GPS_LAT_Home = 13.8670733;//
float GPS_LON_Home = 100.4832416;//
float Alt_Home = 2;//m

float GPS_LAT = 0;
float GPS_LON = 0;
float Alt = 0; 

// ******************
// Servo functions
// ******************
#define MINCOMMAND 1000
#define MIDCOMMAND 1500
#define MAXCOMMAND 1950

float yaw = 0.0;
float pitch = 0.0;
float Distance = 0.0;
///////////////////////////////////////////////////
#define ToRad 0.01745329252  // *pi/180
#define ToDeg 57.2957795131  // *180/pi
////////time///////////////////////////////////////
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 
//time
long deltaTime = 0;
unsigned long previousTime = 0;
int frameCounter=0;
int Status_LED = LOW;
///////////////////////////////////////////////////
