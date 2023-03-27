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

//define variable for symbol usage
bool degreeUse = true;
char symbol[20]; 

//define temp variable
float temp;

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
  //setup terminal for debuging purposes
  Serial.begin(9600);
  
  //initialize lcd
  lcd.begin(16,2);
  lcd.backlight();
  
  //print booting message
  lcd.print("Temperature Sensor");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  //delay before start measure temperature
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
      //increase counter value for every click
      counter++;
      
      //return anything greater than 4 to 0
      //so counter is always between 0 to 3
      if(counter>=4){
          counter =0;
      }

      //for checking counter
      Serial.println(counter);
        
  	}
    
    lastBtnValue= btnValue;
    
  }
  
  
  //update lcd screen every 500 ms
  if(currentTime - lastlcd >= lcdInt){
    //update the condition var
    lastlcd = currentTime;
    
    //clearing lcd
    lcd.clear();
    
    //taking temperature input
    temp = analogRead(pinTemp);    //Read the analog pin
    temp = (20*temp-2035)/41;  //convert output (mv) to readable celcius
	
    //changing unit based on click
    changeUnit(counter);
    
    //rendering temperature
	renderTemp(temp,degreeUse);
    
  }
  
  
}


float changeUnit(int type){
  switch(type){
    
    //celcius
    case 0:
    	temp *= 1;
    	degreeUse = true;
    	strcpy(symbol,"C");
    	break;
    
    //fahrenheit
    case 1:
    	temp *= 1.8;
    	temp += 32;
    	degreeUse = true;
    	strcpy(symbol,"F");
    	break;
    
    //reamur
    case 2:
    	temp *= 0.8;
    	degreeUse = true;
    	strcpy(symbol,"R");
    	break;
    
    //kelvin
    case 3:
    	temp += 273;
    	degreeUse = false;
    	strcpy(symbol,"K");
    	break;
    
    //unmatched input
    default:
    	Serial.println("invalid input (out of the switch options)");
  
  }
  
}

void renderTemp(float temp, bool degreeStatus){
  
  //show degree if necessary 
  if(degreeStatus == true){
    lcd.createChar(0,degree);
    lcd.setCursor(13,0);
    lcd.write(byte(0));
  }
  
  //show current temperature
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.setCursor(14,0);
  lcd.print(symbol);
  
}