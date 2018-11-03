//#include "AirQuality.h"
//#include "Arduino.h"
//#include <SimpleDHT.h>
//
//// air quality sensor
//AirQuality aqs;
//int aq = -1;
//
//// dht pin
//int pinDHT11 = 2;
//
//// temp and humidity sensor
//SimpleDHT11 dht11(pinDHT11);




// LED vars 
const int ledPin = 13;
// const int pwmPin = 3;

// LED read vars
String inputString = "";         // a string to hold incoming data
boolean toggleComplete = false;  // whether the string is complete
// boolean pwmComplete = false;  

// Potmeter vars
const int analogInPin = A0;
int sensorValue = 0;        // value read from the potmeter
// int prevValue = 0;          // previous value from the potmeter

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // init LEDS
  pinMode(ledPin,OUTPUT);
  // pinMode(pwmPin,OUTPUT);
  digitalWrite(ledPin,0);
  // analogWrite(pwmPin,0);
}

void loop() {
   // Recieve data from Node and write it to a String
   while (Serial.available() && toggleComplete == false){// && pwmComplete == false) {
    char inChar = (char)Serial.read();
    if(inChar == 'E'){ // end character for led
     toggleComplete = true;
    }
//    if(inChar == 'P'){
//      pwmComplete = true;
//    }
    else{
      inputString += inChar; 
    }
  }

  // Serial.println(inputString);

  // Toggle LED 13
  if(!Serial.available() && toggleComplete == true) {
    // convert String to int. 
    // int recievedVal = stringToInt();
    Serial.print("received ");
    Serial.println(inputString);

    if (inputString == "0") {
      digitalWrite(A5, 0);
    } else {
      digitalWrite(A5, 1);
    }
    inputString = "";
    toggleComplete = false;
  }
  
//  // Toggle LED 13
//  if(!Serial.available() && toggleComplete == true)
//  {
//    // convert String to int. 
//    int recievedVal = stringToInt();
//    
//    if(recievedVal == 0)
//    {
//      digitalWrite(ledPin,recievedVal);
//    }
//    else if(recievedVal == 1)
//    {
//      digitalWrite(ledPin,recievedVal);
//    }    
//    toggleComplete = false;
//  }
//  // Dim LED 3
//  if(!Serial.available() && pwmComplete == true)
//  {
//    // convert String to int. 
//    int recievedVal = stringToInt();
//    
//    analogWrite(pwmPin,recievedVal);
//    
//    pwmComplete = false;
//  }
    // Potmeter
     sensorValue = analogRead(analogInPin); 
//     Serial.print("sensor ");
//     Serial.println(sensorValue);
       
    // read the analog in value:
//      Serial.print("B"); // begin character 
//      Serial.print(sensorValue);  
//      Serial.print("E"); // end character
//      prevValue = sensorValue;
      
  delay(1000); // give the Arduino some breathing room.
}

int stringToInt() {
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
