#include <AFMotor.h>

int lastReading = 0;
/* Readings below this mean that the robot should react*/
int reactionCeiling = 10;

/* Ceiling aimed sonic sensor pins */
int skySonicTrig = 3, skySonicGnd = 4, skySonic5V = 5;

/* Front mounted sonic sensor pins */
int frontSonicTrig = A5;

/* If this positive, power the motor forwards and decrement. If negative, backwards*/
int motorBuffer = 0;
int bufferChangeDelayMs = 1000;

/* Sonic sensor pins */
int sonicTrig = 2, sonicGnd = 3, sonic5V = 4;

AF_DCMotor* wheelL, *wheelR;

void setup() {
  pinMode(skySonic5V, OUTPUT);
  pinMode(skySonicGnd, OUTPUT);
  pinMode(skySonicTrig, INPUT);
  
  wheelL = new AF_DCMotor(2, MOTOR12_1KHZ);
  
  wheelL->setSpeed(255);
 
}

int timeToCm(int t) {
  //http://www.crcibernetica.com/ultrasonic-range-finder-sdm-io/
  return (t*340.23/2/10000)-3;
}

/* Returns distance of remote obstacle in cm */
int echolocate() {
  
  /* Power the pins */

  digitalWrite(skySonic5V, HIGH);
  digitalWrite(skySonicGnd, LOW);
  
  
  /* Do some location */
  pinMode(skySonicTrig, OUTPUT);
  digitalWrite(skySonicTrig, LOW);
  delay(25);
  digitalWrite(skySonicTrig, HIGH);
  int time = micros();
  pinMode(skySonicTrig, INPUT);
  int pulseSize = pulseIn(skySonicTrig, LOW, 18000); //expect 15us, timeout 18ms
  time = micros() - time - pulseSize; // time since sending the pulse and receiving the whole pulse
  
  return timeToCm(time);  
}

int frontEcholocate() {
    pinMode(frontSonicTrig, OUTPUT);
  digitalWrite(frontSonicTrig, LOW);
  delay(25);
  digitalWrite(frontSonicTrig, HIGH);
  int time = micros();
  pinMode(frontSonicTrig, INPUT);
  int pulseSize = pulseIn(frontSonicTrig, LOW, 18000); //expect 15us, timeout 18ms
  time = micros() - time - pulseSize; // time since sending the pulse and receiving the whole pulse
  
  return timeToCm(time);  
}

void calibrate(int numSamples) {
      int d = 0;
      for (int i = 0; i  != numSamples; ++i) {
        d += echolocate();
      }
      reactionCeiling = d / numSamples;
}
void loop() {
  int a  = echolocate();

  if (a < reactionCeiling) {

    
    motorBuffer = 1;
  }
  
    int b = frontEcholocate();
  
  boolean isRunForward = b>reactionCeiling;
  
  if (motorBuffer > 0 && !isRunForward) {
   motorBuffer = -motorBuffer;

  }
  
  if (motorBuffer > 0) {
    wheelL->run(FORWARD);
    delay(bufferChangeDelayMs);
    wheelL->run(RELEASE);
    --motorBuffer;
  }
  else if (motorBuffer < 0) {
    wheelL->run(BACKWARD);
    delay(bufferChangeDelayMs);
    wheelL->run(RELEASE);
    ++motorBuffer;
  }  
}

