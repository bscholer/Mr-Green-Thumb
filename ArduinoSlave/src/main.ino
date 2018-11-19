#include "Arduino.h"
#include "dht.h"

//The default grow light pin connections
#define GROW_LIGHT 2

dht DHT;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

//functions:
//float getTempC(int sensorPin, int powerPin) - returns temperature in Celcius
//float getTempF(int sensorPin, int powerPin) - returns temperature in Fahrenheit
//float getHumidity(int sensorPin, int powerPin) - returns the humidity (0-100)
//float getMoist(int sensorPin, int powerPin, float calibrationCoef) - returns the soil moisture (0-100), using the calibrationCoef.
//float getCalibrationCoef(int sensorPin, int powerPin) - returns the calibration coefficent.
//void water(int pin1, int pin2, int ms) - turns <pin1> HI, and <pin2> LO for <ms>
//void setBrightness(int brightness) - outputs appropriate PWM signal on defined GROW pins to obtain <brightness> (0-100%).
//void setBrightnessPins(int posPin, int negPin, int brightness) - outputs appropriate PWM signal on <posPin> and <negPin> to obtain <brightness> (0-100%).
// NOT A THING void makePinNeg(int pin) - makes <pin> LOW

void setup () {
	Serial.begin(115200);
	Serial.println("<Arduino ready>");
	Serial.setTimeout(2000);
	pinMode(GROW_LIGHT, OUTPUT);
	digitalWrite(GROW_LIGHT, LOW);
}

int percentBrightnessToPwm(int brightness) {
	int val = map(brightness, 0, 100, 0, 255);
	return val;
}

void setBrightnessPins(int posPin, int negPin, int brightness) {
	pinMode(posPin, OUTPUT);
	pinMode(negPin, OUTPUT);
	digitalWrite(negPin, LOW);
	analogWrite(posPin, percentBrightnessToPwm(brightness));
}

//TODO write this
void setBrightness(int brightness) {
}

float getTempC(int sensorPin) {
	int status;
	float ret;

	do {
		status = DHT.read11(sensorPin);
	} while (DHT.humidity > 100 || DHT.humidity < 0 || DHT.temperature < -100);
	ret = DHT.temperature;

	return ret;
}

float getTempF(int sensorPin) {
	int status;
	float ret;

	do {
		status = DHT.read11(sensorPin);
	} while (DHT.humidity > 100 || DHT.humidity < 0 || DHT.temperature < -100);
	ret = ((DHT.temperature * 9 / 5) + 32);

	return ret;
}

float getHumidity(int sensorPin) {
	int status;
	float ret;

	do {
		status = DHT.read11(sensorPin);
	} while (DHT.humidity > 100 || DHT.humidity < 0 || DHT.temperature < -100);
	ret = DHT.humidity;

	return ret;
}

float getCalibratedSoilMoisture(int sensorPin, int powerPin, float calibrationCoef) {
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);
	delay(200);

	ret = map(analogRead(sensorPin), 1023, 0, 0, 100);

	delay(200);
	digitalWrite(powerPin, LOW);
	return ret * calibrationCoef;
}

float getMoist(int sensorPin, int powerPin) {
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);
	delay(200);

	ret = analogRead(sensorPin);

	delay(200);
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
		Serial.print("Rec: ");
		Serial.println(input);

		if (input.indexOf("getTempC(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getTempC(sensorPin));
		}
		else if (input.indexOf("getTempF(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getTempF(sensorPin));
		}
		else if (input.indexOf("getHumidity(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getHumidity(sensorPin));
		}
		else if (input.indexOf("getMoist(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getMoist(sensorPin, powerPin));
		}
		else if (input.indexOf("water(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int pin1 = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int pin2 = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int ms = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(water(pin1, pin2, ms));
		}
		else if (input.indexOf("setBrightness(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int brightness = args.substring(0, args.indexOf(",")).toInt();
			setBrightness(brightness);
			Serial.println("0");
		}
		else if (input.indexOf("setBrightnessPins(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int posPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int negPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int brightness = args.substring(0, args.indexOf(",")).toInt();
			setBrightnessPins(posPin, negPin, brightness);
			Serial.println("0");
		}
		else {
			Serial.println("Invalid entry, please try again");	
		}
	}
}
