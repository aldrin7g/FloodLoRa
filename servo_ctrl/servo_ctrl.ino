#include <Servo.h>

// Pin connected to digital sensor
const int sensorPin1 = A0;
const int sensorPin2 = A1;
// Pin connected to servo
const int servoPin = 9;

Servo myServo;

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // Initialize the digital pin as an input.
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  // Attach servo to the servo pin
  myServo.attach(servoPin);
}

void loop() {
  // Read the value from the sensor:
  int sensorValue1 = digitalRead(sensorPin1);
  int sensorValue2 = digitalRead(sensorPin2);

  // Print the sensor value and servo status to the serial monitor:
  Serial.print("Sensor Value1: ");
  Serial.print(sensorValue1);
  Serial.print("Sensor Value2: ");
  Serial.print(sensorValue2);
  Serial.print(" - Servo Status: ");

  // Check the sensor value:
  if (sensorValue1 == HIGH) {
    // If sensor reads '1', turn servo to 90 degrees and print status:
    myServo.write(90);
    Serial.println("90 degrees");
  } else if (sensorValue2 == HIGH) {
    // If sensor reads '1', turn servo to 90 degrees and print status:
    myServo.write(180);
    Serial.println("180 degrees");
  } else {
    // If sensor reads '0', turn servo to 0 degrees and print status:
    myServo.write(0);
    Serial.println("0 degrees");
  }

  // Delay for better readability (optional)
  //delay(500);
}
