/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 26.07.2019 14:00:52
 Libraries   :
 Description :This class will be used to manage every aspect of the arduino
        board, as well as providing an API to communicate with other
        arduino boards.
******************************************************************/

// Prevent multiple inclusions
#ifndef MANAGER_H
#define MANAGER_H

// Includes
#include "Line_track.h"
#include "Vehicle.h"
#include "Object_Detection.h"
#include "Globals.h" // Contains global variables

// Enumeration of rules to be shared between the maneger class.
// m is an alias to mean Manager
enum SharedRules
{
  M_START = 0,
  M_STOP,
  M_STOP_ERROR,
  M_DONE  
};

// Enumeration of the serial output to use
// Applicabel to only araduino mega
enum SerialToUse
{
  SERIAL_0 = 0,
  SERIAL_1,
  SERIAL_2,
  SERIAL_3
};

class Manager
{
public:
  // Default constructor
  Manager()
  {
    vehicle = NULL;
    line = NULL;
    objDetection = NULL;
    
    // Run the setup
    setUp();
  }
  
  // Parameterized constructor
  Manager(LineTrack &line_, Vehicle &vehicle_, ObjectDetection &objD)
  {
    // Assign the objects as pointers
    vehicle = &vehicle_;
    line = &line_;
    objDetection = &objD;
    
    // Set the serial to be used
    setSerialUsed(SERIAL_0);
        
    // Run the setup
    setUp();
  }
  
  // Copy constructor
  Manager(const Manager &other)
  {
    vehicle = other.vehicle;
    line = other.line;
    objDetection = other.objDetection;
    
    // Set the serial to be used
    setSerialUsed(other.serialUsed);
    
    // Run the setup
    setUp();
  }
  
  // Accesor funtions
  // Checks if this class is active
  bool isActive() const
  {
    return active;
  }
  
  // Set the classes to be used
  void setClass(LineTrack &line_, Vehicle &vehicle_, ObjectDetection &objD)
  {
    line = &line_;
    vehicle = &vehicle_;
    objDetection = &objD;
    
    // Set the serial to be used
    setSerialUsed(SERIAL_0);
    
    // Run the setup
    setUp();
  }
  
  // Sets the serial class to use Serial by default
  void setSerialUsed(int serialToUse_)
  {
    switch(serialToUse_)
    {
    case SERIAL_0:
    case SERIAL_1:
    case SERIAL_2:
    case SERIAL_3:  
      serialUsed = serialToUse_;
      break;
    
    default:
      // Do not change anything
      break;
    }
    
    // Setup my serial
    setUpMySerial();
  }
  
  // Gets the serial reader or writer used by this class
  int getSerialUsed() const
  {
    return serialUsed;
  }
  
  
  // This function is run only once to set up the necessary
  // procedures
  void setUp()
  {
    // Setup led pin used to blink
    pinMode(ledPin, OUTPUT);
    
    // Set up the serial output
    mySerial->begin(9600); // 9600 is the Baudrate
    
    // Set the active state to false
    active = false;
    
    // Set the default values of the PID controller
    error = P = I = D = PID_value = previous_error = previous_I = 0;
  }
  
