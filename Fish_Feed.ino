#include <Wire.h>
#include "RTClib.h"
#include <Servo.h>

#define led D5

RTC_DS3231 rtc;
Servo myservo;

int Jam,Menit,Detik;

void setup() {
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  Wire.begin();
  myservo.attach(D4);
  myservo.write(0);
  rtc.begin();
  rtc.adjust(DateTime(__DATE__,__TIME__));
  
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__,__TIME__));
    digitalWrite(led,HIGH);
  }
}

void loop() {
  DateTime now = rtc.now();
  
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Jam = now.hour();
  Menit = now.minute();
  Detik = now.second();

  Serial.println(Jam);
  Serial.println(Menit);

  if ((Jam==10 && Menit==0 && Detik==0) || (Jam==18 && Menit==00 && Detik==0)){
     myservo.write(90);
     delay(3000);
     myservo.write(0);
  }
}
