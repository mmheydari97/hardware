#include <LiquidCrystal.h>

const int buttonPin1 = 11;        
const int buttonPin2 = 10;    
const int Led1 = 2;
const int Led2 = 3;

int ledState1 = HIGH;         
int buttonState1;            
int lastButtonState1 = LOW;   
int ledState2 = HIGH;
int buttonState2;
int lastButtonState2 = LOW;

//temp
int value=0;            
float volts=0.0;  

float temps[20] ;
float temp = 0.0;
float temp_old = 0.0;
float yout = 0.0;
float yold = 0.0;

float avg =0.0;
float tempd= 0.0;  

int i = 0;


unsigned long lastDebounceTime1 = 0; 
unsigned long debounceDelay1 = 50;    
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay2 = 50;

const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
 
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  Serial.begin(9600);
  
  value=analogRead(A0);
  volts=(value/1024.0)*5.0;
  temp= volts*100.0;
  tempd =temp;
  int n;
  for (n = 0; n < 20; n = n + 1)
  {
    temps[i]=temp;
  }

  digitalWrite(Led2,HIGH);
  digitalWrite(Led1,HIGH);
}

void loop() {
  value=analogRead(A0);
  volts=(value/1024.0)*5.0;
  temp_old = temp;
  temp= volts*100.0;
  
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);
  
  if (reading1 != lastButtonState1) {
    
    lastDebounceTime1 = millis();
  }
  if (reading2 != lastButtonState2) {
    
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay1) {
    
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      
      
      if (buttonState1 == HIGH) {
        ledState1 = !ledState1;
        tempd=tempd+1;
      }
    }
  }
  if ((millis() - lastDebounceTime2) > debounceDelay2) {
   
    if (reading2 != buttonState2) {
      buttonState2 = reading2;

      
      if (buttonState2 == HIGH) {
        ledState2 = !ledState2;
        tempd=tempd-1;
      }
    }
  }
  if(i==20)
    i=0;
    temps[i]=temp;
    i=i+1;
    
    int x;
    avg=0;
    for(x=0 ; x<20 ; x=x+1) // finding average.
    {
      avg=avg+temps[x]/20;
    }
    yold = avg;
    yout = 0.9691*yold + 0.015466 *temp + 0.015466* temp_old;
    
    
  lcd.setCursor(0, 0);
  lcd.print("current T : ");
  lcd.print((int)yout);
  lcd.setCursor(0, 1);
  lcd.print("desired T : ");
  lcd.print((int)tempd);
  
       if ((int)(yout-(int)tempd)>=3)
        {
           digitalWrite(Led1,LOW);
        }
       else if ((int)((int)tempd-yout)>=3)
        {
            digitalWrite(Led2,LOW);
            digitalWrite(Led1,LOW);
        }
  
  else {
    digitalWrite(Led2,HIGH);
    digitalWrite(Led1,HIGH);
    }
  
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;

}
