/*
Names: Anit Gandhi, Bruce Kim
Assignment: Autonomous Car
Description: Checks to see if it's all white, then split, then regular track
Version: Alpha
*/

int leftPin = 3;
int rightPin = 9;
int white = 800;
int black = 250;
int fullSpeed = 75;
int stopSpeed = 0;

void setup()
{
  //set up outputs
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  //Serial.begin(9600);
}

void run(int l, int r)
{
  // method to clean up code later on
  analogWrite(leftPin, l);
  analogWrite(rightPin, r);
}

void loop()
{
  // set up inputs
  int frontLeft = analogRead(A0);
  int frontLeftCenter = analogRead(A1);
  int frontRightCenter = analogRead(A2);\
  int frontRight = analogRead(A3);
  int backCenterLeft = analogRead(A4);
  int backCenterRight = analogRead(A5);
  
  //Serial.println(frontLeft);
  
  // Stop circuit
  if ((frontLeft >= white) && (frontRight >= white) && (frontLeftCenter >= white) && (frontRightCenter >= white) && (backCenterLeft>= white) && (backCenterRight >= white))
  {
    run(stopSpeed,stopSpeed);
  }
  
  else
  {
    // Split Navigation
    if ((frontLeft >= white) && (frontRight >= white))
    {
      if (((frontLeftCenter <= white) && (frontLeftCenter >= black)) && ((frontRightCenter <= white) && (frontRightCenter >= white)))
      //go right if color
      {
        run(fullSpeed, stopSpeed);
      }
      else 
      //go left if no color
      {
        run(stopSpeed, fullSpeed);
      }
    }
    else 
    //track navigation using tape following (two back sensors)
    {
      if ((backCenterLeft >=white) && (backCenterRight >=white))
      {
        run(fullSpeed, fullSpeed);
      }
      else if ((backCenterLeft >=white) && (backCenterRight <=black))
      {
        run(stopSpeed, fullSpeed);
      }
      else if ((backCenterLeft <= black) && (backCenterRight >=white))
      {
        run(fullSpeed, stopSpeed);
      }
    }
  }
  
}
