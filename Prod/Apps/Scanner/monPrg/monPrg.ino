
//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>

LiquidCrystal_I2C lcd(0x3f,20,4); 

void setup()
{
  lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Bonjour !");
  delay(3000);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Scannez un");
  lcd.setCursor(4,1);
  lcd.print("article:");
}


void loop()
{
  if (Serial.available()>0){
    lcd.clear();
    char inChar = Serial.read(); //essaie de lire une entrée puis de l'afficher mais marche pas
    lcd.setCursor(0,0);
    lcd.print(inChar);
  }
}
