#include "dht.h"

#define TEMP_HUM_SENSOR_PIN 2
#define SOIL_MOIST_SENSOR_PIN A0

dht DHT;

//functions:
//float tempC() - returns temperature in Celcius
//float tempF() - returns temperature in Fahrenheit
//float humidity() - returns the humidity (0-100)
//float soilMoisture(int pin) - returns the soil moisture (0-100)
//void water(int pin1, int pin2, int ms) - turns <pin1> HI, and <pin2> LO for <ms>

void setup () {
    Serial.begin(115200);
    Serial.setTimeout(100);
}

float tempC() {
  int status;
      do {
        status = DHT.read11(TEMP_HUM_SENSOR_PIN);
      } while (DHT.humidity > 100 && DHT.humidity < 0);

      return DHT.temperature;
}

float tempF() {
  int status;
  do {
    status = DHT.read11(TEMP_HUM_SENSOR_PIN);
  } while (DHT.humidity > 100 && DHT.humidity < 0);

  return ((DHT.temperature * 9 / 5) + 32);
}

float humidity() {
  int status;
  do {
    status = DHT.read11(TEMP_HUM_SENSOR_PIN);
  } while (DHT.humidity > 100 && DHT.humidity < 0);

  return DHT.humidity;
}

float soilMoisture(int pin) {
  return map(analogRead(pin), 1023, 0, 0, 100);
}

short water(int pin1, int pin2, int ms) {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  delay(ms);
  digitalWrite(pin1, LOW);
  return 0;
}

void loop () {
  if (Serial.available() > 0) {
    String input = Serial.readString();

    if (input.indexOf("tempC(") >= 0) {
      Serial.println(tempC());
    }
    else if (input.indexOf("tempF(") >= 0) {
      Serial.println(tempF());
    }
    else if (input.indexOf("humidity(") >= 0) {
      Serial.println(humidity());
    }
    else if (input.indexOf("soilMoisture(") >= 0) {
      String arg1 = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
      Serial.println(soilMoisture(arg1.toInt()));
    }
    else if (input.indexOf("water(") >= 0) {
      String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
        int pin1 = args.substring(0, args.indexOf(",")).toInt();
        args = args.substring(args.indexOf(",") + 1);
        args.trim();
        int pin2 = args.substring(0, args.indexOf(",")).toInt();
        args = args.substring(args.indexOf(",") + 1);
        args.trim();
        int ms = args.substring(0, args.indexOf(",")).toInt();
        Serial.println(water(pin1, pin2, ms));
    }
  }
}
