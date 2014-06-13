const int photoresistor = 1;
int lastReading = 0;
/* Readings above this mean the light sensor 
 is not obscured enough*/
int darknessCeiling = 0;


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
  int trig = 2, gnd = 3, fiveV = 4;
  
  /* Power the pins */
  pinMode(fiveV, OUTPUT);
  digitalWrite(fiveV, HIGH);
  pinMode(gnd, OUTPUT);
  digitalWrite(gnd, LOW);
  
  
  /* Do some location */
  
  
  pinMode(trig , OUTPUT);
  digitalWrite(trig, LOW);
  delay(25);
  digitalWrite(trig, HIGH);
  int time = micros();
  pinMode(trig, INPUT);
  int pulseSize = pulseIn(trig, LOW, 18000); //expect 15us, timeout 18ms
  time = micros() - time - pulseSize; // time since sending the pulse and receiving the whole pulse
  return timeToCm(time);  
}

void loop() {
  /* int a = analogRead(photoresistor); */
  int a  = echolocate();
  Serial.println(a);

  if (a < darknessCeiling) {
    Serial.println("darkness ceiling under");
  }

  if (Serial.available()) {
    int c = Serial.read();
    /* Calibrate! */
    if (c == 'c') {
      int numSamples = 10;
      int d = 0;
      for (int i = 0; i  != numSamples; ++i) {
        d += analogRead(photoresistor);
      }
      darknessCeiling = d / numSamples;
    }
  }

  delay(50);
}

