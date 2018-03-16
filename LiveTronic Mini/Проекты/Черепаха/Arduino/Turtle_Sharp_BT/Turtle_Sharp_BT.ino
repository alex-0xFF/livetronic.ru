#define CLOSE 160
#define OPEN 110

#include <Servo.h>



int simpleCount;
int headTimer;
Servo neck, gripper;
const int distance_pin = A0;
int command, nextcommand;
int mode = 0;

// ------------------------- Legs -----------------------------
const int loopRestart = 10;
class Leg {
public:
  Leg(int shoulderCenter, char shoulderSign, int kneeCenter, int kneeSign)
: 
    shoulderCenter_(shoulderCenter)
      , shoulderSign_(shoulderSign)
        , kneeCenter_(kneeCenter)
          , kneeSign_(kneeSign)
          { 
          }

  void attach(int shoulderPin, int kneePin) {
    shoulder_.attach(shoulderPin);
    knee_.attach(kneePin);
  }

  int getShoulderDelta() { 
    return shoulderSign_ * shoulder_.read() - shoulderCenter_; 
  }
  int getKneeDelta() { 
    return kneeSign_ * knee_.read() - kneeCenter_; 
  }

  void set(int shoulderDelta, int kneeDelta) {
    int shoulderTarget = shoulderCenter_ + shoulderSign_ * shoulderDelta;

    shoulder_.write(shoulderTarget);

    int kneeTarget = kneeCenter_ + kneeSign_ * kneeDelta;
    knee_.write(kneeTarget);
  }

  void setKnee(int kneeDelta) {
    int kneeTarget = kneeCenter_ + kneeSign_ * kneeDelta;
    knee_.write(kneeTarget);


  }

  void setTarget(int shoulderTarget, int kneeTarget, int delay)
  {
    shoulderTarget_ = shoulderTarget;
    shoulderV_ = ((float)shoulderTarget - getShoulderDelta()) / delay;
    kneeTarget_ = kneeTarget;
    kneeV_ = ((float)kneeTarget - getKneeDelta()) / delay;

    //Serial.println(headTimer);

    targetTime_ = millis() + delay;
  }

  bool ease()
  {
    unsigned long curTime = millis();
    if (curTime >= targetTime_) {
      set(shoulderTarget_, kneeTarget_);
      return true;
    }

    set(shoulderTarget_ - shoulderV_ * (targetTime_ - curTime), kneeTarget_ - kneeV_ * (targetTime_ - curTime));
    return false;
  }

private:
  Servo shoulder_;
  Servo knee_;
  const int shoulderCenter_;
  const char shoulderSign_;
  const int kneeCenter_;
  const char kneeSign_;

  float shoulderV_;
  float kneeV_;
  int shoulderTarget_;
  int kneeTarget_;
  unsigned long targetTime_;
  // int headTimer; 
};

// Counterclockwise from front right leg.
// Initial position is should be calibrated to approximately perpendicular to movement axis.

Leg legs[4] = {
  Leg(50, +1, 115, +1), Leg(120, -1, 65, -1), Leg(65, -1, 100, +1), Leg(115, 1, 80, -1)};
Leg& frontRightLeg = legs[0];
Leg& frontLeftLeg = legs[1];
Leg& backLeftLeg = legs[2];
Leg& backRightLeg = legs[3];


void easeAllLegs()
{
  while(true) {
    delay(15);
    if (legs[0].ease() && legs[1].ease() && legs[2].ease() && legs[3].ease()) {
      break;
    }
  }
}

void resetAllLegs()
{
  for (int i = 0; i < 4; ++i) {
    legs[i].set(0, 0);
  } 
}

// ------------------------- Movement ----------------------------

// Equal interval sin table:
const int NUM_STEPS = 8;
const float CIRCLE[NUM_STEPS] = {
  0, 0.71, 1.0, 0.71, 0, -0.71, -1.0, -0.71};

int wrapAround(int i)
{
  if (i >= NUM_STEPS) {
    return i - NUM_STEPS;
  }
  if (i < 0) {
    return i + NUM_STEPS;
  }
  return i;
}

struct LegAngles {
  int shoulder;
  int knee;
};

class Movement
{
public:
  Movement(const LegAngles* legsAmplitudes)
: 
    step_(0), legsAmplitudes_(legsAmplitudes)
    { 
    }

  void reset() { 
    step_ = 0; 
  }

  void performStep(int delay_, int stepIncrement)
  {
    if (headTimer == loopRestart) {
      //  headLoop();
      //  headTimer = 0;
    }
    else{
      headTimer++;
    }

    for (int iLeg = 0; iLeg < 4; ++iLeg) {
      legs[iLeg].setTarget(CIRCLE[step_]*legsAmplitudes_[iLeg].shoulder, CIRCLE[wrapAround(step_ + NUM_STEPS/4)]*legsAmplitudes_[iLeg].knee, delay_);
    }

    easeAllLegs();

    step_ = wrapAround(step_ + stepIncrement);
  }

private:
  char step_;
  const LegAngles* legsAmplitudes_;
};


LegAngles FORWARD_AMPLITUDES[4] = {
  {
    30, 10  }
  , {
    -30, -10  }
  , {
    30, 10  }
  , {
    -30, -10  }
};
Movement forward(FORWARD_AMPLITUDES);

