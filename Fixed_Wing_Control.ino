#include <Servo.h>

// manual input section
int minClearance = 50; //cm
int elevatorPos = 0; //deg
int posIncrement = 2; //deg
int maxSteering = 30; //deg
int rudderCenter = 90; //deg

// system variables
int pos = 0; 
int triggerRight = 3;
int echoRight = 4;
int triggerLeft = 5;
int echoLeft = 6;

Servo rudder;
Servo elevator;

int rudderPin = 2;
int elevatorPin = 1;

void setup ()
{
  rudder.attach(rudderPin);
  elevator.attach(elevatorPin);
  Serial.begin(9600);

  // Initialize servo
  rudder.write(rudderCenter);
  elevator.write(elevatorPos);

  pinMode(triggerRight, OUTPUT);
  pinMode(triggerLeft, OUTPUT);
  pinMode(echoRight, INPUT);
  pinMode(echoLeft, INPUT);

  // Initialize ultrasound sensor
  digitalWrite(triggerRight, LOW);
  digitalWrite(triggerLeft, LOW);
}

long us2cm(long us)
{
  return us * 0.034 / 2;
}

void loop()
{
  long durationRight, durationLeft, distRight, distLeft;

  digitalWrite(triggerRight, HIGH);
  delayMicroseconds(10); // at least trigger for 10 us to ensure receiving
  digitalWrite(triggerRight, LOW);
  durationRight = pulseIn(echoRight, HIGH, 50000);
  if (durationRight == 0) {
    durationRight = 999999;
  }
  distRight = us2cm(durationRight);

  delay(5); // at least wait for 5 ms to allow one sensor to finish sending and receiving

  digitalWrite(triggerLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerLeft, LOW);
  durationLeft = pulseIn(echoLeft, HIGH, 50000);
  if (durationLeft == 0) {
    durationLeft = 999999;
  };
  distLeft = us2cm(durationLeft);

  if(distRight <= minClearance){
    pos += posIncrement; 
  } else if(distLeft <= minClearance) {
    pos -= posIncrement;
  } else {
    pos = 0;
  }

  pos = constrain(pos, -maxSteering, maxSteering);
  rudder.write(rudderCenter + pos);
}




