#include "Arduino.h"
#include "dht.h"

#define DEFAULT_GROW_LIGHT_PWM_PIN 12

dht DHT;
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

//functions:
//float getTempC(int sensorPin, int powerPin) - returns temperature in Celcius
//float getTempF(int sensorPin, int powerPin) - returns temperature in Fahrenheit
//float getHumidity(int sensorPin, int powerPin) - returns the humidity (0-100)
//float getSoilMoisture(int sensorPin, int powerPin, float calibrationCoef) - returns the soil moisture (0-100), using the calibrationCoef.
//float getCalibrationCoef(int sensorPin, int powerPin) - returns the calibration coefficent.
//void water(int pin1, int pin2, int ms) - turns <pin1> HI, and <pin2> LO for <ms>
//void setBrightness(int pwmPin, int brightness) - outputs appropriate PWM signal on <pwmPin> to obtain <brightness> (0-100%).

void setup () {
	Serial.begin(115200);
	Serial.setTimeout(2000);
	Serial.println("<Arduino is ready>");
	pinMode(DEFAULT_GROW_LIGHT_PWM_PIN, OUTPUT);
	analogWrite(DEFAULT_GROW_LIGHT_PWM_PIN, percentBrightnessToPwm(0));
}

float getCalibrationCoef(int sensorPin, int powerPin) {
	//the amount of samples to take and average
	float samples = 5;
	float sum = 0;
	for (int i = 0; i < samples; i++) {
		sum += getSoilMoisture(sensorPin, powerPin);
		delay(200);
	}
	return 100.0 / (sum / samples);
}

//TODO redo this for actual LED strip
int percentBrightnessToPwm(int brightness) {
	int val = map(brightness, 100, 0, 0, 255);
	return val;
}

void setBrightness(int pwmPin, int brightness) {
	pinMode(pwmPin, OUTPUT);
	analogWrite(pwmPin, percentBrightnessToPwm(brightness));
}

float getTempC(int sensorPin, int powerPin) {
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

float getTempF(int sensorPin, int powerPin) {
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

float getHumidity(int sensorPin, int powerPin) {
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

float getCalibratedSoilMoisture(int sensorPin, int powerPin, float calibrationCoef) {
	float ret;

	pinMode(powerPin, OUTPUT);
	digitalWrite(powerPin, HIGH);
	delay(50);

	ret = map(analogRead(sensorPin), 1023, 200, 0, 100);

	delay(50);
	digitalWrite(powerPin, LOW);
	return ret * calibrationCoef;
}

float getSoilMoisture(int sensorPin, int powerPin) {
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

		if (input.indexOf("getTempC(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getTempC(sensorPin, powerPin));
		}
		else if (input.indexOf("getTempF(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getTempF(sensorPin, powerPin));
		}
		else if (input.indexOf("getHumidity(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getHumidity(sensorPin, powerPin));
		}
		else if (input.indexOf("getSoilMoisture(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			float coef = args.substring(0, args.indexOf(",")).toFloat();
			Serial.println(getCalibratedSoilMoisture(sensorPin, powerPin, coef));
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
			int pwmPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int brightness = args.substring(0, args.indexOf(",")).toInt();
			setBrightness(pwmPin, brightness);
			Serial.print("Set brightness to ");
			Serial.print(brightness);
			Serial.println("%");
		}
		else if (input.indexOf("getCalibrationCoef(") >= 0) {
			String args = input.substring(input.indexOf("(") + 1, input.indexOf(")"));
			args.trim();
			int sensorPin = args.substring(0, args.indexOf(",")).toInt();
			args = args.substring(args.indexOf(",") + 1);
			args.trim();
			int powerPin = args.substring(0, args.indexOf(",")).toInt();
			Serial.println(getCalibrationCoef(sensorPin, powerPin), 5);
		}
	}
}
