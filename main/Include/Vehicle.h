/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 26.07.2019 14:00:52
 Libraries   :
 Author      :Grimm
 Description :This class will be used to perform actions that control the motor
       e.g. Move forward, move backwards, rotate etc.
******************************************************************/

// Prevent multiple inclusions
#ifndef VEHICLE_H
#define VEHICLE_H

// Include libraries
#include "Globals.h" // Contain global variables

// Struct for the vehicle wheel
struct Wheel
{
  unsigned int enablePin; // Important since we are using an h bridge
  unsigned int motorPin_1;
  unsigned int motorPin_2;
};

// Enumeration of an action being done by a vehicle
enum Status
{
  TURNING_RIGHT = 0,
  TURNING_SOFT_RIGHT,
  TURNING_LEFT,
  TURNING_SOFT_LEFT,
  REVERSING,
  MOVINGFORWARD,
  STOPPED
};

// Enumeration to hold the mode for the class
enum Mode
{
  FORWARD = 0,
  REVERSE 
};

class Vehicle
{
// This function is used to copy one wheel from one struct
// to another
void copyWheel(const Wheel &from, Wheel &to)
{
  to.enablePin = from.enablePin;
  to.motorPin_1 = from.motorPin_1;
  to.motorPin_2 = from.motorPin_2;
}

public:
  // Default constructor
  Vehicle()
  {
    // Set up the default wheel configuration
    // Front wheels
    frontLeftWheel.enablePin = 14;
    frontLeftWheel.motorPin_1 = 15;
    frontLeftWheel.motorPin_2 = 16;
    
    frontRightWheel.enablePin = 17;
    frontRightWheel.motorPin_1 = 18;
    frontRightWheel.motorPin_2 = 19;
    
    // Back wheels
    backLeftWheel.enablePin = 20;
    backLeftWheel.motorPin_1 = 21;
    backLeftWheel.motorPin_2 = 22;
    
    backRightWheel.enablePin = 23;
    backRightWheel.motorPin_1 = 24;
    backRightWheel.motorPin_2 = 25;
  }
  
  // Parameterised constructor
  Vehicle(const Wheel &frontRight, const Wheel &frontLeft,
      const Wheel &backRight, const Wheel &backLeft)
  {
    // Set up the wheels
    setWheels(frontRight, frontLeft, backRight, backLeft);
    
    // Run the setup
    setUp();
  }
  
  // Copy constructor
  Vehicle(const Vehicle &other)
  {
    // Copy the configuration of the wheels
    copyWheel(other.frontLeftWheel, frontLeftWheel);
    copyWheel(other.frontRightWheel, frontRightWheel);
    copyWheel(other.backLeftWheel, backLeftWheel);
    copyWheel(other.backRightWheel, backRightWheel);
  }
  
  // Accessor function
  // Get the status of the vehicle
  int getStatus() const
  {
    return status;    
  }
  
  // Get the wheels
  Wheel *getWheels()
  {
    // Assign values to correct 
    tempW[0] = frontLeftWheel;
    tempW[1] = frontRightWheel; 
    tempW[2] = backLeftWheel;
    tempW[3] = backRightWheel;
    
    return tempW;
  }
  
  // Set up the wheels
  void setWheels(const Wheel &frontRight, const Wheel &frontLeft,
      const Wheel &backRight, const Wheel &backLeft)
  {
    // Front wheels
    frontRightWheel.enablePin = frontRight.enablePin;
    frontRightWheel.motorPin_1 = frontRight.motorPin_1;
    frontRightWheel.motorPin_2 = frontRight.motorPin_2;
    
    frontLeftWheel.enablePin = frontLeft.enablePin;
    frontLeftWheel.motorPin_1 = frontLeft.motorPin_1;
    frontLeftWheel.motorPin_2 = frontLeft.motorPin_2;
    
    // Back wheels
    backRightWheel.enablePin = backRight.enablePin;
    backRightWheel.motorPin_1 = backRight.motorPin_1;
    backRightWheel.motorPin_2 = backRightWheel.motorPin_2;
    
    backLeftWheel.enablePin = backLeft.enablePin;
    backLeftWheel.motorPin_1 = backLeft.motorPin_1;
    backLeftWheel.motorPin_2 = backLeft.motorPin_2;
    
    // Run the setUp
    setUp();
  }
  
  // Overloaded function
  void setWheels(Wheel temp[])
  {
    setWheels(temp[0], temp[1], temp[2], temp[3]);
  }
  
  // Gets the mode for this class
  int getMode() const
  {
    return mode;
  }
  
  void setSpeed(unsigned int newSpeed)
  {
    // Check if the speed is  correct
    if(newSpeed > 100)
      return;
    
    // Set the new speed
    speedPercentage = newSpeed / 100;
    
    // Transfer this to the vehicle
    reduceSpeed();
  }
  
