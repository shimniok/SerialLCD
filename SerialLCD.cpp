/*
 SerialLCD.h - Library for controlling a SparkFun SerialLCD module
	    Code written for firmware version 2.5

 Created by Cody B. Null, September 9, 2011
 Modified by Jordan Hochenbaum, Dec 5, 2011. (Version 1.5)
 Modified by Michael Shimniok, Dec 23, 2016. (Version 1.6)

 Version 1.4 - This version wrote specifically for 16x2
               Display.
               More display support coming in later version.

 Version 1.5 - Since mid 2011 the Arduino IDE has implemented the NewSoftSerial
               library updates in the standard SoftwareSerial library and so
               using NewSoftSerial is no longer needed. SerialLCD has been updated
               to use the Standard SoftwareSerial library. Also, Arduino 1.0
               support has been added. Backwards compatible with previous versions.

Version 1.6 - (shimniok) updated to use SoftSerialTx, transmit-only software serial

Note -	This library requires SoftSerialTx library at http://github.com/shimniok/SoftSerialTx
*/

#include <SoftSerialTx.h>
#include "SerialLCD.h"

//	PUBLIC FUNCTIONS

// Contstructor
// defaults to 16x2 display
SerialLCD::SerialLCD(int pin) : SoftSerialTx(pin) {
	pinMode(pin, OUTPUT);
	begin(9600);
	_numlines = LCD_2LINE;
	_numcols = LCD_16CHAR;
	_rowoffsets[0] = 0x00;
	_rowoffsets[1] = 0x40;
	_rowoffsets[2] = 0x00 + 0x27;
	_rowoffsets[3] = 0x40 + 0x27;
}

/* Initialize.. not used trying to implement all display sizes
void SerialLCD::init(int pin, int rows, int cols){
	pinMode(pin, OUTPUT);
	delay(4);
	begin(9600);
	if(cols == LCD_20CHAR){
		_numchars = LCD_20CHAR;
		specialCommand(LCD_SET20CHAR);
	}else{ // default to 16 char display
		_numchars = LCD_16CHAR;
		specialCommand(LCD_SET16CHAR);
	}
	if(rows == LCD_4LINE){
		_rowoffset = 1;
		_numlines = LCD_4LINE;
		specialCommand(LCD_SET4LINE);
	}else{ // default to 2 line if input was invalid
		_rowoffset = 0;
		_numlines = LCD_2LINE;
		specialCommand(LCD_SET2LINE);
	}
	// clear the display
	clear();
	// set brightness to full
	setBrightness(30);
}
*/

// Set brightness value range 1-30 1=OFF 30=FULL
void SerialLCD::setBrightness(int val){
	if(val >= 1 && val <= 30){
		specialCommand(LCD_BACKLIGHT | (val - 1));
	}
}

// Clears screen and returns cursor to home position
void SerialLCD::clear(){
	command(LCD_CLEARDISPLAY);
}

// Clears a single line by writing blank spaces then returning
// cursor to beginning of line
void SerialLCD::clearLine(int num){
	if(num > 0 && num <= _numlines){
		setCursor(num, 1);
		print("                ");
		setCursor(num, 1);
	}
}

// Moves cursor to the beginning of selected line
void SerialLCD::selectLine(int num){
	if(num > 0 && num <= _numlines){
		setCursor(num, 1);
	}
}

// returns cursor to home position
void SerialLCD::home(){
	command(LCD_RETURNHOME);
}

// Saves first 2 lines of txt to splash screen memory
void SerialLCD::setSplash(){
	specialCommand(LCD_SETSPLASHSCREEN);
}

// Toggles splashscreen on and off
void SerialLCD::toggleSplash(){
	specialCommand(LCD_SPLASHTOGGLE);
}

//  This is for text that flows Left to Right
void SerialLCD::leftToRight(){
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void SerialLCD::rightToLeft() {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Blinking cursor on/off
void SerialLCD::blink(){
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SerialLCD::noBlink(){
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Underline cursor on/off
void SerialLCD::cursor(){
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SerialLCD::noCursor(){
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SerialLCD::display(){
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SerialLCD::noDisplay(){
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Set cursor to specific row and col values start count at 0
void SerialLCD::setCursor(int row, int col){
	uint8_t position;
  if (row >= 0 && row <= 3) {
      position = col + _rowoffsets[row];
  }
	command(LCD_SETDDRAMADDR|position);
}

// Creates custom characters 8 char limit
// Input values start with 1
void SerialLCD::createChar(int location, uint8_t charmap[]){
	location -= 1;
	location &= 0x07;
  for (int i=0; i<8; i++){
    command(LCD_SETCGRAMADDR | (location << 3) | i);
    write(charmap[i]);
  }
}

// Prints custom character
// Input values start with 1
void SerialLCD::printCustomChar(int num){
	write((num - 1));
}

// PRIVATE FUNCTIONS

// Functions for sending the special command values
void SerialLCD::command(uint8_t value){
	write(0xFE);
	write(value);
	delay(5);
}
void SerialLCD::specialCommand(uint8_t value){
	write(0x7C);
	write(value);
	delay(5);
}
