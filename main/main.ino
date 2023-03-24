#include <Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
int pinTemp = A1;
byte degree[] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};


void setup()
{  
  lcd.begin(16,2);
  // lcd.begin();
  lcd.backlight();
  lcd.print("Temperature Sensor");
}

void loop()
{
  lcd.clear();
  float temp = analogRead(pinTemp);    //Read the analog pin
  temp = (20*temp-2035)/41;  //convert output (mv) to readable celcius
  temp *= 1.8;
  temp += 32;
  lcd.createChar(0,degree);
  lcd.setCursor(13,0);
  lcd.write(byte(0));
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.setCursor(14,0);
  lcd.print("F");

  delay(1000);
  
}