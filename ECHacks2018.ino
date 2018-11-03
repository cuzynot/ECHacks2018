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

// the desired humidity set by the user
int userHumidity = 60;

void setup() {
  Serial.begin(9600);
  pinMode(A5, OUTPUT);// Set A5 as OUTPUT
  aqs.init(A0);
}

void loop() {
  aq = aqs.slope();

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

  // print temperature
  Serial.print("temperature:"); 
  Serial.print((int)temperature); 
  Serial.println(" *C, "); 

  // print humidity
  Serial.print("humidity:"); 
  Serial.print((int)humidity); 
  Serial.println(" H");

  if((int)humidity < userHumidity){
    Serial.print("Less than ");
    digitalWrite(A5, HIGH);
  } else {
    Serial.println("More than ");
    digitalWrite(A5, LOW);
  }
  Serial.println(userHumidity);

  Serial.println();
  
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
