/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 26.07.2019 14:00:52
 Libraries   :
 Author      :Grimm
 Description :This class will be used to track the line for the robot
              based on IR (infra-red sensors)
******************************************************************/

// Prevent multiple inclusions
#ifndef LINE_TRACK_H
#define LINE_TRACK_H

// Include libraries
#include "Globals.h" // Contains global variables
#include "Vehicle.h" // Contains vehicle definitions

// Create a struct for front sensors
struct FrontSensors
{
  unsigned int pin1;
  unsigned int pin2;
  unsigned int pin3;
};

// Struct for the back sensors
struct BackSensors
{
  unsigned int pin1;
  unsigned int pin2;
  unsigned int pin3;
  unsigned int pin4;
};

// Struct to keep track of actions performed by the class
// will be used to print errors
struct Actions {
  char actionPerfomed[25];
};

// Create an enumeration for selecting the field configuration
enum Field {
  BLACK_SURFACE_WHITE_LINE = 0,
  WHITE_SURFACE_BLACK_LINE
};

// Enumeration for directions to take
enum directions {
  STRAIGHT = 0,
  LEFT = 100,
  SOFT_LEFT = 1,
  RIGHT = 101,
  SOFT_RIGHT = -1,
  STOP = 102,
  STOP_ERROR = 1000
};

// Enumeration that holds the line type
enum lineType {
  STRAIGHT_ROAD = 0,
  LEFT_TURN = 100,
  RIGHT_TURN = 101,
  CROSSROAD = 103,
  NO_ROAD = 102,
};

class LineTrack
{
public:  
  // Default constructor
  LineTrack()
  {
    // Default front sensors
    frontSensorPins[0] = 2;
    frontSensorPins[1] = 3;
    frontSensorPins[2] = 4;
    
    // Default back sensors
    backSensorPins[0] = 5;
    backSensorPins[1] = 6;
    backSensorPins[2] = 7;
    backSensorPins[3] = 8;
    
    // Setup the other default settings
    defaultSetting();
    
    // Set the vehicle instance to a default of NULL
    vehicleInstance = NULL;
    
    // Setup the pins
    setUp();
  }
  
  // Parameterized constructor
  LineTrack(const FrontSensors &front, const BackSensors &back)
  {
    // Write the pins and the setup the pins
    // Front sensors
    frontSensorPins[0] = front.pin1;
    frontSensorPins[1] = front.pin2;
    frontSensorPins[2] = front.pin3;
    
    // Back sensors
    backSensorPins[0] = back.pin1;
    backSensorPins[1] = back.pin2;
    backSensorPins[2] = back.pin3;
    backSensorPins[3] = back.pin4;
    
    // Setup the other default settings
    defaultSetting();
    
    // Run the setup
    setUp();
  }
  
  // Copy constructor
  LineTrack(const LineTrack &other)
  {    
    // Loop 3 times to assign the front sensor pins
    for(int i = 0; i < 3; ++i)
      frontSensorPins[i] = other.frontSensorPins[i];
      
    // Loop 4 times to assign the back sensor pins
    for(int i = 0; i < 3; ++i)
      backSensorPins[i] = other.backSensorPins[i];
      
    // Setup the surface to be used
    setFieldConfiguration(other.fieldType);
    
    // Copy the other object vehicle instance
    vehicleInstance = other.vehicleInstance;
    
    // Setup the pins
    setUp();
  }
  
  // Accessor functions here
  // Sets the front sensor pins
  void setFrontSensorPins(unsigned int pin1, unsigned int pin2, unsigned int pin3)
  {
    frontSensorPins[0] = pin1;
    frontSensorPins[1] = pin2;
    frontSensorPins[2] = pin3;
    
    // Run the setup
    setUp();
  }
  
  // Sets the front sensor pins
  void setFrontSensorPins(const FrontSensors &front)
  {
    frontSensorPins[0] = front.pin1;
    frontSensorPins[1] = front.pin2;
    frontSensorPins[2] = front.pin3;
    
    // Run the setup
    setUp();
  }
  
  // Gets the front sensor pins as an array
  const unsigned int* getFrontSensorPins() const
  {
    return frontSensorPins;
  }
  
  // Sets the back sensor pins
  void setBackSensorPins(unsigned int pin1, unsigned int pin2, unsigned int pin3, unsigned int pin4)
  {
    backSensorPins[0] = pin1;
    backSensorPins[1] = pin2;
    backSensorPins[2] = pin3;
    backSensorPins[3] = pin4;
    
    // Run the setup
    setUp();
  }
  
