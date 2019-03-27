
#include <SoftwareSerial.h>

SoftwareSerial mavoieserie(17, 16); // RX2 et TX2

void setup()
{   
    // Ouvre la voie série avec l'ordinateur nombre de braud sur le port com du pc
    Serial.begin(9600);
    // Ouvre la voie série avec le module BT nombre de braud pour le module bluetooth
    mavoieserie.begin(9600);
}

void loop() // run over and over
{
    if (mavoieserie.available()) {
        Serial.write(mavoieserie.read());
    }
    if (Serial.available()) {
        mavoieserie.write(Serial.read());
    }
}
