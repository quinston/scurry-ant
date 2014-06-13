const int photoresistor = 1;
int lastReading = 0;
/* Readings below this mean that the robot should react*/
int reactionCeiling = 0;

/* Sonic sensor pins */
int sonicTrig = 2, sonicGnd = 3, sonic5V = 4;

void setup() {
  pinMode(photoresistor, INPUT);
  Serial.begin(9600);
  
  
}

int timeToCm(int t) {
  //http://www.crcibernetica.com/ultrasonic-range-finder-sdm-io/
  return (t*340.23/2/10000)-3;
}

/* Returns distance of remote obstacle in cm */
int echolocate() {
  
  /* Power the pins */
  pinMode(sonic5V, OUTPUT);
  digitalWrite(sonic5V, HIGH);
  pinMode(sonicGnd, OUTPUT);
  digitalWrite(sonicGnd, LOW);
  
  
  /* Do some location */
  pinMode(sonicTrig , OUTPUT);
  digitalWrite(sonicTrig, LOW);
  delay(25);
  digitalWrite(sonicTrig, HIGH);
  int time = micros();
  pinMode(sonicTrig, INPUT);
  int pulseSize = pulseIn(sonicTrig, LOW, 18000); //expect 15us, timeout 18ms
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
  /* int a = analogRead(photoresistor); */
  int a  = echolocate();
  Serial.println(a);

  if (a < reactionCeiling) {
    Serial.println("ALERT!");
  }

  if (Serial.available()) {
    int c = Serial.read();
    /* Calibrate! */
    if (c == 'c') {
      calibrate(10);
    }
  }

  delay(50);
}

