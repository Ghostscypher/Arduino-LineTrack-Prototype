/******************************************************************
  Created with PROGRAMINO IDE for Arduino - 26.07.2019 14:00:52
  Libraries   :Line_tack.h - The main line tracking library
  Author      :Grimm
  Description :The main sketch
******************************************************************/
// Header files
#include "include/Line_Track.h"
#include "include/Object_Detection.h"
#include "include/Globals.h" // Contains global variables
#include "include/Vehicle.h"
#include "include/Manager.h"

// Create line track object for purpose of line tracking
LineTrack line;

// Create a vehicle object
Vehicle vh;

// Create an object detection object
ObjectDetection objD;

// Create a manager object
Manager master_man;

// Array of directions
// NOTE: Change the directions to take in Globals.h

void setup()
{
  // Set up the serial output
  Serial.begin(9600);
  Serial.println("I am here");
  
  // Create the struct for front and back sensors
  FrontSensors fs;
  BackSensors bs;

  // The 3 front sensors, change the pin here, fs => front sensor
  /********************************************************
    The pins are arranged this way
    Left = pin1
    Center = pin2
    Right = pin3
  **********************************************************/
  fs.pin1 = 31; // Left side
  fs.pin2 = 33; // Center side
  fs.pin3 = 35; // Right side

  // The 4 Back sensors change the pins here, bs => back sensor
  /*******************************************************
    The pins are arranged this way
    Left = pin1
    Center-left = pin2
    Center-right = pin3
    Right = pin4
  **********************************************************/
  bs.pin1 = 37; // Left side
  bs.pin2 = 39; // Center-left side
  bs.pin3 = 41; // Center-right side
  bs.pin4 = 43; // Right side

  // Load the pins into the LineTrack class. Change to corresponding
  // pins above
  line.setFrontSensorPins(fs); // WARNING: Do not modify this line
  line.setBackSensorPins(bs); // WARNING: Do not modify this
  
  // Create wheels
  /************************************************************
   0 - Front left wheel
   1 - Front right wheel
   2 - Back left wheel
   3 - Back right wheel
   
   
   [0]---------[1]
    |           |
    |           |
   [2]---------[3]
  *************************************************************/
  Wheel temp[4];
  
  // Front wheels
  // Front left wheel
  temp[0].enablePin = 7;
  temp[0].motorPin_1 = 6;
  temp[0].motorPin_2 = 5;
  
  // Front right wheel
  temp[1].enablePin = 10;
  temp[1].motorPin_1 = 22;
  temp[1].motorPin_2 = 24;
  
  // Back wheel
  // Back left wheel
  temp[2].enablePin = 2;
  temp[2].motorPin_1 = 3;
  temp[2].motorPin_2 = 4;
  
  // Back right wheel
  temp[3].enablePin = 9;
  temp[3].motorPin_1 = 28;
  temp[3].motorPin_2 = 26;
  
  // Set up the wheels, change the pins above
  vh.setWheels(temp); // Warning: DO NOT MODIFY this line
  
  // Set up the surface type here
  /***********************************************
  It can either be
    BLACK_SURFACE_WHITE_LINE = 0
    WHITE_SURFACE_BLACK_LINE = 1
  ************************************************/
  line.setFieldConfiguration(BLACK_SURFACE_WHITE_LINE);
  
  // Set up the object detection sensor
  UltraSoundSensor uss;
  
  // Change the sensor pin here
  uss.triggerPin = 40;
  uss.echoPin = 41;
  
  // Set up the object detection class
  objD.setSensor(uss);
  
  // Pass the vehicle, line, and ObjectDetecion object into our manager
  // WARNING: Do not modify this line
  master_man.setClass(line, vh, objD);
  
  // Setup the serial to be used to communicate with the other
  // arduino
  /**************************************************
  // Configuration in an Arduino mega
    SERIAL_0 = Serial // Default // Pins 0(RX) and 1(TX)
    SERIAL_1 = Serial1 // Pins 19(RX) and 18(TX)
    SERIAL_2 = Serial2 // Pins 17(RX) and 16(TX)
    SERIAL_3 = Serial3 // Pins 15(RX) and 14(TX)
    ***************************************************/
  // NB: Change the serial here
  master_man.setSerialUsed(SERIAL_1);
  
  // Indicates whether the board is active
  bool boardActive = false;
  
  // Do a POST test to check if the other arduino is active
  // Loop 10 times to listen for ping request from the other arduino
  for(int i = 0; i < 10; i++)
  { 
    if(master_man.ping())
    {
      boardActive = true;
      
      break;
    }
  }

  // Check if the other board is active, if not raise the error
  /*if(!boardActive)
    master_man.raiseError("Could not connect to the other board");*/
}

void loop()
{
  // Start our manager class
  /*if(!master_man.isActive())
    master_man.start();*/

    vh.forward();
    delay(10000);

    vh.reverse();
    delay(10000);
}
