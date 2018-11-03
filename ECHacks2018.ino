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

void setup() {
  Serial.begin(9600);
  pinMode(A5, OUTPUT);// Set A5 as OUTPUT
  aqs.init(A0);
}

void loop() {
  aq = aqs.slope();
  if (aq >= 0){
    if (aq == 0) {
      Serial.println("High pollution! Force signal active");
    } else if (aq == 1) {
      Serial.println("High pollution!");
    } else if (aq == 2) {
      Serial.println("Low pollution!");
    } else if (aq == 3) {
      Serial.println("Fresh air");
    } else {
      Serial.print("aq ");
      Serial.println(aq);
    }
  } else {
    Serial.println("heh?");
  }

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
  
  Serial.print((int)temperature); 
  Serial.print(" *C, "); 
  Serial.print((int)humidity); 
  Serial.println(" H");

  if((int)humidity < 60){
    Serial.println("Less than 60 H");
    digitalWrite(A5, HIGH);
  } else {
    Serial.println("More than 60 H");
    digitalWrite(A5, LOW);
  }
  
  // DHT11 sampling rate is 2HZ.
  delay(2000);
}

ISR(TIMER2_OVF_vect) {
    if(aqs.counter == 122) {//set 2 seconds as a detected duty
        aqs.last_vol = aqs.first_vol;
        aqs.first_vol = analogRead(A0); // change this value if you use another A port
        aqs.counter = 0;
        aqs.timer_index = 1;
        PORTB = PORTB^0x20;
    } else {
        aqs.counter++;
    }
}
