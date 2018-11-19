const delay = require("delay");
const fs = require("fs");
const SerialPort = require("serialport");
const timeout = 100;
const scale = (num, in_min, in_max, out_min, out_max) => {
	return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

let ready = false;
let slave = new SerialPort("/dev/ttyACM0", { baudRate: 115200 });
let config = JSON.parse(fs.readFileSync("config.json", 'utf8'));
let message = "";
let messageStartMillis;
let curMoistSensor;

slave.on('open', function() {
	console.log("port opened");
});

slave.on('readable', function () {
	console.log('Datareadable: ', slave.read());
});

slave.on('data', function(data) {
	data = data.toString('utf8');
	console.log(data);
	if (data.indexOf(">") !== -1) {
		loop();
	}
	/*
	if (messageStartMillis) {
		if (new Date().getMilliseconds() < messageStartMillis + timeout) {
			console.log("Within timeout");
			message = message + data.toString('utf8');
			console.log(message);
		} else {
			if (message) newData(message);
			messageStartMillis = 0;
			message = "";
		}
	}
	else {
		message = data.toString('utf8');
		messageStartMillis = new Date().getMilliseconds();
	}
	*/
});

function loop() {
	getMoisture();
}

function getMoisture() {
	let vals = [];
	config.moistureSensors.forEach( function (sensor) {
		curMoistSensor = sensor;
		setTimeout(sendQuery, 1000, sensor);
	});
}

function sendQuery(sensor) {
	console.log(sensor);
	slave.write("getMoist(" + sensor.sensorPin + "," + sensor.powerPin + ")\r");
}
