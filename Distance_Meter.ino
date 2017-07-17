#define trigPin 3
#define echoPin 2
#define led 11
#define led2 10
#define motor_relay 5
#define ir_detect 7
#define rpm_detect 8
int seconds = 0;
bool rotation[2] = {0};
long currentTime;
long prevTime = 1;
long diffTime;

void setup() {
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(motor_relay, OUTPUT);
  pinMode(ir_detect, INPUT);
  pinMode(rpm_detect, INPUT);
  digitalWrite(led, HIGH);
  digitalWrite(led2, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
}

void loop() {

  int distance = distance_measure();
  int rpm = rpm_check();
  bool flow = ir_check();
  
  if (distance < 40 && rpm !=0) {  // This is where the LED On/Off happens
    digitalWrite(led, HIGH); // When the Red condition is met, the Green LED should turn off
    digitalWrite(led2, LOW);
    digitalWrite(motor_relay, HIGH);
  }
  else {
    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(motor_relay, LOW);
    
    if (flow == LOW){
    //  Serial.print("flowing");
    }
  }
  if (distance >= 200 || distance <= 0) {
   // Serial.println("Out of range");
  }
  else {
    //Serial.print(distance);
   // Serial.println(" cm");
  }
    Serial.print(rpm);
    Serial.println(" rpm");
 // delay(500);
  
}
//------------------------------------------------------------------
//Check IR Sensor. Return presence of absence of seasoning.
bool ir_check() {
  bool detection = HIGH;
  detection = digitalRead(ir_detect);
  if (detection == LOW) {
    return LOW;
  } else {
    return HIGH;
  }
}

//-------------------------------------------------------------------
//This function dedects the rotation of an input shaft
//returns the rpm.
int rpm_check(){
static int rpm;
 // rotation[1] = rotation[2];
  static int seconds_elapsed = millis();
  rotation[1]= digitalRead(rpm_detect);
//  Serial.println(rotation[1]);
  currentTime = micros();
  diffTime = currentTime - prevTime;
//  Serial.println(diffTime);
  if (diffTime > 3000000 && rotation[1] == rotation[2]){
    rpm = 0;
    
  }
  
  if (rotation[1] != rotation[2]){
    
    currentTime = micros();
    diffTime = currentTime - prevTime;
    
    rpm = 60000000/diffTime;
    rpm = rpm /2;
    prevTime = currentTime;
    //rotation[2] = rotation[1];
    
   
  }
 rotation[2] = rotation[1];
 return rpm;
}
int distance_measure(){
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
  
}

