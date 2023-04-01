#include <U8glib.h>
#include <FPS_GT511C3.h>

/*****************************************************************
	FPS_IDFinger.ino - Library example for controlling the GT-511C3 Finger Print Scanner (FPS)
	Created by Josh Hawley, July 23rd 2013
	Licensed for non-commercial use, must include this license message
	basically, Feel free to hack away at it, but just give me credit for my work =)
	TLDR; Wil Wheaton's Law

	Description: This sketch will attempt to identify a previously enrolled fingerprint
	saved in its database.
	
	This code should work with the any model of ADH-Tech's FPS as long as
	you are within the minimum logic level threshold for the FPS serial UART.
	This code has been tested with these models:

              GT-521F52 [ https://www.sparkfun.com/products/14585 ]
              GT-521F32 [ https://www.sparkfun.com/products/14518 ]
              GT-511C3  [ https://www.sparkfun.com/products/11792 ]
              GT-511C1R [ https://www.sparkfun.com/products/13007 ]

-------------------- HARDWARE HOOKUP with 5V Arduino --------------------
1.) Dedicated Bi-Directional Logic Level Converter (LLC)
It is recommended to use a dedicated bi-direcitonal LLC
[ https://www.sparkfun.com/products/12009 ] for a reliable connection if you
are using a 5V Arduino microcontroller:
   Fingerprint Scanner (Pin #) <-> Logic Level Converter <-> 5V Arduino w/ Atmega328P
     UART_TX (3.3V TTL)(Pin 1) <->     LV1 <-> HV1       <->  RX (pin 4)
     UART_RX (3.3V TTL)(Pin 2) <->     LV4 <-> HV4       <->  TX (pin 5)
           GND         (Pin 3) <->     GND <-> GND       <->     GND
      Vin (3.3V~6V)    (Pin 4) <->        HV             <->      5V
                                          LV             <->     3.3V
					  
2.) Voltage Division w/ 3x 10kOhm Resistors
Otherwise, you could use 3x 10kOhm resistors [ https://www.sparkfun.com/products/11508 ]
to divide the voltage from a 5V Arduino down to 3.3V FPS similar to the
"Uni-Directional" application circuit on our old logic level converter
[ https://cdn.sparkfun.com/assets/b/0/e/1/0/522637c6757b7f2b228b4568.png ]:
    Voltage Divider         <-> Fingerprint Scanner(Pin #) <-> Voltage Divider <-> 5V Arduino w/ Atmega328P
                            <-> UART_TX (3.3V TTL) (Pin 1) <->                 <->       RX (pin 4)
  GND <-> 10kOhm <-> 10kOhm <-> UART_RX (3.3V TTL) (Pin 2) <->      10kOhm     <->       TX (pin 5)
          GND               <->        GND         (Pin 3) <->       GND       <->        GND
                            <->    Vin (3.3V~6V)   (Pin 4) <->                 <->        5V
			    
Note: You can add the two 10kOhm resistors in series for 20kOhms. =)
--------------------------------------------------------------------------------
*****************************************************************/

//RST=8,D/C=9,CS=10,SCL=13,DIN= 11// 

#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

// set up software serial pins for Arduino's w/ Atmega328P's
// FPS (TX) is connected to pin 4 (Arduino's Software RX)
// FPS (RX) is connected through a converter to pin 5 (Arduino's Software TX)
FPS_GT511C3 fps(4, 5); // (Arduino SS_RX = pin 4, Arduino SS_TX = pin 5)
U8GLIB_SSD1306_128X64 u8g(13, 11, 10 ,9, 8);
//U8G2_SSD1306_128X64_ALT0_F_4W_HW_SPI u8g(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
/*If using another Arduino microcontroller, try commenting out line 53 and
uncommenting line 62 due to the limitations listed in the
library's note => https://www.arduino.cc/en/Reference/softwareSerial . Do
not forget to rewire the connection to the Arduino*/

// FPS (TX) is connected to pin 10 (Arduino's Software RX)
// FPS (RX) is connected through a converter to pin 11 (Arduino's Software TX)
//FPS_GT511C3 fps(10, 11); // (Arduino SS_RX = pin 10, Arduino SS_TX = pin 11)
void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}
void draw1(void)
{
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 1, 21, "VERIFIED");

}
void draw2(void)
{
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 1, 21, "WRONG");
}
void presss(void)
{
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 13, 21, "PLACE YOUR");
  u8g.drawStr( 13, 41, "FINGER");

}

void u8g_line(uint8_t a) {
  u8g.drawStr( 0, 0, "");
  u8g.drawLine(7+a, 10, 40, 55);
  u8g.drawLine(7+a*2, 10, 60, 55);
  u8g.drawLine(7+a*3, 10, 80, 55);
  u8g.drawLine(7+a*4, 10, 100, 55);
}
void welcome(void)
{
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 15, 21, "SYSTEM IS");
  u8g.drawStr( 15, 41, "STARTING... ");
  //u8g.drawStr( 38, 61, "ROBOT");
}
uint8_t draw_state = 0;
void animation(void) {
  u8g_prepare();
  switch(draw_state >> 1) {
    case 0: u8g_line(draw_state&7); break;

  }
}
void setup()
{
	Serial.begin(9600); //set up Arduino's hardware serial UART
	delay(100);
	fps.Open();         //send serial command to initialize fps
	fps.SetLED(true);   //turn on LED so fps can see fingerprint
  pinMode(13, OUTPUT);
  pinMode(7, OUTPUT);


  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  pinMode(8, OUTPUT);
}

void loop()
{
	// Identify fingerprint test
	if (fps.IsPressFinger())
	{
		fps.CaptureFinger(false);
		int id = fps.Identify1_N();
		
	     /*Note:  GT-521F52 can hold 3000 fingerprint templates
                GT-521F32 can hold 200 fingerprint templates
                 GT-511C3 can hold 200 fingerprint templates. 
		            GT-511C1R can hold 20 fingerprint templates.
			 Make sure to change the id depending on what
			 model you are using */

       u8g.firstPage();  
  do {
    presss();
  } while( u8g.nextPage() );
		if (id <200) //<- change id value depending model you are using      
		{//if the fingerprint matches, provide the matching template ID
			Serial.print("Verified ID:");
			Serial.println(id);
      digitalWrite(13,HIGH);
      digitalWrite(7,HIGH);


       u8g.firstPage();  
  do {
    animation();
  } while( u8g.nextPage() );

  delay(100);
      u8g.firstPage();  
  do {
    delay(100);
    draw1();
    delay(100);                                 //verified function//
  } while( u8g.nextPage() );
  delay(100);
  u8g.firstPage();  
  do {
    welcome();
  } while( u8g.nextPage() );
  delay(1000);
		}
		else
		{//if unable to recognize
			Serial.println("Finger not found");
            u8g.firstPage();  
  do {
    animation();
    
  } while( u8g.nextPage() );
      delay(100);
      u8g.firstPage(); 
       do {
          delay(100);
          draw2();
          delay(100);
  } while( u8g.nextPage() );
		}
	}
	else
	{
		Serial.println("Please press finger");
          u8g.firstPage();  
  do {
    presss();
  } while( u8g.nextPage() );
	}
	delay(100);

}
