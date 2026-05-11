#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 13;
const int echoPin = 12;
const int buzzer = 7;
const int redLED = 6;
const int yellowLED = 5;
const int greenLED = 4;

const int numReadings = 5; 
int readings[numReadings]; 
int readIndex = 0;
long total = 0;
int averageDistance = 0;

long duration;
int distance;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Parking Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  
  delay(2000);
  lcd.clear();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  total = total - readings[readIndex];    
  readings[readIndex] = distance;        
  total = total + readings[readIndex];    
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;                       
  }

  averageDistance = total / numReadings;  

  lcd.setCursor(0, 0);
  
  if (averageDistance > 0 && averageDistance <= 25) {
    lcd.print("Dist: ");
    lcd.print(averageDistance);
    lcd.print(" cm    "); 
  } else {
    lcd.print("Scanning...     "); 
  }

  lcd.setCursor(0, 1); 

  Serial.print("Average Distance: ");
  Serial.println(averageDistance);


  if (averageDistance > 0 && averageDistance <= 5) { 
    lcd.print("STATUS: STOP!   ");
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    tone(buzzer, 2500); 
  } 
  else if (averageDistance > 5 && averageDistance <= 12) { 
    lcd.print("STATUS: WARNING ");
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
    
    tone(buzzer, 1200); 
    delay(80);          
    noTone(buzzer);
    delay(80);          
  } 
  else if (averageDistance > 12 && averageDistance <= 25) { 
    lcd.print("STATUS: CLEAR   ");
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    noTone(buzzer); 
  } 
  else {
    lcd.print("STATUS: IDLE    ");
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    noTone(buzzer);
  }

  delay(30); 
}
