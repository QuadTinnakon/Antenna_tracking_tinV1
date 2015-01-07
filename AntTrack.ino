/*
project_antenna_tracking 
by: tinnakon kheowree  
tinnakon_za@hotmail.com
tinnakonza@gmail.com
http://quad3d-tin.lnwshop.com/
https://www.facebook.com/tinnakonza
*/
void antenna_tracking(float lon1, float lat1, float lon2, float lat2, float alt){
   //antena_bearing(home.lng, home.lat, current_loc.lng/10, current_loc.lat/10);
  
  float a, tc1, R, c, d, dLat, dLon;
  
  //lon1/=10000000.0;
  //lat1/=10000000.0;
  //lon2/=10000000.0;
  //lat2/=10000000.0;
  
  lon1=lon1*ToRad;
  lat1=lat1*ToRad;
  lon2=lon2*ToRad;
  lat2=lat2*ToRad;
  
  R=6371000.0;    //raio da terra 6371km

  a=atan2(sin(lon2-lon1)*cos(lat2), cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1));
  yaw=a*180/PI;
  if (yaw<0) yaw=360+yaw;
 
//calculo da distancia entre modelo e home
  dLat = (lat2-lat1);
  dLon = (lon2-lon1);
  a = sin(dLat/2) * sin(dLat/2) + sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2); 
  c = 2* asin(sqrt(a));  
  d = 6371000 * c;
  alt=alt-Alt_Home;
  pitch=atan(alt/d);
  pitch=pitch*360/(2*PI);
  Distance=sqrt(alt*alt+d*d);
  //return yaw, pitch;
}

