/*
Names: Anit Gandhi, Bruce Kim
Assignment: Autonomous Car
Description: Checks to see if it's all white, then split, then regular track
Version: Gamma
*/

// Outputs
int leftPin = 3;
int rightPin = 9;
// Inputs
int white = 800;
int black = 250;
// Speeds
int fullSpeed = 150;
int halfSpeed = fullSpeed/2;
int stopSpeed = 0;

// Setup outputs
void setup()
{
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
}

// Method to output speed to motor
void run(int left, int right)
{
  analogWrite(leftPin, left);
  analogWrite(rightPin, right);
}

// Main loop
void loop()
{
  // Takes in inputs from sensors
  int frontLeft = analogRead(A0);
  int frontCenterLeft = analogRead(A1);
  int frontCenterRight = analogRead(A2);
  int frontRight = analogRead(A3);
  int backCenterLeft = analogRead(A4);
  int backCenterRight = analogRead(A5);
  
  // Stop conditions
  boolean allWhite = (frontLeft >= white) && (frontRight >= white) && (frontCenterLeft >= white) && (frontCenterRight >= white) && (backCenterLeft>= white) && (backCenterRight >= white);
  
  // Split conditions
  boolean split = (frontLeft >= white) && (frontRight >= white);
  // gray is if the two front center ones see a value between black and white
  boolean gray = ((frontCenterLeft <= white) && (frontCenterLeft >= black)) && ((frontCenterRight <= white) && (frontCenterRight >= black));
  
  // Tape following conditions
  boolean frontCentersWhite = (frontCenterLeft >=white) && (frontCenterRight >= white);
  boolean left = (backCenterLeft >= white) && (backCenterRight <= black);
  boolean left90 = (frontLeft >= white) && (frontRight <= black) && (frontCentersWhite);
  boolean right = (backCenterLeft <= black) && (backCenterRight >= white);
  boolean right90 = (frontLeft <= black) && (frontRight >= white) && (frontCentersWhite);
  boolean straight = (backCenterLeft >= white) && (backCenterRight >= white);
  
  // Stop circuit
  // If all the sensors see white, then stop
  if (allWhite)
  {run(stopSpeed,stopSpeed);}
  
  else
  {
    // Split Navigation
    if (split)
    {
      if (gray) //go right if color
      {run(halfSpeed, stopSpeed);}
      else //go left if no color
      {run(stopSpeed, halfSpeed);}
    }
    else // regular track navigation using tape navigation
    {
      if (left90)
      {
        delay(250);
        unsigned long startTime = millis();
        unsigned long interval = 1000UL;
        while (millis() - startTime < interval) // Run for 1 second before continuing
        {
          run(stopSpeed, halfSpeed);
        }
      }
      
      else if (right90)
      {
        delay(250);
        unsigned long startTime = millis();
        unsigned long interval = 1000UL;
        while (millis() - startTime < interval) // Run for 1 second before continuing
        {
          run(halfSpeed, stopSpeed);
        }
      }
      
      else if (left)
      {
        run(stopSpeed, halfSpeed);
      }
      
      else if (right)
      {
        {run(halfSpeed, stopSpeed);}
      }
      
      else if (straight)
      {
        run(halfSpeed, halfSpeed);
      }
    }
  }
}
