/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 26.07.2019 14:00:52
 Libraries   :
 Author      :Grimm
 Description :This header file is used to control the ultrasound to detect 
 	 	 	 objects
******************************************************************/

// Prevent multiple inclusions
#ifndef OBJECT_DETECTION_H
#define OBJECT_DETECTION_H

// Structor for the ultrasound sensor
struct UltraSoundSensor
{
	unsigned int triggerPin;
	unsigned int echoPin;
};

class ObjectDetection
{
public:
	// Default constructor
	ObjectDetection()
	{
		ultraSensor.triggerPin = 40;
		ultraSensor.echoPin = 41;
		
		// Set up the class
		setUp();	
	}
	
	// Parameterized constructor
	ObjectDetection(const UltraSoundSensor &uss)
	{
		ultraSensor.triggerPin = uss.triggerPin;
		ultraSensor.echoPin = uss.echoPin;
		
		// Set up the class
		setUp();	
	}
	
	ObjectDetection(const unsigned int triggerPin, const unsigned int echoPin)
	{
		ultraSensor.triggerPin = triggerPin;
		ultraSensor.echoPin = echoPin;
		
		// Set up the class
		setUp();
	}
	
	// Copy constructor
	ObjectDetection(const ObjectDetection &other)
	{
		ultraSensor.triggerPin = other.ultraSensor.triggerPin;
		ultraSensor.echoPin = other.ultraSensor.echoPin;
		
		// Set up the class
		setUp();
	}
	
	// Sets the sensor
	void setSensor(const UltraSoundSensor &uss)
	{
		ultraSensor.triggerPin = uss.triggerPin;
		ultraSensor.echoPin = uss.echoPin;
		
		// Set up the class
		setUp();
	}
	
	// Gets the sensor
	UltraSoundSensor getSensor() const
	{
		return ultraSensor;
	}
	
	// Sets up this class
	void setUp()
	{
		// Set up the ultra sound sensor
		pinMode(ultraSensor.triggerPin, OUTPUT);
		pinMode(ultraSensor.echoPin, INPUT);
	}
	
	// This function detects and returns -1 if an object is not detected
	// else it returns assumed distance of the object
	long objectDetected()
	{
		// Send two pings
		for(int i = 0; i < 2; ++i)
		{
			ping();
			
			// Delay for 100 ms
			delay(100);	
		}
		
		return listen();
	}
	
	// Used to convert the pulse to inches
	long microsecondsToInches(long microseconds)
	{
	  // According to Parallax's datasheet for the PING))), there are 73.746
	  // microseconds per inch (i.e. sound travels at 1130 feet per second).
	  // This gives the distance travelled by the ping, outbound and return,
	  // so we divide by 2 to get the distance of the obstacle.
	  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
	  return microseconds / 74 / 2;
	}

	// Used to convert the pulse to centimeters
	long microsecondsToCentimeters(long microseconds) 
	{
	  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
	  // The ping travels out and back, so to find the distance of the object we
	  // take half of the distance travelled.
	  return microseconds / 29 / 2;
	}
private:
	UltraSoundSensor ultraSensor;
	
	// Utility functions
	// This functions sends a ping
	void ping()
	{
		pinMode(ultraSensor.triggerPin, OUTPUT);
		digitalWrite(ultraSensor.triggerPin, LOW);
		delayMicroseconds(2);
		digitalWrite(ultraSensor.triggerPin, HIGH);
		delayMicroseconds(5);
		digitalWrite(ultraSensor.triggerPin, LOW);
	}
	
	// This function recieves a ping
	long listen()
	{
		long duration = pulseIn(ultraSensor.echoPin, HIGH);
		
		return duration;
	}
	
}; // End class ObjectDetection

#endif