  // This function is used to start this manager class
  void start()
  {
    // Set the active state to true
    active = true;
    
    // Check to see if any of the required class is null. If yes then
    // raise an error
    if(line == NULL || vehicle == NULL || objDetection == NULL)
      raiseError("The needed class to intialize the manager class is/are missing");
    
    // Eveything looks good start managing the class
    while(active)
	{	
		currentAction = constrain(currentAction, 0, totalActions - 1);
		
		// Update the new error value based on the direction given by the IR
		// sensors
		error = line->getDirections();
		
		// Check if an object has been detected
		// If the object distance is less than 10 cm then stop
		/*if(objDetection->microsecondsToCentimeters(objDetection->objectDetected()) < 10)
		{
			vehicle->stop();
			
			// Check the vehicle should pick up the object
			switch(myActions[currentAction])
			{
			case A_PICK_OBJECT:
				sendPickObjectMessage();
				break;
			
			case A_DROP_OBJECT:
				sendDropObjectMessage();	
				break;
			
			default:
				// If the vehicle is lost start back tracking
				if(frontDirections == STOP && backDirections == STOP)
					backTrack();
				break;
			}
		}*/
		
		// Make a decision based on the error given
		if(error == LEFT)
		{
			// Check if it the vehicle suppose to make a left turn
			// else just ignore the line
			if(myActions[currentAction] == A_LEFT)
			{
				// Make the left turn
				do
				{
					// Update the error value
					error = line->getDirections();
					vehicle->turnLeft();
				} while(error != 0);
				
				// Push the action into the stack
				
				
				// Update the new currentAction
				updateActionIndex(currentAction++);
			}
		}
		else if(error == RIGHT)
		{
			// Check if the vehicle is supposed to make a right turn
			if(myActions[currentAction] == A_RIGHT)
			{
				// Make the right turn
				do
				{
					error = line->getDirections();
					vehicle->turnRight();
				} while(error != 0);
				
				// Update the new currentAction
				updateActionIndex(currentAction++);
			} 
		}
		else if(error == STOP)
		{
			// Check if the vehicle should stop or make a uturn
			if(myActions[currentAction] == A_UTURN)
			{
				// Make the right turn
				do
				{
					error = line->getDirections();
					vehicle->turnRight();
				} while(error != 0);
				
				// Update the new currentAction
				updateActionIndex(currentAction++);
			}
			else
			{
				// Stop the vehicle
				vehicle->stop();
			}
		}
		else if(error == CROSSROAD)
		{
			if(myActions[currentAction] == A_LEFT)
			{
				vehicle->turnLeft(); // Make the left turn
				delay(100);
				
				do
				{
					error = line->getDirections();
					vehicle->turnLeft();
				} while(error != 0);
				
				// Update the new currentAction
				updateActionIndex(currentAction++);
			}
			else if(myActions[currentAction] == A_RIGHT)
			{
				vehicle->turnRight(); // Make the left turn
				delay(100);
				
				do
				{
					error = line->getDirections();
					vehicle->turnRight();
				} while(error != 0);
				
				// Update the new currentAction
				updateActionIndex(currentAction++);	
			}
			else if(myActions[currentAction] == A_STRAIGHT)
			{
				vehicle->forward();
				
				// Update the new currentAction
				updateActionIndex(currentAction++);	
			}
			else
			{
				calculate_pid();
    			motor_control();
			}
		}
	}
  }
  
  // This function is used to stop this manager class.
  void stop()
  {
    // Set the active state to false
    active = false;
  }
  
  // This functions is used to raise irrecoverable errors
  void raiseError(const char *errMsg)
  {
    // Print the error message first before entering into a loop
    Serial.println("------------------------------------------");
    Serial.println(errMsg);
    Serial.println("------------------------------------------"); 
    
    // Send a stop signal to every arduino manager class
    stop();
    
    // Loop until hardware reset
    while(true)
    {
      digitalWrite(ledPin, HIGH);
      delay(250); // 250 ms
      
      digitalWrite(ledPin, LOW);
      delay(250); // 250 ms
    }
  }
  
  // This function is used to send a message to other board
  // it assumes that the other board will recieve the message
  // just like UDP
  void sendMsg(const char *message) const
  {
    // Set up the serial
    mySerial->begin(9600);
    
    // Send a message
    mySerial->print(message);
  }
  
  // This function pings to see if the other arduino board is active
  bool ping()
  {
    // Make sure tha the serial has been set
    setUpMySerial();
    
    // Set up the serial
    mySerial->begin(9600);
    
    // Send a message
    mySerial->print("Hello");
   
    // Wait for a reply with a maximum of 2 retries
    if(listen() == "-1")
      return false;
    else
      return true;
  }
  
  // This function enters into a loop and listens for a message from
  // the other arduino board
  String listen(unsigned int retries = 3)
  {
    int retry = 0; // Holds the number of time the ping request was sent
    
    String data = mySerial->readString();
    
    // Flush the buffer
    flush(); 
    
    while(data == "")
    { 
      // Increase number of retries
      retry++;
      
      // Check if max retry length has been reached
      if(retry >= retries)
        return "-1";
      
      // Retry to read again
      data = mySerial->readString();
      
      // Flush the buffer
      flush();
    }
    
    // Send an acknowledgement if it is a ping request
    if(data == "Hello")
      mySerial->print("ACK");
    
    // Flush the buffer
   flush(); 
      
    // Data has been recieved return it
    return data;
  }
  
  // This function is used to send a message to pick up ana object
  void sendPickObjectMessage()
  {
  	// Send a message to the other arduino to start picking the object
	sendMsg("PickObject");
	
	// Wait till the picking has been completed. Listen for the complete
	// command
	for(int i = 0; i < 100; ++i)
	{
		// Wait for a reply from the other arduino timeout after 5 seconds
		if(listen() != "-1")
			break;
		
		// Delay for 50 ms
		delay(50);	
	}
  }
  
