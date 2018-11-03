#include <SimpleDHT.h>

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

void setup() {
  Serial.begin(9600);
  pinMode(A5, OUTPUT);// Set A5 as OUTPUT
}

void loop() {  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
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
    Serial.println("Less than 50 H");
    digitalWrite(A5, HIGH);
  } else {
    Serial.println("More than 50 H");
    digitalWrite(A5, LOW);
  }
  
  // DHT11 sampling rate is 1HZ.
  delay(2000);
}
