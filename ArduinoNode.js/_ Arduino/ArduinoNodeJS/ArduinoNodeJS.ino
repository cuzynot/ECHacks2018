#include "AirQuality.h"
#include "Arduino.h"
#include <SimpleDHT.h>

// air quality sensor
AirQuality aqs;
int aq = -1;

// temp and humidity sensor
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

// strings
String inputString = "0";         // a string to hold incoming data
String prevString = "0";

// bools
boolean buttonClicked = false;  // whether the string is complete
boolean slided = false;

// int prevValue = 0;          // previous value from the potmeter

// the desired humidity set by the user
int userHumidity = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);

  // ultrasonic mist
  pinMode(A5, OUTPUT);

  // init air quality sensor
  aqs.init(A0);
}

void loop() {
  // Recieve data from Node and write it to a String
  for (int i = 0; i < 100; i++) {
    if (Serial.available() > 0) {
      char c = (char)Serial.read();
      if (c == 'E') { // end character for led
        buttonClicked = true;
      } else if (c == 'P') {
        Serial.println("cur is p");
        slided = true;
      } else {
        inputString += c;
      }
    }

    if (buttonClicked || slided) break;
    delay(20);
  }

  // get air quality
  aq = aqs.slope();
  // delay(500);

  // get temp and humidity
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;

  // if there's an error
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    // Serial.print("Read DHT11 failed, err=");
    // Serial.println(err);
    delay(1000);
    return;
  }

  if (slided) {
      userHumidity = inputString.toInt();
      Serial.println("new humidity ");
      Serial.println(userHumidity);
  } else {
    if (inputString == "0") {
      digitalWrite(A5, 0);
      prevString = "0";
    } else if (inputString == "" && prevString == "0"){
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

      if (inputString != ""){
        prevString = inputString;
      }
      Serial.println(userHumidity);
    }
  }
  
  inputString = "";
  buttonClicked = false;
  slided = false;

  //  }

  //  // Toggle LED 13
  //  if(!Serial.available() && buttonClicked == true)
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
  //    buttonClicked = false;
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


  // sensorValue = analogRead(analogInPin);
  //     Serial.print("sensor ");
  //     Serial.println(sensorValue);

  // read the analog in value:
  //      Serial.print("B"); // begin character
  //      Serial.print(sensorValue);
  //      Serial.print("E"); // end character
  //      prevValue = sensorValue;

  delay(2000); // give the Arduino some breathing room.
  Serial.println();
}

int stringToInt() {
  char charHolder[inputString.length() + 1];
  inputString.toCharArray(charHolder, inputString.length() + 1);
  inputString = "";
  int _recievedVal = atoi(charHolder);
  return _recievedVal;
}

ISR(TIMER2_OVF_vect) {
  if (aqs.counter == 122) { //set 2 seconds as a detected duty
    aqs.last_vol = aqs.first_vol;
    aqs.first_vol = analogRead(A0); // change this value if you use another A port
    aqs.counter = 0;
    aqs.timer_index = 1;
    PORTB = PORTB ^ 0x20;
  } else {
    aqs.counter++;
  }
}
