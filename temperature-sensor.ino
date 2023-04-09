#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//define button
const int BUTTON_PIN = 2;

//setup lcd
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//define Temperature Input
const int pinTemp = 7;

//define counter for switch unit
int counter = 0;

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
DHT dht(pinTemp, DHT11);


void setup() {
  //setup terminal for debuging purposes
  Serial.begin(9600);

  //initialize lcd
  lcd.begin(16, 2);
  lcd.backlight();

  //print booting message
  //lcd.print("Temperature Sensor");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.begin();
  message();
  //delay before start measure temperature
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
}

void loop() {
  //check current time
  unsigned long currentTime = millis();

  //run button counter based on button delay interval

  if (currentTime - lastbtn >= btnInt) {
    int btnValue = digitalRead(BUTTON_PIN);
    lastbtn = currentTime;

    if (btnValue == LOW && btnValue != lastBtnValue) {
      //increase counter value for every click
      counter++;

      //return anything greater than 4 to 0
      //so counter is always between 0 to 3
      if (counter >= 5) {
        counter = 0;
      }

      //for checking counter
      Serial.println(counter);
    }

    lastBtnValue = btnValue;
  }


  //update lcd screen every 500 ms
  if (currentTime - lastlcd >= lcdInt) {
    //update the condition var
    lastlcd = currentTime;

    lcd.setCursor(6, 0);
    lcd.print("        ");
    lcd.setCursor(6, 1);
    lcd.print("        ");

    //taking temperature input
    temp = dht.readTemperature();
    
    //changing unit based on click
    changeUnit(counter);
    Serial.println(temp);
    //rendering temperature
    renderTemp(temp, degreeUse);
  }
}


void changeUnit(int type) {
  switch (type) {

    //celcius
    case 0:
      temp *= 1;
      degreeUse = true;
      strcpy(symbol, "C");
      break;

    //fahrenheit
    case 1:
      temp *= 1.8;
      temp += 32;
      degreeUse = true;
      strcpy(symbol, "F");
      break;

    //reamur
    case 2:
      temp *= 0.8;
      degreeUse = true;
      strcpy(symbol, "R");
      break;

    //kelvin
    case 3:
      temp += 273;
      degreeUse = false;
      strcpy(symbol, "K");
      break;

    //M
    case 4:
      temp *= 0.8;
      temp += 15;
      degreeUse = true;
      strcpy(symbol, "M");
      break;
    
    //unmatched input
    default:
      Serial.println("invalid input (out of the switch options)");
  }
}

void renderTemp(float temp, bool degreeStatus) {
  int x = countDigit(temp);
  //show degree if necessary
  if (degreeStatus == true) {
    lcd.createChar(0, degree);
    lcd.setCursor(10+ x, 0);
    lcd.write(byte(0));
    x++;
  }

  //show current temperature
  lcd.setCursor(6, 0);
  lcd.print(temp);
  lcd.print(" ");
  lcd.setCursor(10 + x, 0);
  lcd.print(symbol);

  lcd.setCursor(6, 1);
  lcd.print(dht.readHumidity());
  lcd.print(" %");
}

void message() {
  lcd.clear();
  //message
  char title[255] = "  Temperature Sensor  ";
  char credit[255] = "     by Khalisa, Langit, Rifqi, & Narda";
  //for looping setup
  int y = 0, z = 0;
  int limit = strlen(credit);
  bool back = false;

  unsigned long lastlcd = 0;
  unsigned long lastbtn = 0;
  int lastBtnValue;

  bool isBtnClicked= false;

  while (z <= limit && !isBtnClicked) {
    unsigned long currentTime = millis();    
    if(currentTime - lastbtn >=btnInt){
      
      int btnValue = digitalRead(BUTTON_PIN);

      if (btnValue == LOW && btnValue != lastBtnValue){
        isBtnClicked = true;
      }
      
      lastBtnValue = btnValue;
      lastbtn = currentTime;
    }

    if(currentTime - lastlcd >= 350){
      lcd.clear();
      //create a 16 char to display
      char a[16];
      char b[16];
      strncpy(a, title + y, 16);
      strncpy(b, credit + z, 16);

      //dispaying 16 char
      lcd.setCursor(0, 0);
      lcd.print(a);
      lcd.setCursor(0, 1);
      lcd.print(b);

      //for creating back and forth effect
      (y == 6) ? (back = true) : (y == 0) && (back = false);
      (back) ? (y--) : (y++);

      z++;
      lastlcd = currentTime;
    }    
    
  }
  lcd.clear();
}


int countDigit(int n){
  int digit =0;
  while (n!=0){
    digit++;
    n/=10;
  }
  return digit;
}