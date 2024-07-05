#include "WiFi.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 10800, 60000);

const char* ssid = "EYUP DENEYAP";
const char* passwd = "MTH#122016?!.";

unsigned long timer = 0;
bool buzz = false;
int counter = 0;
int buzzer = D9;
int latchPin = D4;
int clockPin = D3;
int dataPin = D2;
int A = D5;
int B = D6;
int C = D7;
int D = D8;
const byte digits[] = {0b00000011, 0b10011111, 0b00100101, 0b00001101, 0b10011001, 0b01001001, 0b01000001, 0b00011111, 0b00000001, 0b00001001};
const byte digitsplus[] = {0b00000010, 0b10011110, 0b00100100, 0b00001100, 0b10011000, 0b01001000, 0b01000000, 0b00011110, 0b00000000, 0b00001000};
const int digitPins[] = {A, B, C, D};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, passwd);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi baglantisi kuruldu.");

  timeClient.begin();

  pinMode(buzzer, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }
}
 
void loop() {
  timeClient.update();
  
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  
  displayTime(hours, minutes);
  if(minutes == 0 && counter < 4){
    if (millis() - timer > 200){ // Her 100 ms de led ve buzzer'in durumu değişerek ses ve ışık yayması sağlandı
      counter++;
      buzz = !buzz;
      digitalWrite(buzzer, buzz);
      timer = millis();
    }
  }
  else if(minutes != 0){
    counter = 0;
  }
}

void displayDigits(byte digit, int position){
  if(digitPins[position] == A){
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
  }
  else if(digitPins[position] == B){
    digitalWrite(A,LOW);
    digitalWrite(C,LOW);
    digitalWrite(D,LOW);
  }
  else if(digitPins[position] == C){
    digitalWrite(B,LOW);
    digitalWrite(A,LOW);
    digitalWrite(D,LOW);
  }
  else if(digitPins[position] == D){
    digitalWrite(B,LOW);
    digitalWrite(C,LOW);
    digitalWrite(A,LOW);
  }
  
  digitalWrite(digitPins[position], HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void displayTime(int hours, int minutes){
  byte hourFirst = digits[hours / 10];
  byte hourSecond = digitsplus[hours % 10];
  byte minuteFirst = digits[minutes / 10];
  byte minuteSecond = digits[minutes % 10];

  displayDigits(hourFirst, 3);
  delay(1);
  displayDigits(hourSecond, 2);
  delay(1);
  displayDigits(minuteFirst, 1);
  delay(1);
  displayDigits(minuteSecond, 0);
  delay(1);
}
