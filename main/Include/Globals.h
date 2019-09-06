/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 26.07.2019 14:00:52
 Libraries   :
 Author      :Grimm
 Description :This header file holds global variables to be shared
 	 	 	  over multiple files
WARNING: Do not modify this file unless needed as this may cause unwanted
behaviors
******************************************************************/

// Prevent multiple inclusions
#ifndef GLOBALS_H
#define GLOBALS_H

// Includes
#include "CircularBuffer.h"

// Global functions
// This function calculates the number of steps written
template<typename T>
long getSizeOfArray(const T array[])
{
	return sizeof(array) / sizeof(T);
}

// Global variables
// Enumeration that hold possible actions by the vehicle
// A is an alias for ActionsToTake
enum ActionsToTake
{
  A_STRAIGHT = 0,
  A_RIGHT,
  A_BACKWARD,
  A_LEFT,
  A_UTURN,
  A_STOP,
  A_DROP_OBJECT,
  A_PICK_OBJECT
};

// WARNING: If you change the values in my object please update the new number 
// of actions here
const int totalActions = 25; // NOTE: Last index of the array will be totalActions - 1 
 
// Keeps track of the current action index
int currentAction = 0;

// Update the the current action index
void updateActionIndex(unsigned int newValue)
{
	if((currentAction - newValue) > 1)
	{
		currentAction++;
		
		return;
	}
	else		
		currentAction = constrain(newValue, 0, totalActions - 1);
}

// Hold the maximum directions that can the vehicle takes
int myActions[totalActions] = 
{
	A_STRAIGHT,
	A_RIGHT,
	A_LEFT,
	A_LEFT,
	A_PICK_OBJECT,
	A_UTURN,
	A_STRAIGHT,
	A_STRAIGHT,
	A_LEFT,
	A_RIGHT,
	A_DROP_OBJECT,
	A_UTURN,
	A_LEFT,
	A_RIGHT,
	A_STRAIGHT,
	A_RIGHT,
	A_PICK_OBJECT,
	A_UTURN,
	A_RIGHT,
	A_STRAIGHT,
	A_STRAIGHT,
	A_LEFT,
	A_DROP_OBJECT,
	A_UTURN,
	A_STOP
};

// This holds the last 24 actions performed by the manager class
CircularBuffer<int, 25> actions;

// Directions to take
int frontLineType = 0;
int frontDirections = 0;
int backLineType = 0;
int backDirections = 0;

// Other definitions
#if defined(TEENSYDUINO) 

    //  --------------- Teensy -----------------

    #if defined(__AVR_ATmega32U4__)
        #define BOARD "Teensy 2.0"
    #elif defined(__AVR_AT90USB1286__)       
        #define BOARD "Teensy++ 2.0"
    #elif defined(__MK20DX128__)       
        #define BOARD "Teensy 3.0"
    #elif defined(__MK20DX256__)       
        #define BOARD "Teensy 3.2" // and Teensy 3.1 (obsolete)
    #elif defined(__MKL26Z64__)       
        #define BOARD "Teensy LC"
    #elif defined(__MK64FX512__)
        #define BOARD "Teensy 3.5"
    #elif defined(__MK66FX1M0__)
        #define BOARD "Teensy 3.6"
    #else
       #error "Unknown board"
    #endif

#else // --------------- Arduino ------------------

    #if   defined(ARDUINO_AVR_ADK)       
        #define BOARD "Mega Adk"
    #elif defined(ARDUINO_AVR_BT)    // Bluetooth
        #define BOARD "Bt"
    #elif defined(ARDUINO_AVR_DUEMILANOVE)       
        #define BOARD "Duemilanove"
    #elif defined(ARDUINO_AVR_ESPLORA)       
        #define BOARD "Esplora"
    #elif defined(ARDUINO_AVR_ETHERNET)       
        #define BOARD "Ethernet"
    #elif defined(ARDUINO_AVR_FIO)       
        #define BOARD "Fio"
    #elif defined(ARDUINO_AVR_GEMMA)
        #define BOARD "Gemma"
    #elif defined(ARDUINO_AVR_LEONARDO)       
        #define BOARD "Leonardo"
    #elif defined(ARDUINO_AVR_LILYPAD)
        #define BOARD "Lilypad"
    #elif defined(ARDUINO_AVR_LILYPAD_USB)
        #define BOARD "Lilypad Usb"
    #elif defined(ARDUINO_AVR_MEGA)       
        #define BOARD "Mega"
    #elif defined(ARDUINO_AVR_MEGA2560)       
        #define BOARD "Mega 2560"
    #elif defined(ARDUINO_AVR_MICRO)       
        #define BOARD "Micro"
    #elif defined(ARDUINO_AVR_MINI)       
        #define BOARD "Mini"
    #elif defined(ARDUINO_AVR_NANO)       
        #define BOARD "Nano"
    #elif defined(ARDUINO_AVR_NG)       
        #define BOARD "NG"
    #elif defined(ARDUINO_AVR_PRO)       
        #define BOARD "Pro"
    #elif defined(ARDUINO_AVR_ROBOT_CONTROL)       
        #define BOARD "Robot Ctrl"
    #elif defined(ARDUINO_AVR_ROBOT_MOTOR)       
        #define BOARD "Robot Motor"
    #elif defined(ARDUINO_AVR_UNO)       
        #define BOARD "Uno"
    #elif defined(ARDUINO_AVR_YUN)       
        #define BOARD "Yun"

    // These boards must be installed separately:
    #elif defined(ARDUINO_SAM_DUE)       
        #define BOARD "Due"
    #elif defined(ARDUINO_SAMD_ZERO)       
        #define BOARD "Zero"
    #elif defined(ARDUINO_ARC32_TOOLS)       
        #define BOARD "101"
    #else
       #error "Unknown board"
    #endif
  
#endif

#endif