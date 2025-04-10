#include <Servo.h>  // Include the Servo library to control servo motors

// Declare Servo objects for horizontal and vertical control
Servo horizontal;
int servohori = 180;  // Initial horizontal servo position
int servohoriLimitHigh = 160;  // Upper limit for horizontal servo
int servohoriLimitLow = 20;    // Lower limit for horizontal servo

Servo vertical;
int servovert = 45;   // Initial vertical servo position
int servovertLimitHigh = 150;  // Upper limit for vertical servo
int servovertLimitLow = 20;    // Lower limit for vertical servo

// Define analog pins connected to LDRs (Light Dependent Resistors)
int ldrlt = A1; // Top-left LDR
int ldrrt = A0; // Top-right LDR
int ldrld = A2; // Bottom-left LDR
int ldrrd = A3; // Bottom-right LDR

void setup() {
  Serial.begin(9600);          // Initialize serial communication at 9600 bps
  horizontal.attach(10);       // Attach horizontal servo to digital pin 10
  vertical.attach(11);         // Attach vertical servo to digital pin 11

  // Set initial positions for both servos
  horizontal.write(90);        // Center horizontal servo
  vertical.write(45);          // Set vertical servo to a slightly downward position
  delay(2500);                 // Wait for servos to reach position
}

void loop() {
  // Read analog values from all four LDRs
  int lt = analogRead(ldrlt); // Top-left
  int rt = analogRead(ldrrt); // Top-right
  int ld = analogRead(ldrld); // Bottom-left
  int rd = analogRead(ldrrd); // Bottom-right

  int dtime = 20;  // Delay between each adjustment (ms)
  int tol = 90;    // Tolerance threshold for light difference

  // Calculate average light intensity for top, bottom, left, and right
  int avt = (lt + rt) / 2; // Average top
  int avd = (ld + rd) / 2; // Average bottom
  int avl = (lt + ld) / 2; // Average left
  int avr = (rt + rd) / 2; // Average right

  // Determine difference between top-bottom and left-right
  int dvert = avt - avd;   // Vertical light difference
  int dhoriz = avl - avr;  // Horizontal light difference

  // Adjust vertical servo if vertical difference exceeds tolerance
  if (abs(dvert) > tol) { 
    if (avt > avd) {
      servovert++;
      if (servovert > servovertLimitHigh) servovert = servovertLimitHigh;
    } else {
      servovert--;
      if (servovert < servovertLimitLow) servovert = servovertLimitLow;
    }
    vertical.write(servovert);  // Move vertical servo
  }

  // Adjust horizontal servo if horizontal difference exceeds tolerance
  if (abs(dhoriz) > tol) { 
    if (avl > avr) {
      servohori++;
      if (servohori > servohoriLimitHigh) servohori = servohoriLimitHigh;
    } else {
      servohori--;
      if (servohori < servohoriLimitLow) servohori = servohoriLimitLow;
    }
    horizontal.write(servohori);  // Move horizontal servo
  }

  delay(dtime);  // Small delay before next loop iteration
}

// Optional function to calibrate servo movement across full range
void calibrateServos(int delayTime) {
  Serial.println("Calibrating Servos...");

  // Move horizontal servo from low to high and back
  for (int pos = servohoriLimitLow; pos <= servohoriLimitHigh; pos++) {
    horizontal.write(pos);
    delay(delayTime);
  }
  for (int pos = servohoriLimitHigh; pos >= servohoriLimitLow; pos--) {
    horizontal.write(pos);
    delay(delayTime);
  }

  // Move vertical servo from low to high and back
  for (int pos = servovertLimitLow; pos <= servovertLimitHigh; pos++) {
    vertical.write(pos);
    delay(delayTime);
  }
  for (int pos = servovertLimitHigh; pos >= servovertLimitLow; pos--) {
    vertical.write(pos);
    delay(delayTime);
  }

  // Return to initial positions
  horizontal.write(180);
  vertical.write(45);
  Serial.println("Calibration Complete!");
}