  // Sets the back sensor pins
  void setBackSensorPins(const BackSensors &back)
  {
    backSensorPins[0] = back.pin1;
    backSensorPins[1] = back.pin2;
    backSensorPins[2] = back.pin3;
    backSensorPins[3] = back.pin4;
    
    // Run the setup
    setUp();
  }
  
  // Get the back sensor pins as an array
  const unsigned int *getBackSensorPins() const
  {
    return backSensorPins;
  }
  
  // Set the field configuration
  void setFieldConfiguration(int config)
  {
    switch(config)
    {
    case WHITE_SURFACE_BLACK_LINE:
    case BLACK_SURFACE_WHITE_LINE:
        fieldType = config;
        changeFieldType();
        break;
    
    default:
       // Do not change the config since it is unknown
      break;
    }
  }
  
  // Get the filed configuration
  int getFieldConfiguration() const
  {
    return fieldType;
  }
  
  // This is used to set the vehicleInstance
  void setVehicle(Vehicle &vehicle)
  {
    vehicleInstance = &vehicle;
  }
  
  // Other functions here
  void setUp()
  {
    // Setup the pins here
    // Front sensors
    for(int i = 0; i < 3; ++i)
    {
      pinMode(frontSensorPins[i], INPUT);
    }
    
    // Back sensors
    for(int i = 0; i < 4; ++i)
    {
      pinMode(backSensorPins[i], INPUT);
    }
    
    // Set up the serial
    Serial.begin(9600);

    // Set up the surface type
    changeFieldType();
    
    // Setup the blinking LED
    pinMode(ledPin, OUTPUT);
  }
  
  // Will print to the serial 
  void print(unsigned int _delay = 3000) const
  {
    // Three sensors
    Serial.println("3 front sensors");
    Serial.println(digitalRead(frontSensorPins[0]));
    Serial.println(digitalRead(frontSensorPins[1]));
    Serial.println(digitalRead(frontSensorPins[2]));
    Serial.println("-------------------------------");

    // Four sensors
    Serial.println("4 back sensors");
    Serial.println(digitalRead(backSensorPins[0]));
    Serial.println(digitalRead(backSensorPins[1]));
    Serial.println(digitalRead(backSensorPins[2]));
    Serial.println(digitalRead(backSensorPins[3]));
    Serial.println("-------------------------------");

    // Delay for n seconds. 3000 ms (3 Seconds) by default
    delay(_delay);
  }
  
  // This function is used to get the direction the vehicle should take
  int *getDirections(char a)
  {    
    // Get the front sensors direction decision
    fillFrontSensorDirection();
    
    // Get the back sensors direction decision
    fillBackSensorDirection();
    
    // Record the final decision as a major action which will be pushed into the
    // stack
    // Check if it is a stop error
    if(frontSensorDecisions[1] == STOP_ERROR || backSensorDecisions[1] == STOP_ERROR)
      raiseError("ERROR: The road type could not be detected");
    
    // Set the directions that was encountered
    frontDirections = frontSensorDecisions[0];
    frontLineType = frontSensorDecisions[1];
    
    // Set the back directions so that the manager can know what to do
    backDirections = backSensorDecisions[0];
    backLineType = backSensorDecisions[1];
    
    // Check the direction the vehicle is going
    // Prioritize the sensor that the vehicle is going
    if(vehicleInstance->getMode() == FORWARD)
    	return frontSensorDecisions;
    else
    	return backSensorDecisions; // The car is going backwards
  }
  
  // This function get the larger error value from getDirection
  int getDirections()
  {
	  int *temp = getDirections('a');
	  
	  if(temp[0] >= temp[1])
	  	return temp[0];
	  else
	  	return temp[1];
  }
  
private:
  unsigned int frontSensorPins[3]; // Pins used for the front sensor
  int frontSensorDecisions[2]; // Holds the decision made by the front sensor
  unsigned int backSensorPins[4]; // Pins used for back sensor
  int backSensorDecisions[2]; // Holds the decision made by the back sensor
  unsigned int fieldType; // Default field surface
  static const int ledPin = 13; // Blinking LED incase of error
  Vehicle *vehicleInstance; // Pointer to the vehicle class
  
  // The variables below refer to the input by the IR sensors when
  // the sensors are over the black line
  bool WHITE;
  bool BLACK;
  
  // A struct object to keep track of actions performed this will be pushed into
  // the stack
  Actions myActions;
  
  // Utility function to change configuration of the field
  void changeFieldType()
  {
    if(fieldType == BLACK_SURFACE_WHITE_LINE)
    {
      WHITE = LOW;
      BLACK = HIGH;
    }
    else // WHITE_SURFACE_BLACK_LINE
    {
      WHITE = HIGH;
      BLACK = LOW;
    }
  }
  