  unsigned int getSpeed() const
  {
    // Convert to percentage
    return speedPercentage * 100;
  }
  
  // Get the maximum speed for this vehicle
  unsigned int getMaxSpeed() const 
  {
  	  return maxSpeed;
  }
  
  // Set the mode for this class
  void setMode(int mode_)
  {
    // Prevent errors
    switch(mode_)
    {
    case FORWARD:
    case REVERSE:
      mode = mode_; 
      break;
    default:
      // Do nothing as the mode is unknown
      break;
    }
  }
  
  // The following functions are used to control the general direction of the
  // vehicle
  // Right direction
  void turnRight(int mySpeed = -1)
  {
    // Change the status of the vehicle
    status = TURNING_RIGHT;
      
    // Spin the two left wheels
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontLeftWheel.motorPin_1, HIGH);
    digitalWrite(frontLeftWheel.motorPin_2, LOW);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 0);
    digitalWrite(frontRightWheel.motorPin_1, HIGH);
    digitalWrite(frontRightWheel.motorPin_2, LOW);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backLeftWheel.motorPin_1, HIGH);
    digitalWrite(backLeftWheel.motorPin_2, LOW);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 0);
    digitalWrite(backRightWheel.motorPin_1, HIGH);
    digitalWrite(backRightWheel.motorPin_2, LOW);
  }
  
  void turnSoftRight(int mySpeed = -1)
  {
    // Change the status of the vehicle
    status = TURNING_SOFT_RIGHT;
    
    // Spin all the wheels, but the right wheels at 50% speed
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontLeftWheel.motorPin_1, HIGH);
    digitalWrite(frontLeftWheel.motorPin_2, LOW);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed(0.50));
    digitalWrite(frontRightWheel.motorPin_1, HIGH);
    digitalWrite(frontRightWheel.motorPin_2, LOW);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backLeftWheel.motorPin_1, HIGH);
    digitalWrite(backLeftWheel.motorPin_2, LOW);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed(0.50)); 
    digitalWrite(backRightWheel.motorPin_1, HIGH);
    digitalWrite(backRightWheel.motorPin_2, LOW);
  }
  
  // Left direction
  void turnLeft(int mySpeed = -1)
  {
    // Change the status of the vehicle
    status = TURNING_LEFT;
    
    // Spin right wheels
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 0);
    digitalWrite(frontLeftWheel.motorPin_1, HIGH);
    digitalWrite(frontLeftWheel.motorPin_2, LOW);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontRightWheel.motorPin_1, HIGH);
    digitalWrite(frontRightWheel.motorPin_2, LOW);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 0);
    digitalWrite(backLeftWheel.motorPin_1, HIGH);
    digitalWrite(backLeftWheel.motorPin_2, LOW);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backRightWheel.motorPin_1, HIGH);
    digitalWrite(backRightWheel.motorPin_2, LOW);
  }
  
  void turnSoftLeft(int mySpeed = -1)
  {
    // Change the status of the vehicle
    status = TURNING_SOFT_LEFT;
    
    // Spin all the wheels but the right wheels at 50% speed
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed(0.50));
    digitalWrite(frontLeftWheel.motorPin_1, HIGH);
    digitalWrite(frontLeftWheel.motorPin_2, LOW);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontRightWheel.motorPin_1, HIGH);
    digitalWrite(frontRightWheel.motorPin_2, LOW);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed(0.50));
    digitalWrite(backLeftWheel.motorPin_1, HIGH);
    digitalWrite(backLeftWheel.motorPin_2, LOW);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backRightWheel.motorPin_1, HIGH);
    digitalWrite(backRightWheel.motorPin_2, LOW);
  }
  
  // Forward
  void moveForward(int mySpeed = -1)
  {	
    // Change the mode
    mode = FORWARD;
    
    // Change the status of the vehicle
    status = MOVINGFORWARD;
    
    // Spin all the wheels
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontLeftWheel.motorPin_1, HIGH);
    digitalWrite(frontLeftWheel.motorPin_2, LOW);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontRightWheel.motorPin_1, HIGH);
    digitalWrite(frontRightWheel.motorPin_2, LOW);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backLeftWheel.motorPin_1, HIGH);
    digitalWrite(backLeftWheel.motorPin_2, LOW);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backRightWheel.motorPin_1, HIGH);
    digitalWrite(backRightWheel.motorPin_2, LOW);
  }
  
  // Alias to moveForward
  void forward(int mySpeed = -1)
  {
    moveForward(mySpeed); 
  }
  
  // Reverse
  void moveBack(int mySpeed = -1)
  {
    // Change the mode
    mode = REVERSE;
    
    // Change the status of the vehicle
    status = REVERSING;
    
    // Spin all the wheels in reverse
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontLeftWheel.motorPin_1, LOW);
    digitalWrite(frontLeftWheel.motorPin_2, HIGH);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(frontRightWheel.motorPin_1, LOW);
    digitalWrite(frontRightWheel.motorPin_2, HIGH);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backLeftWheel.motorPin_1, LOW);
    digitalWrite(backLeftWheel.motorPin_2, HIGH);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 
    mySpeed > -1 && mySpeed <= maxSpeed ? mySpeed : calculateSpeed());
    digitalWrite(backRightWheel.motorPin_1, LOW);
    digitalWrite(backRightWheel.motorPin_2, HIGH);
  }
  
  // Alias to move back
  void reverse(int mySpeed = -1)
  {
    moveBack(mySpeed);
  }
  
  // Stop or Brake
  void stop()
  {
    // Change the mode
    mode = FORWARD;
    
    // Change the status of the vehicle
    status = STOPPED;
    
    // Stop all the wheels from spinning
    // Front left wheel
    analogWrite(frontLeftWheel.enablePin, 0);
    digitalWrite(frontLeftWheel.motorPin_1, LOW);
    digitalWrite(frontLeftWheel.motorPin_2, LOW);
    
    // Front right wheel
    analogWrite(frontRightWheel.enablePin, 0);
    digitalWrite(frontRightWheel.motorPin_1, LOW);
    digitalWrite(frontRightWheel.motorPin_2, LOW);
    
    // Back left wheel
    analogWrite(backLeftWheel.enablePin, 0);
    digitalWrite(backLeftWheel.motorPin_1, LOW);
    digitalWrite(backLeftWheel.motorPin_2, LOW);
    
    // Back right wheel
    analogWrite(backRightWheel.enablePin, 0);
    digitalWrite(backRightWheel.motorPin_1, LOW);
    digitalWrite(backRightWheel.motorPin_2, LOW);
  }
  
  // Reduce speed
  void reduceSpeed(float newSpeed)
  {
    if(!newSpeed > 1)
    {
      speedPercentage = newSpeed;
      
      // Transfer the new speed to the vehicle
      reduceSpeed();    
    }
  }
  
  void reduceSpeed()
  {
    // Repeat all the actions the vehicle is currently doing but with a new
    // speed
    switch(status)
    {
    case TURNING_RIGHT:
      turnRight();
      break;
    
    case TURNING_SOFT_RIGHT:
      turnSoftRight();
      break;
    
    case TURNING_LEFT:
      turnLeft();
      break;
    
    case TURNING_SOFT_LEFT:
      turnSoftLeft();
      break;
    
    case MOVINGFORWARD:
      forward();
      break;
    
    case REVERSING:
      reverse();
      break;
    
    default:
      // Do not do anything
      break;
    }
  }
  
  // Allows individual manipulation of a wheel
  void setIndivdualWheel(Wheel &w, int newSpeed)
  {    
    // Set the new speed of the motor
    analogWrite(w.enablePin, newSpeed);
    
    switch(status)
    {
    case MOVINGFORWARD:
      digitalWrite(w.motorPin_1, HIGH);
      digitalWrite(w.motorPin_2, LOW);
      break;
    
    case REVERSING:
      digitalWrite(w.motorPin_1, LOW);
      digitalWrite(w.motorPin_2, HIGH);
      break;
    
    default:
      // Do not do anaything
      break;
    }
  }
  
  // Setup the class
  void setUp()
  {
    // Create a variable to temporarily hold an array of wheels
    Wheel *temp = getWheels();
    
    // Loop 4 times i.e for each wheel
    for(int i = 0; i < 4; i++)
    {
      pinMode(temp[i].enablePin, OUTPUT);
      pinMode(temp[i].motorPin_1, OUTPUT);
      pinMode(temp[i].motorPin_2, OUTPUT);  
    } // End for loop
    
    // Set the default mode to be forward
    mode = FORWARD;
    
    // Set the speed to be initially 0
    speedPercentage = 1;
    
    // Set the default status to stopped
    status = STOPPED;
  }

private:
  int mode; // The mode of the vehicle. 0 - Forward, 1 - Reverse
  Wheel tempW[4]; // Temporary array of wheels. Not to be directly used
  float speedPercentage;
  static const int maxSpeed = 150; // Change this to regulate the enable pin output
  int status; // Record the action being done currently
              // by the vehicle
  
  // The front wheel
  Wheel frontRightWheel;
  Wheel frontLeftWheel;
  
  // The back wheel
  Wheel backRightWheel;
  Wheel backLeftWheel;
  
  // Utility functions
  // Calculates the speed and returns it
  int calculateSpeed(float percentage = 1)
  {	
    if(percentage > 1)
		return 0;
      
      
    return speedPercentage * maxSpeed * percentage;
  }
}; // End class Vehicle
#endif