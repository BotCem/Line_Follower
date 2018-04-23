//#include "line_follower.c"
//unsigned int aux = 0;
////OCR0A = power PD6 or pin 6
////OCR0B = power PD6 or pin 5
//int sensor_list2[NUM_SENSORS];
//#include <Arduino.h>
//
//int main() {
//  init();
//
//  init2();
//  Serial.begin(9600);
//
//
//
//  while(1){
//    readLine(sensor_list2);
//    for(register int i = 0; i < NUM_SENSORS; i++){
//      Serial.print(sensor_list2[i]); Serial.print(" ");
//    }
//    Serial.println(" lalala ");
//    delay(100);
//
//  }
//
//
//}




/*
  Serial Call and Response
 Language: Wiring/Arduino

 This program sends an ASCII A (byte of value 65) on startup
 and repeats that until it gets some data in.
 Then it waits for a byte in the serial port, and
 sends three sensor values whenever it gets a byte in.

 Thanks to Greg Shakar and Scott Fitzgerald for the improvements

   The circuit:
 * potentiometers attached to analog inputs 0 and 1
 * pushbutton attached to digital I/O 2

 Created 26 Sept. 2005
 by Tom Igoe
 modified 24 April 2012
 by Tom Igoe and Scott Fitzgerald

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialCallResponse

 */
byte power1 = 0, power2 = 0;

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() == 2) {
    power1 = Serial.read();
    power2 = Serial.read();

    Serial.print(power1);
    Serial.print(" ");
    Serial.println(power2);
  }

  analogWrite(10,power1);
  analogWrite(9,power2);
  delay(100);
}




