
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <Keyboard.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f,20,4); 
SoftwareSerial mavoieserie(17, 16); // RX2 et TX2
String inputSerialString;


void setup()
{   
    pinMode(LED_BUILTIN, OUTPUT);
    // Ouvre la voie série avec l'ordinateur nombre de braud sur le port com du pc
    Serial.begin(9600);
    // Ouvre la voie série avec le module BT nombre de braud pour le module bluetooth
    mavoieserie.begin(9600);
    

    lcd.init();  
    lcd.backlight();
    lcd.setCursor(4,0);
    lcd.print("Bonjour !");
    delay(3000);
}

void loop() // run over and over
{
    if (mavoieserie.available()) {
        Serial.write(mavoieserie.read());
        if(strlen(mavoieserie.read())>0){

            lcd.clear();
            char inChar = Serial.read(); //essaie de lire une entrée puis de l'afficher mais marche pas
            lcd.setCursor(0,0);
            lcd.print(inChar);

        }
    }

    

    if(Serial.available()==13){
        
        inputSerialString.concat(Serial.readString());
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(inputSerialString);

        if(inputSerialString.length()==13){

            mavoieserie.print(inputSerialString);
            while(mavoieserie.available()>0){
                delay(10);
            }
            mavoieserie.flush();
            
        }
        inputSerialString="";
        
        
        
        
        Serial.flush();

    }
    
    

}
