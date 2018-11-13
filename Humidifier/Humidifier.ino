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
String inputString = "";         // a string to hold incoming data
String prevString = "";

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
  
  Serial.print("1");
  // Recieve data from Node and write it to a String
  for (int i = 0; i < 100; i++) {
    if (Serial.available() > 0) {
      char c = (char)Serial.read();
      if (c == 'E') { // end character for led
        buttonClicked = !buttonClicked;
        break;
      } else if (c == 'P') {
        userHumidity = inputString.toInt();
      } else {
        inputString += c;
      }
    }

    if (slided) break;
    delay(20);
  }

  Serial.print("1");
  
  // get air quality
  aq = aqs.slope();
  // delay(500);

  Serial.print("2");

  // get temp and humidity
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;

  Serial.print("3");

  // if there's an error
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
//    Serial.print("Read DHT11 failed, err=");
//    Serial.println(err);
    delay(1000);
    // return;
  }

  Serial.print("4");
  
  if (buttonClicked) {
//    // print temperature
//    Serial.print("temperature:");
//    Serial.print((int)temperature);
//    Serial.println(" *C, ");
//
//    // print humidity
//    Serial.print("humidity:");
//    Serial.print((int)humidity);
//    Serial.println(" H");

    if ((int)humidity < userHumidity) {
      digitalWrite(A5, HIGH);
    } else {
      digitalWrite(A5, LOW);
    }
//    Serial.println(userHumidity);
  } else {
    digitalWrite(A5, LOW);
  }

  Serial.print("5");

  inputString = "";
  slided = false;

  // sensorValue = analogRead(analogInPin);
  //  Serial.print("sensor ");
  //  Serial.println(sensorValue);

  // testing
  temperature = random(-30, 40);
  humidity = random(30, 60);
  aq = random(0, 700);

  Serial.print("T"); // begin character
  Serial.print(temperature);
  Serial.print("P"); // end character

  Serial.print("H"); // begin character
  Serial.print(humidity);
  Serial.print("M"); // end character

  Serial.print("A"); // begin character
  Serial.print(aq);
  Serial.print("Q"); // end character

  delay(2000);
  Serial.println();

  Serial.print("6");
}

//int stringToInt() {
//  char charHolder[inputString.length() + 1];
//  inputString.toCharArray(charHolder, inputString.length() + 1);
//  inputString = "";
//  int _recievedVal = atoi(charHolder);
//  return _recievedVal;
//}

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