  // This function resets the class to default
  void defaultSetting()
  {
    // Set the value that indicates that the sensor is over a black material
    // or if it is over a white material
    WHITE = LOW;
    BLACK = HIGH;
  
    // Setup the surface to be used
    setFieldConfiguration(BLACK_SURFACE_WHITE_LINE);
  }
  
  // This function just prints the debug for directions
  void printDirectionsDebug(unsigned int delay_ = 3000) const
  {
    // Just print this to debug it
    Serial.println("******************************************");
    Serial.println("Front sensors: ");
    Serial.println("Roadtype: ");
    Serial.println(frontSensorDecisions[0]);
    Serial.println();
    Serial.println("Direction: ");
    Serial.println(frontSensorDecisions[1]);
    Serial.println();
    
    // Back sensors
    Serial.println("Back sensors: ");
    Serial.println("Roadtype: ");
    Serial.println(backSensorDecisions[0]);
    Serial.println();
    Serial.println("Direction: ");
    Serial.println(backSensorDecisions[1]);
    Serial.println();
    Serial.println("******************************************");
    delay(delay_);
  }
  
  // This functions fills in the front sensor decisions into
  // frontSensorDecisions array
  void fillFrontSensorDirection()
  {
    // Read the values of three front sensors
    // Normal values should read 0 1 0
    int frontSensorValue[3] = {
      digitalRead(frontSensorPins[0]),
      digitalRead(frontSensorPins[1]),
      digitalRead(frontSensorPins[2])
    };
    
     // Holds the decision based on the front sensor inputs
    int frontSensorDecision = 0;
    int frontSensorRoadEncountered = 0;
    
    // Deal with the front sensors first
    if(frontSensorValue[0] == BLACK &&
      frontSensorValue[1] == BLACK &&
      frontSensorValue[2] == BLACK)
    { // 0 0 0
      frontSensorRoadEncountered = NO_ROAD;
      frontSensorDecision =  STOP;  
    }
    else if(frontSensorValue[0] == BLACK &&
     frontSensorValue[1] == BLACK && 
     frontSensorValue[2] == WHITE)
    {// 0 0 1
      // Record that it has passed a left line
      frontSensorRoadEncountered = STRAIGHT_ROAD;
      frontSensorDecision = RIGHT;
    }
    else if(frontSensorValue[0] == BLACK && 
    frontSensorValue[1] == WHITE && 
    frontSensorValue[2] == BLACK)
    {// 0 1 0
      // This is the normal. Record straight pass
      frontSensorRoadEncountered = STRAIGHT_ROAD;
      frontSensorDecision = STRAIGHT;
    }
    else if(frontSensorValue[0] == BLACK && 
    frontSensorValue[1] == WHITE && 
    frontSensorValue[2] == WHITE)
    {// 0 1 1
      // Record that it has taken a hard right
      frontSensorRoadEncountered = RIGHT_TURN;
      frontSensorDecision = RIGHT;
    }
    else if(frontSensorValue[0] == WHITE && 
    frontSensorValue[1] == BLACK && 
    frontSensorValue[2] == BLACK)
    {// 1 0 0
      // Record a left turn
      frontSensorRoadEncountered = STRAIGHT_ROAD;
      frontSensorDecision = SOFT_LEFT;
    }
    else if(frontSensorValue[0] == WHITE && 
    frontSensorValue[1] == BLACK && 
    frontSensorValue[2] == WHITE)
    { // 1 0 1
      // Randomly choose a direction
      int rand = random(0, 2);
      
      // Do not do anything major
      frontSensorRoadEncountered = STRAIGHT_ROAD;
      
      if(rand == 0) // Prefer left
        frontSensorDecision = SOFT_LEFT;  
      else // Prefer right    
        frontSensorDecision = SOFT_RIGHT;
    }
    else if(frontSensorValue[0] == WHITE && 
    frontSensorValue[1] == WHITE && 
    frontSensorValue[2] == BLACK)
    { // 1 1 0
      // Record that it has taken a hard left
      frontSensorRoadEncountered = LEFT_TURN;
      frontSensorDecision = LEFT;
    }
    else if(frontSensorValue[0] == WHITE && 
    frontSensorValue[1] == WHITE && 
    frontSensorValue[2] == WHITE)
    { // 1 1 1
      // Record that it has met a crossroad
      frontSensorRoadEncountered = CROSSROAD;
      frontSensorDecision = STRAIGHT;
    }
    else
    {
      // When an unknown input is recieved raise the error
      frontSensorRoadEncountered = NO_ROAD;
      frontSensorDecision = STOP_ERROR; // Will induce blinking
    }
    
    // Load the decision into the decision array
    frontSensorDecisions[0] = frontSensorRoadEncountered;
    frontSensorDecisions[1] = frontSensorDecision;
  }
  
