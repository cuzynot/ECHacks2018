var socketServer;
var fs = require('fs');
var http = require('http');
var socketio = require('socket.io');
var url = require("url");
var SerialPort = require('serialport');
// var portName = '/dev/cu.usbmodem14101';
var portName = 'COM1';

var temp, humi, airq;

var serialPort

// var sp = new SerialPort(portName,{baudRate:baud});

// handle contains locations to browse to (vote and poll); pathnames.
function startServer(route,handle,debug) {
	// on request event
	function onRequest(request, response) {
	  // parse the requested url into pathname. pathname will be compared
	  // in route.js to handle (var content), if it matches the a page will 
	  // come up. Otherwise a 404 will be given. 
	  var pathname = url.parse(request.url).pathname; 
	  console.log("Request for " + pathname + " received");
	  var content = route(handle,pathname,response,request,debug);
	}
	
	var httpServer = http.createServer(onRequest).listen(1337, function(){
		console.log("Listening at: http://localhost:1337");
		console.log("Server is up");
	}); 
	serialListener(debug);
	initSocketIO(httpServer,debug);
}

function initSocketIO(httpServer,debug) {
	socketServer = socketio.listen(httpServer);
	if(debug == false){
		socketServer.set('log level', 1); // socket IO debug off
	}
	socketServer.on('connection', function (socket) {
		console.log("user connected");
		socket.emit('onconnection', {pollOneValue:temp});

		setInterval(function () {
			socket.emit('updateData',temp);
			socket.emit('updateData',humi);
			socket.emit('updateData',airq);
		}, 3000);
		// socketServer.on('update', function(data) {
		// 	socket.emit('updateData',{pollOneValue:data});
		// });

		socket.on('buttonval', function(data) {
			serialPort.write(data + 'E');
			console.log("button " + data);
		});

		socket.on('sliderval', function(data) {
			serialPort.write(data + 'P');
			console.log("slide " + data);
		});
	});
}

// Listen to serial port
function serialListener(debug) {
	var receivedData = "";
	serialPort = new SerialPort(portName, {
		baudRate: 9600,
        // defaults for Arduino serial communication
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false
    });

	serialPort.on("open", function() {
		console.log('open serial communication');
        // Listens to incoming data
        serialPort.on('data', function(data) {
        	receivedData += data.toString();
        	if (receivedData.indexOf("T") >= 0 && receivedData.indexOf("P") >= 0) {
        		temp = receivedData.substring(receivedData.indexOf('T'), receivedData.indexOf('P'));
        		// console.log("temp " + temp);
        		receivedData = receivedData.substring(receivedData.indexOf('P') + 1);

        	} else if (receivedData.indexOf("H") >= 0 && receivedData.indexOf("M") >= 0) {
        		humi = receivedData.substring(receivedData.indexOf('H'), receivedData.indexOf('M'));
        		// console.log("humi " + humi);
        		receivedData = receivedData.substring(receivedData.indexOf('M') + 1);

        	} else if (receivedData.indexOf("A") >= 0 && receivedData.indexOf("Q") >= 0) {
        		airq = receivedData.substring(receivedData.indexOf('A'), receivedData.indexOf('Q'));
        		// console.log("airq " + airq);
        		receivedData = receivedData.substring(receivedData.indexOf('Q') + 1);
        	}
	        // send the incoming data to browser with websockets.
		    // socketServer.emit('update', sendData);
     	});
    }); 
}

exports.start = startServer;