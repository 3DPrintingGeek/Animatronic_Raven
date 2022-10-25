#include <Servo.h>

 
#define SERVO_PIN          3
#define TRIGGER_PIN        2
#define LED_PIN           13
#define EYE1_PIN           7
#define EYE2_PIN           8
#define SOUND_PIN          4
#define REFRESH_PERIOD_MS 20

int pos = 0;    // variable to store the servo position 
int speed = 1;
int moveInterval = 5;
int maxPos = 150;
int minPos = 30;
volatile byte active = 0;
volatile byte shouldBlink = 0;
volatile byte shouldBlinkTwice = 0;
volatile byte shouldTwitch = 0;
volatile byte shouldCrow = 0;

Servo myservo;

void triggerIsr() {
  active=1;
}
 
void setup() 
{ 
  pinMode(SOUND_PIN, OUTPUT);
  digitalWrite(SOUND_PIN, LOW);
  pinMode(EYE1_PIN, OUTPUT);
  pinMode(EYE2_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), triggerIsr, RISING);
  myservo.attach(SERVO_PIN);
} 
 
void loop() 
{ 
   if (active) {
    for (int i=0; i<10; i++){
      shouldBlink = random(0, 1);
      if (shouldBlink==0) {
        digitalWrite(EYE1_PIN, HIGH);
        digitalWrite(EYE2_PIN, HIGH);
      }
      shouldBlinkTwice = random(0, 5);
      shouldCrow = random(0, 10);
      shouldTwitch = random(0, 2);
      int target = random(minPos, maxPos);
      birdMove (target);
      if (shouldCrow==0){
        digitalWrite(SOUND_PIN, HIGH); 
      }
      if (shouldBlinkTwice=0) {
        digitalWrite(EYE1_PIN, LOW);
        digitalWrite(EYE2_PIN, LOW);
      }
      maybeTwitch(); 
      digitalWrite(SOUND_PIN, LOW);
      delay(500);
    }
    digitalWrite(EYE1_PIN, LOW);
    digitalWrite(EYE2_PIN, LOW);
    active=0;
  }
} 

void birdMove (int dest) {
  int start = myservo.read();

  if (dest - start > 0) {
    for (pos = start; pos <= dest; pos += speed) {
      myservo.write(pos); // tell servo to go to position in variable 'pos'
      delay(moveInterval);
    }
  }
  else {
    for (pos = start; pos >= dest; pos -= speed) {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(moveInterval);
    }
  }
}

void maybeTwitch () {
  int shouldTwitch = random(1, 3);
  if (shouldTwitch == 3) {
    int timesTwitch = random(3, 5);
    int twitchCount = 0;
    for (twitchCount = 0; twitchCount <= timesTwitch; twitchCount += 1) {
      int min = pos - 10;
      int max = pos + 10;

      if (min < minPos) {
        min = minPos;
      }
      if (max > maxPos) {
        max = maxPos;
      }
      int target = random(min, max);
      birdMove(target);
    }
  }
}

void maybeBlink () {
  int shouldBLink = random(0, 1);
  if (shouldBLink == 0) {
    delay(1000);
    blink();
    int blinkTwice = random(1, 6);
    if (blinkTwice == 1) {
      delay(500);
      blink();
    }
  }
}

void blink () {
  delay(300);
}
