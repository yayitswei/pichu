#include <thinkingcap.h>

/*
Controls an Arduino to send current to a Thinking Cap (v0.1)
Uses pulse-width modulation to ramp average voltage up to 5 V, then keeps it going for 20 minutes
Warning:  Do not exceed 2 mA of current!  Ensure that the total resistance of your circuit is at least 2.5 kΩ
Written 2012 by ianmathwiz7, of Grindhouse Wetware (ianmathwiz7@gmail.com)
*/


int redpin = 13;      //Pin that controls the red light in the status LED
int greenpin = 12;    //Pin that controls the green light in the status LED
int bluepin = 11;     //Pin that controls the blue light in the status LED
int circuitpin = 10;  //Pin to which the electrode is attached
int buttonpin = 9;
int timer = 0;        //Counts 20 minutes
int pwmtime = 10000;  //How long you want it to ramp
double maxpulse = 100;//Maximum pulse width

//boolean buttonstatus;
//boolean lastbuttonstatus = LOW;
//long lastdebouncetime = 0;
//long debouncedelay = 50;
//boolean ledOn = LOW;
//int color = -1;

boolean StartTC;
ThinkingCap *tcap = new ThinkingCap();
boolean tDCSon = LOW;

void setup()
{
  StartTC = (*tcap).begin();
  pinMode(circuitpin, OUTPUT);
  Serial.begin(9600);
  pinMode(buttonpin, INPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(redpin, OUTPUT);
  digitalWrite(redpin, LOW);
  digitalWrite(greenpin, LOW);
  digitalWrite(bluepin, LOW);
}



void loop()
{
  while(1)
  {
    if (StartTC)
    {
      int time = 30;
      (*tcap).tDCS(time / 2, time);  //Perform tDCS for that time, with ramp time equal to 1/120 of that
    }
  }
  
//  while (1)
//  {
//    if (StartTC) 
//    {
//      boolean reading = digitalRead(buttonpin);
//      if (reading != lastbuttonstatus) {
//        if (reading)
//        {
//          //color++;
//          //color%=8;
//          tDCSon = !tDCSon;
//        }
//        lastdebouncetime = millis();
//      }
//      if ((millis() - lastdebouncetime) > debouncedelay) 
//      {
//        buttonstatus = reading;
//      }
//      if (tDCSon) 
//      {
//        int time = 30; //Convert the input value into a time
//          (*tcap).tDCS(time / 2, time);  //Perform tDCS for that time, with ramp time equal to 1/120 of that
//      }
//      //digitalWrite(redpin, (color & 1));
//      //digitalWrite(greenpin, (color & 2)>>1);
//      //digitalWrite(bluepin, (color & 4)>>2);
//      lastbuttonstatus = reading;
//    }
//  }
for(;;) continue;
  
  
  int val = (int)millis()/1000.0;
  int pwmVal = map(val,0,30,0,255);
  if(val < 30)
  {
    //If it isn't, generate a square pulse
    analogWrite(circuitpin, pwmVal); 
  }
  else
  {
    //Otherwise, keep the current flowing and zero the timer
    digitalWrite(circuitpin, HIGH);
    timer = millis();
    //Turn status LED red
    digitalWrite(bluepin, LOW);
    digitalWrite(greenpin, LOW);
    digitalWrite(redpin, HIGH);
    Serial.println("The current is fully ramped; 20 minutes left");
    //While 20 minutes haven't passed
    while((millis() - timer) < 1200000)
    {
      //Serial.println("I'm in yr loop"); (used to help debug code, and kept for its pure hilariousness)
      //Every 30 seconds, print how much time is left
      if(!((millis() - timer) % 30000))
      {
        Serial.print(1200 - static_cast<double>(millis() - timer)/1000);
        Serial.println(" seconds left!");
      }
    }
    //After 20 minutes, print "time's up" and turn off current
    Serial.println("Time's up!");
    digitalWrite(circuitpin, LOW);
    digitalWrite(redpin, LOW);
    digitalWrite(greenpin, HIGH);
    for(;;) continue;
  }
  
}


double pulsewidth() //Gets the width of the pulse appropriate for the current time
{
  return maxpulse / pwmtime * millis();
}
