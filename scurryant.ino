const int photoresistor = 1;
int lastReading = 0;
/* Readings above this mean the light sensor 
 is not obscured enough*/
int darknessCeiling = 0;


void setup() {
  pinMode(photoresistor, INPUT);
  Serial.begin(9600);
}

void loop() {
  int a = analogRead(photoresistor);
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

  delay(200);
}

