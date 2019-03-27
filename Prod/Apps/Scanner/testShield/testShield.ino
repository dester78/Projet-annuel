#include <Max3421e.h>
#include <Usb.h>
#include <SoftwareSerial.h>
#include "Keyboard.h"
#define LED 9

SoftwareSerial mavoieserie(17, 16); // RX2 et TX2
USB usb;

void setup(){
Serial.begin(9600);
Serial.print("\r\nStart");
Serial.print(usb.getUsbTaskState());
Keyboard.read();
//uint8_t descBuff[256];
//USB_DEVICE_DESCRIPTOR *devDesc = (USB_DEVICE_DESCRIPTOR *) descBuff;



//Serial.print(int=devDesc);
pinMode(LED, OUTPUT);
digitalWrite(LED, 0);
mavoieserie.begin(9600);
}

void loop(){
// check for incoming serial data:
  if (Serial.available() > 0) {
    // read incoming serial data:
    char inChar = Serial.read();
    // Type the next ASCII value from what you received:

  }  
}
