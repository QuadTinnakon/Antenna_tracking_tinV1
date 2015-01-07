/*
project_antenna_tracking 
by: tinnakon kheowree  
tinnakon_za@hotmail.com
tinnakonza@gmail.com
http://quad3d-tin.lnwshop.com/
https://www.facebook.com/tinnakonza
*/
#include <Arduino.h>
#include "config.h"
#include "Servo_tin.h"
#include "GPS_multi.h"

void setup() 
{ 
   // Init serial communication; and  GPS port 38400
  Serial.begin(38400);
  Serial1.begin(38400);
  pinMode(13, OUTPUT);
  pinMode(11,OUTPUT);// (328 = pin 9) PB1 - OC1A (2560 = pin 11)
  pinMode(12,OUTPUT);// (328 = pin 10) PB2 - OC1B (2560 = pin 12)
  init_PWM();
  
  previousTime = micros();
} 
 
void loop() 
{ 
     while(Serial1.available())
   {
     char byteGPS=Serial1.read(); 
     GPS_newFrame(byteGPS);
   }
  deltaTime = micros() - previousTime;
    if(deltaTime <= 0)
  {
    deltaTime = 20001;//50 Hz
  }
  // Run task loop 20 ms  = 50 Hz
  if(deltaTime >= 20000){
  previousTime = micros();
  frameCounter++;
  
  GPS_LAT = GPS_coord[LAT]/10000000.0;
  GPS_LON = GPS_coord[LON]/10000000.0;
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  //Set HOME
  if(val < 10)
  {
    GPS_LAT_Home = 13.8654345;//
    GPS_LON_Home = 100.4818690;// 
  }
    if(val > 1000)
  {
    GPS_LAT_Home = GPS_LAT;//
    GPS_LON_Home = GPS_LON;// 
  }
  Alt = GPS_altitude/10.0;
  antenna_tracking(GPS_LON_Home, GPS_LAT_Home, GPS_LON, GPS_LAT, Alt);
  
  int val1 = map(pitch, -10, 120, 810, 2200);// scale it to use it with the servo (value between 810 and 1200) 
  int val2 = map(yaw, 0, 360, 2000, 1000);
  writeServo(val1 ,val2);

    // 10Hz task loop  0.1 s = 0.1/dt
    int Hz10 = 10;//5
    // Read compass
        if (frameCounter % Hz10 == 0) 
        {
          Serial.print(GPS_coord[LAT]);Serial.print("\t");
          Serial.print(GPS_coord[LON]);Serial.print("\t");
          Serial.print(GPS_altitude/10.0);Serial.print("\t");
          //Serial.print(GPS_altitude2/10.0);Serial.print("\t");
          //Serial.print(GPS_numSat);Serial.print("\t");
          //Serial.print(GPS_FIX);Serial.print("\t");
          
          Serial.print(Distance);Serial.print("\t");
          Serial.print(yaw);Serial.print("\t");
          Serial.println(pitch);
        }//end roop 10 Hz
     // 1Hz task loop  1 s = 1/dt
   int Hz1 = 50;   
 // Reset frameCounter after 1s
 if (frameCounter >= Hz1)
 {
     frameCounter = 0;
     if(Status_LED == LOW && GPS_FIX > 0)
     {
     Status_LED = HIGH;
     }
     else
     {
     Status_LED = LOW;
     }
     digitalWrite(13, Status_LED);
 }   
}//end loop 20 ms 
}