  // This functions fills in the back sensor decisions into
  // backSensorDecisions array
  void fillBackSensorDirection()
  {
    // Read the values of the four back sensors
    // Normal values should read 0 1 1 0
    int backSensorValues[4] = {
      digitalRead(backSensorPins[0]),
      digitalRead(backSensorPins[1]),
      digitalRead(backSensorPins[2]),
      digitalRead(backSensorPins[3])
    };
    
    // Posibilities for this functions are 2^4
    int backSensorDecision;
    int backSensorRoadEncountered;
    
    if(backSensorValues[0] == BLACK && 
    backSensorValues[1] == BLACK && 
    backSensorValues[2] == BLACK && 
    backSensorValues[3] == BLACK)
    {// 0 0 0 0
      backSensorRoadEncountered = NO_ROAD;
      backSensorDecision =  STOP;  
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == WHITE)
    {// 0 0 0 1
      // Sensor is a right line return decision
      backSensorRoadEncountered = RIGHT_TURN;
      backSensorDecision =  RIGHT;  
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == BLACK)
    { // 0 0 1 0
      //Make a soft right turn
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  SOFT_RIGHT;   
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == WHITE)
    { // 0 0 1 1
      backSensorRoadEncountered = RIGHT_TURN;
      backSensorDecision =  RIGHT;
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == BLACK)
    { // 0 1 0 0
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  SOFT_LEFT;
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == WHITE)
    { // 0 1 0 1
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  SOFT_LEFT;
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == BLACK)
    { // 0 1 1 0
      // Normal
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  STRAIGHT;
    }
    else if(backSensorValues[0] == BLACK && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == WHITE)
    { // 0 1 1 1
      backSensorRoadEncountered = LEFT_TURN;
      backSensorDecision =  LEFT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == BLACK)
    { // 1 0 0 0
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  RIGHT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == WHITE)
    { // 1 0 0 1
        // Randomly choose a direction
        int rand = random(0, 2);
        
        // Do not do anything major
        backSensorRoadEncountered = STRAIGHT_ROAD;
        
        if(rand == 0) // Prefer left
          backSensorDecision = SOFT_LEFT;  
        else // Prefer right    
          backSensorDecision = SOFT_RIGHT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == BLACK)
    { // 1 0 1 0
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  SOFT_RIGHT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == BLACK && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == WHITE)
    { // 1 0 1 1
      backSensorRoadEncountered = RIGHT_TURN;
      backSensorDecision =  SOFT_RIGHT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == BLACK)
    { // 1 1 0 0
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  SOFT_LEFT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == BLACK && 
      backSensorValues[3] == WHITE)
    { // 1 1 0 1
      backSensorRoadEncountered = STRAIGHT_ROAD;
      backSensorDecision =  SOFT_LEFT;  
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == BLACK)
    { // 1 1 1 0
      backSensorRoadEncountered = LEFT_TURN;
      backSensorDecision =  LEFT;
    }
    else if(backSensorValues[0] == WHITE && 
      backSensorValues[1] == WHITE && 
      backSensorValues[2] == WHITE && 
      backSensorValues[3] == WHITE)
    { // 1 1 1 1
      backSensorRoadEncountered = CROSSROAD;
      backSensorDecision =  STRAIGHT;
    }
    else
    {
      // Return that an error was encountered on the back sensor,
      // this will induce blinking
      backSensorRoadEncountered = NO_ROAD;
      backSensorDecision =  STOP_ERROR;
    }
    
    // Record final decision and return it as an array
    backSensorDecisions[0] = backSensorRoadEncountered;
    backSensorDecisions[1] = backSensorDecision;
  }
  
  // This will blink incase an error occurs and outputs the error to the screen
  void raiseError(const char *errMsg) const
  {
    // Set the delayTime for the blinking LED
    int delayTime = 20;
    
    // Output the error to serial monitor
    Serial.println("--------------------------------------------");
    Serial.println("Error");
    Serial.println(errMsg);
    Serial.println("--------------------------------------------");
    
    // Blink incase of error until hardware reset
    while(true)
    {
      // LED ON
      digitalWrite(ledPin, HIGH);
      delay(delayTime);
      
      // LED OFF
      digitalWrite(ledPin, LOW);
      delay(delayTime);
      
      // Calculate the new delayTime
      delayTime *= 2;
      
      // Reset delay time if it is greater than 320
      if(delayTime > 320)
        delayTime = 20;
    } // End while loop
  }
}; // End class line track
#endif
