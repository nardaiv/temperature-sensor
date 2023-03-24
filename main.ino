#include <LiquidCrystal.h>
int pinTemp = A0;   //This is where our Output data goes

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);   
  lcd.begin(16,2);
  
  lcd.print("hello world sdfjakfkajfkajfkaj");
  delay(3000);
  lcd.clear();
}
void loop() {
 float temp = analogRead(pinTemp);    //Read the analog pin
  temp = (temp/1024*5-0.5)/0.01;  //convert output (mv) to readable celcius
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("C");  //print the temperature status
  lcd.print("Temp : ");
  lcd.print(temp);
  lcd.print(" C");
  delay(500);  
  lcd.clear();

}