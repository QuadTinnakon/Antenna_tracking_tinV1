/*
project_antenna_tracking 
by: tinnakon kheowree  
tinnakon_za@hotmail.com
tinnakonza@gmail.com
http://quad3d-tin.lnwshop.com/
https://www.facebook.com/tinnakonza
*/
//GPS CN-06 GPS Receiver  NMEA frames

int32_t GPS_coord[2];
uint8_t GPS_Present;// Checksum from Gps serial
uint8_t GPS_FIX;
uint8_t GPS_numSat;
int GPS_altitude;//uint16_t GPS altitude      - unit: meter
int GPS_altitude2;
#define LAT  0
#define LON  1
 /* Alex optimization 
  The latitude or longitude is coded this way in NMEA frames
  dm.f   coded as degrees + minutes + minute decimal
  Where:
    - d can be 1 or more char long. generally: 2 char long for latitude, 3 char long for longitude
    - m is always 2 char long
    - f can be 1 or more char long
  This function converts this format in a unique unsigned long where 1 degree = 10 000 000

  EOS increased the precision here, even if we think that the gps is not precise enough, with 10e5 precision it has 76cm resolution
  with 10e7 it's around 1 cm now. Increasing it further is irrelevant, since even 1cm resolution is unrealistic, however increased 
  resolution also increased precision of nav calculations
*/
/* This calc adds approc 2km error to the gps coordinates reverted back to the original working one 
uint32_t GPS_coord_to_degrees(char* s) {
  char *p = s, *d = s;
  uint8_t min, deg = 0;
  uint16_t frac = 0, mult = 10000;
  
  while(*p) {                                   // parse the string until its end
    if (d != s) {frac+=(*p-'0')*mult;mult/=10;} // calculate only fractional part on up to 5 digits  (d != s condition is true when the . is located)
    if (*p == '.') d=p;                         // locate '.' char in the string
    p++;
  }
  if (p==s) return 0;
  while (s<d-2) {deg *= 10;deg += *(s++)-'0';}  // convert degrees : all chars before minutes ; for the first iteration, deg = 0
  min = *(d-1)-'0' + (*(d-2)-'0')*10;           // convert minutes : 2 previous char before '.'
  return deg * 10000000UL + (min * 100000UL + frac)*10UL / 6;
}
*/

#define DIGIT_TO_VAL(_x)	(_x - '0')
uint32_t GPS_coord_to_degrees(char* s)
{
	char *p, *q;
	uint8_t deg = 0, min = 0;
	unsigned int frac_min = 0;

	// scan for decimal point or end of field
	for (p = s; isdigit(*p); p++)
		;
	q = s;

	// convert degrees
	while ((p - q) > 2) {
		if (deg)
			deg *= 10;
		deg += DIGIT_TO_VAL(*q++);
	}
	// convert minutes
	while (p > q) {
		if (min)
			min *= 10;
		min += DIGIT_TO_VAL(*q++);
	}
	// convert fractional minutes
	// expect up to four digits, result is in
	// ten-thousandths of a minute
	if (*p == '.') {
		q = p + 1;
		for (int i = 0; i < 4; i++) {
			frac_min *= 10;
			if (isdigit(*q))
				frac_min += *q++ - '0';
		}
	}
	return deg * 10000000UL + (min * 1000000UL + frac_min*100UL) / 6;
}

// helper functions 
uint16_t grab_fields(char* src, uint8_t mult) {  // convert string to uint16
  uint8_t i;
  uint16_t tmp = 0;
  for(i=0; src[i]!=0; i++) {
    if(src[i] == '.') {
      i++;
      if(mult==0)   break;
      else  src[i+mult] = 0;
    }
    tmp *= 10;
    if(src[i] >='0' && src[i] <='9') tmp += src[i]-'0';
  }
  return tmp;
}

uint8_t hex_c(uint8_t n) {		// convert '0'..'9','A'..'F' to 0..15
  n -= '0';
  if(n>9)  n -= 7;
  n &= 0x0F;
  return n;
} 

/* This is a light implementation of a GPS frame decoding
   This should work with most of modern GPS devices configured to output NMEA frames.
   It assumes there are some NMEA GGA frames to decode on the serial bus
   Here we use only the following data :
     - latitude
     - longitude
     - GPS fix is/is not ok
     - GPS num sat (4 is enough to be +/- reliable)
     // added by Mis
     - GPS altitude (for OSD displaying)
     - GPS speed (for OSD displaying)
*/
#define FRAME_GGA  1
#define FRAME_RMC  2

bool GPS_newFrame(char c) {
  uint8_t frameOK = 0;
  static uint8_t param = 0, offset = 0, parity = 0;
  static char string[15];
  static uint8_t checksum_param, frame = 0;
  
  if (c == '$') {
    param = 0; offset = 0; parity = 0;
  } else if (c == ',' || c == '*') {
    string[offset] = 0;
    if (param == 0) { //frame identification
      frame = 0;
      if (string[0] == 'G' && string[1] == 'P' && string[2] == 'G' && string[3] == 'G' && string[4] == 'A') frame = FRAME_GGA;
      if (string[0] == 'G' && string[1] == 'P' && string[2] == 'R' && string[3] == 'M' && string[4] == 'C') frame = FRAME_RMC;
    } else if (frame == FRAME_GGA) {
      if      (param == 2)                     {GPS_coord[LAT] = GPS_coord_to_degrees(string);}
      else if (param == 3 && string[0] == 'S') GPS_coord[LAT] = -GPS_coord[LAT];//'S'
      else if (param == 4)                     {GPS_coord[LON] = GPS_coord_to_degrees(string);}
      else if (param == 5 && string[0] == 'W') GPS_coord[LON] = -GPS_coord[LON];//'W'
      else if (param == 6)                     {GPS_FIX = (string[0]  > '0');}
      else if (param == 7)                     {GPS_numSat = grab_fields(string,0);}
      //else if (param == 8)                     {GPS_numSat = grab_fields(string,0);}
      else if (param == 9)                     {GPS_altitude = grab_fields(string,3);}	// altitude in meters added by Mis
      //else if (param == 10)                     {GPS_altitude2 = grab_fields(string,3);}
      //else if (param == 11)                     {GPS_altitude2 = grab_fields(string,3);}
    } else if (frame == FRAME_RMC) {
      //if      (param == 7)                     {GPS_speed = ((uint32_t)grab_fields(string,1)*5144L)/1000L;}  //gps speed in cm/s will be used for navigation	
      //else if (param == 8)                     {GPS_ground_course = grab_fields(string,1); }                 //ground course deg*10 
    }
    param++; offset = 0;
    if (c == '*') checksum_param=1;
    else parity ^= c;
  } else if (c == '\r' || c == '\n') {
    if (checksum_param) { //parity checksum
      uint8_t checksum = hex_c(string[0]);
      checksum <<= 4;
      checksum += hex_c(string[1]);
      if (checksum == parity) frameOK = 1;
    }
    checksum_param=0;
  } else {
     if (offset < 15) string[offset++] = c;
     if (!checksum_param) parity ^= c;
  }
  if (frame) GPS_Present = 1;
  return frameOK && (frame==FRAME_GGA);
}//NMEA
