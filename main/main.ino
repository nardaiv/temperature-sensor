#include <Wire.h>
#include<LiquidCrystal_I2C.h>

//define button
const int BUTTON_PIN = 2; 

//setup lcd
LiquidCrystal_I2C lcd(32,16,2);

//define Temperature Input
const int pinTemp = A1;

//define counter for switch unit
int counter =0;

//setup delay for button and lcd
const int lcdInt = 500;
const int btnInt = 10;
unsigned long lastlcd = 0;
unsigned long lastbtn = 0;
int lastBtnValue;

//custom character to create degree symbol
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
  Serial.begin(9600);
  lcd.begin(16,2);
  //lcd.init();
  lcd.backlight();
  lcd.print("Temperature Sensor");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(500);
}

void loop()
{
  //check current time
  unsigned long currentTime = millis();
  
  //run button counter based on button delay interval
  if(currentTime - lastbtn >= btnInt){
    int btnValue = digitalRead(BUTTON_PIN);
    lastbtn = currentTime;
    if(btnValue==LOW && btnValue!=lastBtnValue ){
      counter++;
      Serial.println(counter);
      
  	}
    lastBtnValue= btnValue;
    
    if(counter>=2){
      	counter =0;
    }
  }
  
  
  //update lcd screen every 500 ms
  if(currentTime - lastlcd >= lcdInt){
    //update the condition var
    lastlcd = currentTime;
    
    //rendering function
    lcd.clear();
    float temp = analogRead(pinTemp);    //Read the analog pin
    temp = (20*temp-2035)/41;  //convert output (mv) to readable celcius
    char symbol[20] = "C"; 
    
    //change to fahrenheit
    if(counter ==1){
    	temp *= 1.8;
    	temp += 32;
        strcpy(symbol,"F");
    }
    lcd.createChar(0,degree);
    lcd.setCursor(13,0);
    lcd.write(byte(0));
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.setCursor(14,0);
    lcd.print(symbol);
    
  }
  
  
}

