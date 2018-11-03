#include "AirQuality.h"
#include "Arduino.h"
#include <SimpleDHT.h>

// air quality sensor
AirQuality aqs;
int aq = -1;

// dht pin
int pinDHT11 = 2;

// temp and humidity sensor
SimpleDHT11 dht11(pinDHT11);




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

// the desired humidity set by the user
int userHumidity = 60;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // init LEDS
  pinMode(ledPin, OUTPUT);
  // pinMode(pwmPin,OUTPUT);
  digitalWrite(ledPin, 0);
  // analogWrite(pwmPin,0);
}

void loop() {
  // Recieve data from Node and write it to a String

  for (int i = 0; i < 100; i++) {
    if (Serial.available() > 0 && toggleComplete == false) { // && pwmComplete == false) {
      char inChar = (char)Serial.read();
      if (inChar == 'E') { // end character for led
        toggleComplete = true;
      }
      //    if(inChar == 'P'){
      //      pwmComplete = true;
      //    }
      else {
        inputString += inChar;
      }
    }

    if (toggleComplete) break;
    delay(20);
  }

  Serial.println("broken");

  // get temp and humidity
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;

  // if there's an error
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(1000);
    return;
  }

  if (inputString == "0") {
    digitalWrite(A5, 0);
  } else {
    // print temperature
    Serial.print("temperature:");
    Serial.print((int)temperature);
    Serial.println(" *C, ");

    // print humidity
    Serial.print("humidity:");
    Serial.print((int)humidity);
    Serial.println(" H");

    if ((int)humidity < userHumidity) {
      Serial.print("Less than ");
      digitalWrite(A5, HIGH);
    } else {
      Serial.print("More than ");
      digitalWrite(A5, LOW);
    }

    Serial.println(userHumidity);
  }
  inputString = "";
  toggleComplete = false;
  //  }

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
  
  sensorValue = analogRead(analogInPin);
  //     Serial.print("sensor ");
  //     Serial.println(sensorValue);

  // read the analog in value:
  //      Serial.print("B"); // begin character
  //      Serial.print(sensorValue);
  //      Serial.print("E"); // end character
  //      prevValue = sensorValue;

  delay(2000); // give the Arduino some breathing room.
}

int stringToInt() {
  char charHolder[inputString.length() + 1];
  inputString.toCharArray(charHolder, inputString.length() + 1);
  inputString = "";
  int _recievedVal = atoi(charHolder);
  return _recievedVal;
}
