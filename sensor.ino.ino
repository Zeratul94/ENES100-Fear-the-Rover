int trigPin = 33;    // Trigger
int echoPin = 32;
int trigPin2 = 35;
int echoPin2 = 34;    // Echo
long duration, cm, inches, duration2, inches2, cm2;
 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}
 
void loop() { // The sensor is triggered by a HIGH pulse of 10 or more microseconds 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);
  
  delay(500);
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  cm2 = (duration2/2) / 29.1;
  inches2 = (duration2/2) / 74;
  
  Serial.print("USS1: ");
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(500);
  
  Serial.print("USS2: ");
  Serial.print(inches2);
  Serial.print("in, ");
  Serial.print(cm2);
  Serial.print("cm");
  Serial.println();
  delay(500); 

  if (cm == 805 || inches == 316 || cm2 == 805 || inches2 == 316) {
    Serial.print("Foam!");
  }
  else if (cm <= 30 || inches <= 10 || cm2 <= 30 || inches2 <= 10) {
    Serial.print("Plastic!");
  }
  delay(500);
  
}


