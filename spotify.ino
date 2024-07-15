#include <LiquidCrystal.h>
#include "SR04.h"

//upload once to arduino

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int bufferSize = 64; 
char messageBuffer[bufferSize];
int messageLength = 0;
SR04 sr04 = SR04(10, 9);
long distance;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode(7, INPUT);
}

void updateLcd(){
messageLength = Serial.readBytesUntil('\n', messageBuffer, bufferSize - 1);
    messageBuffer[messageLength] = '\0'; 

    
    char* separator = strchr(messageBuffer, '|');
    
    if (separator != nullptr) {
      *separator = '\0'; 

      
      lcd.setCursor(0, 0);
      lcd.print("                "); 
      lcd.setCursor(0, 0);
      lcd.print(messageBuffer); 

      
      lcd.setCursor(0, 1);
      lcd.print("                "); 
      lcd.setCursor(0, 1);
      lcd.print(separator + 1); 
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
    }
}

void loop() {
  distance = sr04.Distance();

  if (distance < 15) {
    Serial.print("pause\n");
    delay(1000); 
  }
  
  if (analogRead(0) > 800) {
    Serial.print("prev\n");
    updateLcd();
    delay(200); 
  }
  
  if (analogRead(0) < 200) {
    Serial.print("next\n");
    updateLcd();
    delay(200); 
  }
  
  if (Serial.available() > 0) {
    updateLcd();
  }
}