  // Used to send a drop object command
  void sendDropObjectMessage()
  {
  	// Send a message to the other arduino to start picking the object
	sendMsg("DropObject");
	
	// Wait till the picking has been completed. Listen for the complete
	// command
	for(int i = 0; i < 100; ++i)
	{
		// Wait for a reply from the other arduino timeout after 5 seconds
		if(listen() != "-1")
			break;
		
		// Delay for 50 ms
		delay(50);	
	}
  }
  
  // This function is used to back track
  // Not a perfect function
  void backTrack()
  {
  	  // Stop the vehicle first
	  vehicle->stop();
  	
	  int tempAction = actions.pop();
	 
	 do
	 {
	 	error = line->getDirections();
	 	
		  switch(tempAction)
		  {
			case RIGHT:
				vehicle->turnLeft();
	 		break;
			
			case LEFT:
				vehicle->turnLeft();
			break;
			
			default:
				// Just reverse and hope for the best
				vehicle->reverse();
			 break;
	      }	
	      
	 } while(error != 0);
	 
	 updateActionIndex(currentAction--);
  }
private:
  Vehicle *vehicle; // Pointer to vehicle object
  LineTrack *line; // Pointer to LineTrack object
  ObjectDetection *objDetection; // Pointer to 'objectDetection' object
  static const int ledPin = 13; // LedPin that will blink, 13 by default
  bool active; // Indicates whether this class is active
  int serialUsed;
  
  // Create a pointer to serial class
  HardwareSerial *mySerial;
  
  // PID Constants
  // Used to calculate the PID constants
  const float Kp = 25;
  const float Ki = 0;
  const float Kd = 15;
  float error, P, I, D, PID_value;
  float previous_error, previous_I;
  
  // Utility functions
  // This function is used to keep track of actions
  // to enable back tracking
  void addAction(int newAction)
  {
  	  if(actions.isFull())
	  	  actions.shift(); // Remove the first element
  
  	  // Push the action into the action stack
  	  actions.push(newAction);
  }
  
  // This function is used to calculate the PID
  void calculate_pid()
  {
    P = error;
    I = I + previous_I;
    D = error - previous_error;

    PID_value = (Kp * P) + (Ki * I) + (Kd * D);

    previous_I = I;
    previous_error = error;
  }
  
  // Used to control motor speed based on the PID value 
  void motor_control()
  {
	  // Calculating the effective motor speed:
	  int left_motor_speed = vehicle->getMaxSpeed() - PID_value;
	  int right_motor_speed = vehicle->getMaxSpeed() + PID_value;
	
	  // The motor speed should not exceed the max PWM value
	  left_motor_speed = constrain(left_motor_speed, 0, 255);
	  right_motor_speed = constrain(right_motor_speed, 0, 255);
	  
	  // Write the new speed to the vehicle motor
	  int motors[2] = {0, 1};
	  
	  // Check the mode the car is in
	  if(vehicle->getMode() == REVERSE)
	  {
		motors[0] = 2;
		motors[1] = 3;					
	  }
	  
	  // Left Motor Speed
	  vehicle->setIndivdualWheel(vehicle->getWheels()[motors[0]], left_motor_speed);
	  
	  // Right Motor Speed
	  vehicle->setIndivdualWheel(vehicle->getWheels()[motors[1]], right_motor_speed - 30);
  }

  // This function is used to flush the serial buffer
  void flush()
  {
    mySerial->flush();
  }
  
  // This function sets up my serial class
  void setUpMySerial()
  {
    if(String(BOARD) == "Mega" || String(BOARD) == "Mega 2560")
    {
      // Select the correct serial to use
      switch(serialUsed)
      {
        case SERIAL_0:
          mySerial = &Serial;
          break;
        
        case SERIAL_1:
          mySerial = &Serial1;
          break;
        
        case SERIAL_2:
          mySerial = &Serial2;
          break;
        
        case SERIAL_3:
          mySerial = &Serial3;
          break;
        
        default:
          mySerial = &Serial; // Default serial class
          break;
      }
    }
    else
    {
      mySerial = &Serial; // Default serial class
    }
    
    // Set up the serial pointer
    mySerial->begin(9600);
  }
}; // End class manager
#endif