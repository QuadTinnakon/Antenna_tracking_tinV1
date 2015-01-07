/*
project_antenna_tracking 
by: tinnakon kheowree  
tinnakon_za@hotmail.com
tinnakonza@gmail.com
http://quad3d-tin.lnwshop.com/
https://www.facebook.com/tinnakonza
*/
//servos 
//(328 = pin 9) PB1 - OC1A (2560 = pin 11)
//(328 = pin 10) PB2 - OC1B (2560 = pin 12)
// ------------------------------------------------

void init_PWM()
{
	// Servo setup
	// Setting up the Timer 1 - 16 bit timer
	TCCR1A = ((1<<WGM11) | (1<<COM1B1) | (1<<COM1A1)); //Fast PWM: ICR1=TOP, OCR1x=BOTTOM,TOV1=TOP
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11); // Clock scaler = 8, 2,000,000 counts per second
	OCR1A = 3000;	// Rudder  - multiply your value * 2; for example 3000 = 1500 = 45°; 4000 = 2000 = 90°
	OCR1B = 3000; 	// Elevator
        ICR1 = 40000; 	//50hz freq...Datasheet says	(system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz = 40000,
}
void writeServo(int servo1, int servo2)
{
 servo1 = constrain(servo1,810,2200);
 servo2 = constrain(servo2,810,2200);
 OCR1A = servo1 * 2;//pin 11
 OCR1B = servo2 * 2;//pin 12 
}
