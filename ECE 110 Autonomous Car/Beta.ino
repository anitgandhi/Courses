/*
Names: Anit Gandhi, Bruce Kim
Assignment: Autonomous Car
Description: Checks to see if it's all white, then split, then regular track
Version: Beta
*/

int leftPin = 3;
int rightPin = 9;
int white = 800;
int black = 250;
int fullSpeed = 100;
int halfSpeed = 50;
int stopSpeed = 0;

void setup()
{
  //set up outputs
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
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
  int frontRightCenter = analogRead(A2);
  int frontRight = analogRead(A3);
  int backCenterLeft = analogRead(A4);
  int backCenterRight = analogRead(A5);
  
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
      if ((frontLeftCenter <= white) && (frontLeftCenter >= black))/*frontRightCenter <= white) && (frontRightCenter >= black)*/
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
      
      if ((frontLeft >= white) && (frontRight <= black) && (frontLeftCenter >=white) && (frontRightCenter >=white )) //left right angle
      {
        run(stopSpeed, halfSpeed);
      }
      else if ((frontLeft <= black) && (frontRight >= white) && (frontLeftCenter >=white) && (frontRightCenter >=white )) //right right angle
      {
        run(fullSpeed,stopSpeed);
      }
      
      else if ((backCenterLeft >=white) && (backCenterRight <=black))  //turn left
      {
        run(stopSpeed, halfSpeed);
      }
        
      else if ((backCenterLeft <= black) && (backCenterRight >=white)) //turn right
      {
        run(halfSpeed, fullSpeed);
      }
      
      else if ((backCenterLeft >=white) && (backCenterRight >=white)) // straight
      {
        run(fullSpeed, fullSpeed);
      }
      
      /*
      if ((backCenterLeft >= white) && (backCenterRight <= black)) // turn left
      {
        if ((frontLeft >= white) && (frontRight <= black))
        {
          run (stopSpeed, halfSpeed);
        }
        else
        {
        run (stopSpeed, fullSpeed);
        }
      }
      
      else if ((backCenterLeft <= black) && (backCenterRight >= white)) // turn right
      {
        if ((frontLeft <= black) && (frontRight >= white))
        {
          run(halfSpeed, stopSpeed);
        }
        else
        {
          run(fullSpeed, stopSpeed);
        }
      }
      
      else
      {
        run (fullSpeed, fullSpeed);
      }
      */
    }
  }
}
