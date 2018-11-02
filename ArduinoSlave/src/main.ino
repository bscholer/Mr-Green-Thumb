#include "Arduino.h"
#include "dht.h"

#define TEMP_HUM_SENSOR_PIN 2
#define SOIL_MOIST_SENSOR_PIN A0

dht DHT;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

//functions:
//float tempC(int sensorPin, int powerPin) - returns temperature in Celcius
//float tempF(int sensorPin, int powerPin) - returns temperature in Fahrenheit
//float humidity(int sensorPin, int powerPin) - returns the humidity (0-100)
//float soilMoisture(int sensorPin, int powerPin) - returns the soil moisture (0-100)
//void water(int pin1, int pin2, int ms) - turns <pin1> HI, and <pin2> LO for <ms>

void setup () {
	Serial.begin(115200);
	Serial.setTimeout(2000);
	Serial.write("<Arduino is ready>");
}

float tempC(int sensorPin, int powerPin) {
	int status;
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);

	do {
		status = DHT.read11(sensorPin);
	} while (DHT.humidity > 100 || DHT.humidity < 0 || DHT.temperature < -100);
	ret = DHT.temperature;

	digitalWrite(powerPin, LOW);
	return ret;
}

float tempF(int sensorPin, int powerPin) {
	int status;
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);

	do {
		status = DHT.read11(sensorPin);
	} while (DHT.humidity > 100 || DHT.humidity < 0 || DHT.temperature < -100);
	ret = ((DHT.temperature * 9 / 5) + 32);

	digitalWrite(powerPin, LOW);
	return ret;
}

float humidity(int sensorPin, int powerPin) {
	int status;
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);

	do {
		status = DHT.read11(sensorPin);
	} while (DHT.humidity > 100 || DHT.humidity < 0 || DHT.temperature < -100);
	ret = DHT.humidity;

	digitalWrite(powerPin, LOW);
	return ret;
}

float soilMoisture(int sensorPin, int powerPin) {
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);
	delay(50);

	Serial.print("moist: ");
	Serial.println(analogRead(sensorPin));
	ret = map(analogRead(sensorPin), 1023, 200, 0, 100);

	delay(50);
	digitalWrite(powerPin, LOW);
	return ret;
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
	while(Serial.available()) {
		String input = Serial.readStringUntil('\r');

		if (input.indexOf("tempC(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(tempC(sensorPin, powerPin));
		}
		else if (input.indexOf("tempF(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(tempF(sensorPin, powerPin));
		}
		else if (input.indexOf("humidity(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(humidity(sensorPin, powerPin));
		}
		else if (input.indexOf("soilMoisture(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(soilMoisture(sensorPin, powerPin));
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
