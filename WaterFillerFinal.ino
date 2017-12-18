#include <LiquidCrystal.h>
#include <Servo.h>

bool buttonPress = false;
double lastDebounceTime = 0;
int buttonCounter = 0;
int servoPin1 = 4;
int servoPin2 = 5;
double startTime = 0;
double currentTime = 0;
int buttonPin = 7;
bool buttonState = LOW;
int debounceDelay = 500;
bool timerStart = false;
bool operate = false;
double currentCountdown = 5;
bool countdownTrue = false;
double dispenseTimer = 0;
double countdownStart = 0;
int amountVal = 0;
int estTime = 0;
double operateTime = 0;
int currentAmount = 0;
double timeRemaining = 0;
bool systemStart = true;
LiquidCrystal lcd(2, 3, 10, 11, 9, 12);
Servo Servo1;
Servo Servo2;

void setup() 
{
  Serial.begin(9600);
  Servo1.attach(servoPin1);
  Servo2.attach(servoPin2);
  Servo1.write(0);
  Servo2.write(0);  
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
}

void loop() 
{
ButtonPressed();
ScreenDisplay();
EstimationCalculator();
//if(systemStart)
//  {
//  Servo1.write(0);
//  Servo2.write(0);  
//  systemStart = false;
//  }
if (countdownTrue)
  {
  CountdownTimer();  
  }
  
else if (operate)
  {
  MotorOperation();  
  }
}

void ButtonPressed()
{
  //sample the state of the button - is it pressed or not?
  buttonState = digitalRead(buttonPin);
 
  //filter out any noise by setting a time buffer
  if ((((millis() - lastDebounceTime) > debounceDelay)) && (!(operate))) 
    {
    if (buttonState == HIGH)
      {
      buttonCounter++;
      countdownTrue = true;
      currentCountdown = 5000;
      countdownStart = millis();
      lcd.clear();
      Serial.println("Pressed");
      if (buttonCounter == 5)
        {
        buttonCounter = 0;  
        }
      lastDebounceTime = millis();
      }
    }  
}

void CountdownTimer()
{
  currentCountdown = (5000 - (millis() - countdownStart))/1000 ;
  if (currentCountdown <= 0)
    {
    countdownTrue = false;
    operate = true;
    lcd.clear();
    operateTime = millis();
    }
}

void MotorOperation()
{

  if((buttonCounter == 1) && (timeRemaining > 0)) // 16 oz
    {
    Servo1.write(120);
    Servo2.write(120);
    }
    
  else if ((buttonCounter == 2) && (timeRemaining > 0)) // 32 oz
    {
    Servo1.write(120);
    Servo2.write(120);
    }
    
  else if ((buttonCounter == 3) && (timeRemaining > 0)) // 64 oz
    {
    Servo1.write(120);
    Servo2.write(120);
    }

  else if ((buttonCounter == 4) && (timeRemaining > 0)) // 128 oz
    {
    Servo1.write(120);
    Servo2.write(120);
    }
    
  else if (timeRemaining <= 0)
    {
    Servo1.write(0);
    Servo2.write(0);
    operate = false;
    buttonCounter = 0;
    lcd.clear();
    }
}

void ScreenDisplay()
{
  if(!(operate))
    {
    if(buttonCounter == 0)
      {
      lcd.setCursor(0,0);
      lcd.print("Press Button..");
      lcd.setCursor(0,1);
      lcd.print("For Water Amt");
      }
      
    else if(buttonCounter == 1) // 16 oz
      {
      lcd.setCursor(0,0);
      lcd.print("16 Oz");
      lcd.print(" ETA:");
      estTime = 16/0.73;
      lcd.print(estTime);
      amountVal = 16;
      if (countdownTrue)
        {
        lcd.setCursor(0,1);
        lcd.print("Countdown: ");
        lcd.print(currentCountdown);
        }
      }
      
    else if(buttonCounter == 2) // 32 oz
      {
      lcd.setCursor(0,0);
      lcd.print("32 Oz");
      lcd.print(" ETA:");
      estTime = 32/0.73;
      lcd.print(estTime);
      amountVal = 32;
      lcd.setCursor(0,1);
      if (countdownTrue)
        {
        lcd.setCursor(0,1);
        lcd.print("Countdown: ");
        lcd.print(currentCountdown);
        }
      }
      
    else if(buttonCounter == 3) // 64 oz
      {
      lcd.setCursor(0,0);
      lcd.print("64 Oz");
      lcd.print(" ETA:");
      estTime = 64/0.73;
      lcd.print(estTime);
      amountVal = 64;
      lcd.setCursor(0,1);
      if (countdownTrue)
        {
        lcd.setCursor(0,1);
        lcd.print("Countdown: ");
        lcd.print(currentCountdown);
        }
      }
  
    else if(buttonCounter == 4) // 128 oz
      {
      lcd.setCursor(0,0);
      lcd.print("128 Oz");
      lcd.print(" ETA:");
      estTime = 128/0.73;
      lcd.print(estTime);
      amountVal = 128;
      lcd.setCursor(0,1);
      if (countdownTrue)
        {
        lcd.setCursor(0,1);
        lcd.print("Countdown: ");
        lcd.print(currentCountdown);
        }
      }
    }
    
  if(operate)
    {
    if(buttonCounter > 0) // 16 oz
      {
      lcd.setCursor(0,0);
      lcd.print(currentAmount);
      lcd.print("/");
      lcd.print(amountVal);
      lcd.print("oz");
      lcd.setCursor(0,1);
      lcd.print("Time Left: ");
      lcd.print(timeRemaining);
      }

}
}

void EstimationCalculator()
{
  timeRemaining = (estTime - ((millis() - operateTime)/1000));
  currentAmount = (0.73 * (millis() - operateTime)) / 1000;
}