LegAngles LEFT_TURN_AMPLITUDES[4] = {
  {
    30, 20  }
  , {
    30, -20  }
  , {
    -30, 20  }
  , {
    -30, -20  }
};
Movement leftTurn(LEFT_TURN_AMPLITUDES);

void turnDegrees(int degrees) {
  const int DEGREES_PER_STEP = 6;

  degrees = degrees % 360;
  if (degrees > 180) degrees -= 360;
  else if (degrees < -180) degrees += 360;

  int stepIncrement = degrees > 0 ? +1 : -1;
  for (degrees = abs(degrees); degrees > 0; degrees -= DEGREES_PER_STEP) {
    leftTurn.performStep(100, stepIncrement);
  }
}

void moveCm(int cm) {
  const float MM_PER_STEP = 1.3;
  int stepIncrement = cm > 0 ? +1 : -1;
  for (int mm = abs(cm) * 10; mm > 0; mm -= MM_PER_STEP) {
    forward.performStep(100, stepIncrement);
  } 
}

void backpedal(int delay_, int numSteps) {
  static const LegAngles leftAmplitudes = {
    30, 40  };
  static const LegAngles rightAmplitudes = {
    -35, -40  };

  frontLeftLeg.set(80, 0);
  frontRightLeg.set(80, 0);

  delay(200);

  int step = 0;
  for (int i = 0; i < numSteps; ++i) {
    backLeftLeg.setTarget(CIRCLE[step]*leftAmplitudes.shoulder, CIRCLE[wrapAround(step + NUM_STEPS/4)]*leftAmplitudes.knee, delay_);
    backRightLeg.setTarget(CIRCLE[step]*rightAmplitudes.shoulder, CIRCLE[wrapAround(step + NUM_STEPS/4)]*rightAmplitudes.knee, delay_);

    while(true) {
      delay(15);
      if (backLeftLeg.ease() && backRightLeg.ease()) {
        break;
      }
    }

    step = wrapAround(step - 1);
  }
}

class Head {
public:
  Head() : 
  MAX_DIST_CM(100) { 
  }

  void attach(int triggerPin, int headPin) {
    //  sonar_ = new NewPing(triggerPin, headPin, MAX_DIST_CM);
  }

  int distCm() {
    //    int dist = sonar_->convert_cm(sonar_->ping_median(5));
    //     return (dist == NO_ECHO) ? MAX_DIST_CM : dist;
  }

  ~Head() {
    //     delete sonar_; 
  }
private:
  const int MAX_DIST_CM;
  //  NewPing* sonar_;
};

class Jaw {
public:
  void attach(int pin) {
    servo_.attach(pin); 
  }

  void open() {
    servo_.write(108); 
  }

  void close() {
    servo_.write(75); 
  }

private:
  Servo servo_;
};  

Jaw jaw;
Head head;

const char BUMP_SENSOR_PIN = 0;

volatile int fwdDist = 100;
volatile int floorDist = 0;
volatile bool foundBall = false;
volatile bool isOff = false;



void bumpSensorLoop() {
  unsigned long curTime = millis();
  if (foundBall == true) {
    return;
  }

  if (digitalRead(BUMP_SENSOR_PIN) == 0) {
    foundBall = true;
    //Serial.println("FOUND!");
  }
}

void catchBall() {
  //Serial.println("CATCH!");

  frontRightLeg.setKnee(30);
  frontLeftLeg.setKnee(30);
  delay(100);

  jaw.close();
  delay(100);

  moveCm(-5);

  for (int i = 0; i < 100; ++i) {
    delay(100);
    if (fwdDist < 10) {
      jaw.open();
      delay(500);
      break;
    }  
  }

  jaw.open();

  resetAllLegs();
}

int getDist(){
  int dist = 1023 - analogRead(distance_pin);
  //Serial.println(dist);
  return dist;
}

int ReadCommand() {
  int result = -1;
  if (Serial.available()) {
    result = Serial.read();
  }
  return result;
}

void Test() {
 resetAllLegs();
 forward.performStep(100, 1);  
}

void setup() {
  Serial.begin(9600);
  //Serial2.begin(9600);
  
  pinMode(distance_pin, INPUT);

  frontLeftLeg.attach(6, 7); // 6 - плечо, 7 - колено
  backLeftLeg.attach(8, 9);
  backRightLeg.attach(10, 11);
  frontRightLeg.attach(12, 13);
  resetAllLegs();

  neck.attach(4);
  neck.write(90);

  gripper.attach(5);
  gripper.write(OPEN);

  nextcommand = ReadCommand();
}

void loop() { 
  command = nextcommand;
  //forward.performStep(100, 1); return;
  
  do {
    switch (command) {
      case 'c': mode = 1; break;
      case 'd': mode = 0; break;
      case 'b': forward.performStep(100, -1); break;
      case 'l': turnDegrees(10); break;
      case 'r': turnDegrees(-10); break;
      case 'f': forward.performStep(100, 1); break;    
      case 'g': gripper.write(CLOSE); break; 
      case 'h': gripper.write(OPEN); break; 
    }
    nextcommand = ReadCommand();
    if (mode == 0) {
      if (getDist() < 700) {
      //  turnDegrees(-20);
      
      } 
      else {
        forward.performStep(100, 1); 
      } 
    }
  } while (nextcommand==-1);
  
  /*if (mode == 0) {
    if (getDist() < 700) {
      turnDegrees(-20);
    } 
    else {
      forward.performStep(100, 1); 
    } 
  }*/
  
}


