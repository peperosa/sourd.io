// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
//#include <DHT_U.h>

// pin connected to DH22 data line
#define DATA_PIN 4
#define DHTTYPE DHT22

// create DHT22 instance
// DHT_Unified dht(DATA_PIN, DHTTYPE);
 DHT dht(DATA_PIN, DHTTYPE);

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("sourd-dot-io-dot-tC");
AdafruitIO_Feed *temperatureF = io.feed("sourd-dot-io-dot-tF");
AdafruitIO_Feed *humidity = io.feed("sourd-dot-io-dot-h");
AdafruitIO_Feed *distance = io.feed("sourd-dot-io-dot-d");

/*************************** Ultrasonic sensor *******************************/
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

int Vo;
float Vo_scal, logR2, R2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
long duration; 
float distancecm;

void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // start the serial connection
  Serial.begin(115200);
  //Serial.begin(74880); 
  // wait for serial monitor to open
  while(! Serial);

  // initialize dht22
  dht.begin();

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

    // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(30000);

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // sensors_event_t event;
  // dht.temperature().getEvent(&event);
  // dht.humidity().getEvent(&event);
  
//  Vo = analogRead(THERMISTORPIN);
//  Vo_scal = Vo; //*(5.0/3.3); // or Vo if V_in and V_analogref are the same...
//  R2 =  SERIESRESISTOR/(1023.0/Vo_scal-1); 
//  logR2 = log(R2);
//  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
//  float celsius = T - 273.15;
//  float fahrenheit = (celsius * 1.8) + 32;

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity_percentage = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float celsius = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fahrenheit = dht.readTemperature(true);
  
  // save fahrenheit (or celsius) to Adafruit IO
  temperature->save(celsius);
  temperatureF->save(fahrenheit);
  // save humidity to Adafruit IO
  humidity->save(humidity_percentage);
      
  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");
  
  Serial.print("humidity: ");
  Serial.print(humidity_percentage);
  Serial.println("%");

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  if (isnan(humidity_percentage) || isnan(celsius) || isnan(fahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  // Calculating the distance
  distancecm = duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distancecm);

  // save distance to Adafruit IO
  distance->save(distancecm);
  

}
