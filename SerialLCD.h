/*
 SerialLCD.h - Library for controlling a SparkFun serLCD module
	    Code written for firmware version 2.5

 Created by Cody B. Null, September 9, 2011
 Modified by Jordan Hochenbaum, Dec 5, 2011. (Version 1.5)
 Modified by Michael Shimniok, Dec 23, 2016. (Version 1.6)

 Version 1.4 - This version wrote specifically for 16x2
               Display.
               More display support coming in later version.

 Version 1.5 - Since mid 2011 the Arduino IDE has implemented the NewSoftSerial
               library updates in the standard SoftwareSerial library and so
               using NewSoftSerial is no longer needed. serLCD has been updated
               to use the Standard SoftwareSerial library. Also, Arduino 1.0
               support has been added. Backwards compatible with previous versions.

Version 1.6 - (shimniok) updated to use SoftSerialTx, transmit-only software serial

Note -	This library requires SoftSerialTx library at http://github.com/shimniok/SoftSerialTx
*/
#ifndef SerialLCD_h
#define SerialLCD_h

#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds,digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif
#include <SoftSerialTx.h>

// Commands
#define LCD_BACKLIGHT       0x80
#define LCD_CLEARDISPLAY	  0x01
#define LCD_CURSORLEFT		  0x10
#define LCD_CURSORRIGHT     0x14
#define LCD_DISPLAYCONTROL  0x08
#define LCD_ENTRYMODESET    0x04
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80
#define LCD_SETSPLASHSCREEN	0x0A
#define LCD_SPLASHTOGGLE    0x09
#define LCD_RETURNHOME      0x02
#define LCD_SCROLLLEFT	    0x18
#define LCD_SCROLLRIGHT     0x1C


// Flags for display entry mode
#define LCD_ENTRYRIGHT      0x00
#define LCD_ENTRYLEFT       0x02

// Flags for display on/off control
#define LCD_BLINKON         0x01
#define LCD_CURSORON        0x02
#define LCD_DISPLAYON       0x04

// Flags for display size
#define LCD_2LINE           0x02
#define LCD_4LINE           0x04
#define LCD_16CHAR          0x10
#define LCD_20CHAR          0x14

//  Flags for setting display size
#define LCD_SET2LINE        0x06
#define LCD_SET4LINE        0x05
#define LCD_SET16CHAR       0x04
#define LCD_SET20CHAR       0x03

class SerialLCD : public SoftSerialTx {
public:

	/**
	 * Creates a SerialLCD object, transmitting via pin
	 */
	SerialLCD(int pin);

	/**
	 * Clear display
	 */
	void clear();

	/**
	 * Move cursor to home position
	 */
	void home();

	/**
	 * Set backlight brightness
	 */
	void setBrightness(int val);

	/**
	 * Define the start up splash screen
	 */
	void setSplash();

	/**
	 * Turn on/off the splash screen on startup
	 */
	void toggleSplash();

	/**
	 * Enable cursor Blinking
	 */
	void blink();

	/**
	 * Disable cursor blinking
	 */
	void noBlink();

	/**
	 * Disable cursor blinking
	 */
	void cursor();

	/**
	 * Disable cursor blinking
	 */
	void noCursor();

	/**
	 * Disable cursor blinking
	 */
	void display();

	/**
	 * Disable cursor blinking
	 */
	void noDisplay();

	/**
	 * Set position of cursor
	 */
	void setCursor(int row, int col);

	/**
	 * Move cursor left
	 */
	void left();

	 /**
 	 * Move cursor right
 	 */
  void right();

	/**
	 * Move cursor to beginning of specified row
	 */
	void selectLine(int row);

	/**
	 * Set autoscroll direction left to right
	 */
	void leftToRight();

	/**
	 * Set autoscroll direction right to left
	 */
	void rightToLeft();

	/**
	 * Enable autoscroll
	 */
	void autoscroll();

	/**
	 * Disable autoscroll
	 */
	void noAutoscroll();

	/**
	 * Scroll entire screen left
	 */
	void scrollLeft();

	/**
	 * Scroll entire screen right
	 */
	void scrollRight();

	/**
	 * Create a custom character; 8 char limit, location starts with 0
	 * Charmap represents an 5x8 dot matrix bitmap
	 */
	void createChar(int location, uint8_t charmap[]);

	/**
	 * Display a custom character at location
	 */
	void printCustomChar(int location);

private:
	// Send command specified by value; automatically prefixed by 0xFE
	void command(uint8_t value);

	// Send special command; automatically prefixed by 0x7C
	void specialCommand(uint8_t);

	uint8_t _rowoffsets[4];	   // Row offsets used for setCursor()
	uint8_t _displayfunction;
	uint8_t _displaycontrol;	 // Display control bitfield
	uint8_t _displaymode;
	uint8_t _numlines;         // Number of rows/lines in the display
	uint8_t _numcols;          // Number of columns in the display
};

#endif
