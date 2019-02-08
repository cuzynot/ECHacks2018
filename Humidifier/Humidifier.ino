#include "AirQuality.h" // COMMENT OUT ALL SERIAL PRINTING IN .cpp FILE
#include "Arduino.h"
#include <SimpleDHT.h>

// air quality sensor
AirQuality aqs;
int aq = -1;

// temp and humidity sensor
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

// strings
String inputString = ""; // a string to hold incoming data

// bools
boolean buttonClicked = false; // whether the string is complete
boolean slided = false;

// the desired humidity set by the user
int userHumidity = 0;

// init values
byte temperature = 0;
byte humidity = 0;

// error value
int err = SimpleDHTErrSuccess;

void setup() {
  // initialize serial:
  Serial.begin(9600);

  // ultrasonic mist
  pinMode(A5, OUTPUT);

  // init air quality sensor
  aqs.init(A0);
}

void loop() {
  // reset string and slided values
  inputString = "";
  slided = false;

  // Recieve data from JS and write it to a String
  for (int i = 0; i < 100; i++) {
    if (Serial.available() > 0) {
      char c = (char)Serial.read();
      if (c == 'E') { // end character for button
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

  // RECEIVE VALUES
  aq = aqs.slope();
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(1000);
    // return;
  }

  // if Myst is toggled on
  if (buttonClicked) {
    if ((int)humidity < userHumidity) {
      // turn Myst on if cur humidity is lower than desired humidity
      digitalWrite(A5, HIGH);
    } else {
      // turn Myst off if cur humidity exceeds desired humidity
      digitalWrite(A5, LOW);
    }
  } else {
    // turn Myst off
    digitalWrite(A5, LOW);
  }

  // print temperature
  Serial.print("T"); // begin character
  Serial.print(temperature);
  Serial.print("P"); // end character

  // print humidity
  Serial.print("H");
  Serial.print(humidity);
  Serial.print("M");

  // print air quality
  Serial.print("A");
  Serial.print(aq);
  Serial.print("Q");

  delay(2000);
  Serial.println();
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
