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
//#include <DHT.h>
//#include <DHT_U.h>

// pin connected to DH22 data line
// #define DATA_PIN 2

// create DHT22 instance
//DHT_Unified dht(DATA_PIN, DHT11);

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("sourd-dot-io-dot-tC");
AdafruitIO_Feed *temperatureF = io.feed("sourd-dot-io-dot-tF");
AdafruitIO_Feed *humidity = io.feed("sourd-dot-io-dot-h");


int Vo;
float Vo_scal, logR2, R2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // initialize dht22
  //dht.begin();

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

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  sensors_event_t event;
  // dht.temperature().getEvent(&event);

  Vo = analogRead(THERMISTORPIN);
  Vo_scal = Vo; //*(5.0/3.3); // or Vo if V_in and V_analogref are the same...
  R2 =  SERIESRESISTOR/(1023.0/Vo_scal-1); 
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  float celsius = T - 273.15;
  float fahrenheit = (celsius * 1.8) + 32;

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  // save fahrenheit (or celsius) to Adafruit IO
  temperature->save(celsius);
  temperatureF->save(fahrenheit);
  //dht.humidity().getEvent(&event);
  float humidity_percentage = 20; //event.relative_humidity;
  
  Serial.print("humidity: ");
  Serial.print(humidity_percentage);
  Serial.println("%");

  // save humidity to Adafruit IO
  humidity->save(humidity_percentage);

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(10000);

}
