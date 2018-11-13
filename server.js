var socketServer;
var SerialPort;
var portName = '/dev/cu.usbmodem14101';
// var portName = 'COM3'; 
var sendData = "";

var fs = require('fs'),
http = require('http'),
socketio = require('socket.io'),
url = require("url"),
SerialPort = require('serialport')

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
		socket.emit('onconnection', {pollOneValue:sendData});

		setInterval(function () {
			socket.emit('updateData',sendData);
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
        	if (receivedData.indexOf("B") >= 0 && receivedData.indexOf("E") >= 0) {
        		sendData = receivedData.substring(receivedData.indexOf('B') + 1, receivedData.indexOf('E'));
        		console.log("temp " + sendData);
        		receivedData = receivedData.substring(receivedData.indexOf('E') + 1);
        	} else if (receivedData.indexOf("A") >= 0 && receivedData.indexOf("C") >= 0) {
        		sendData = receivedData.substring(receivedData.indexOf('A') + 1, receivedData.indexOf('C'));
        		console.log("humi " + sendData);
        		receivedData = receivedData.substring(receivedData.indexOf('C') + 1);
        	} else if (receivedData.indexOf("F") >= 0 && receivedData.indexOf("G") >= 0) {
        		sendData = receivedData.substring(receivedData.indexOf('F') + 1, receivedData.indexOf('G'));
        		console.log("airq " + sendData);
        		receivedData = receivedData.substring(receivedData.indexOf('G') + 1);
        	}
	        // send the incoming data to browser with websockets.
	     //    if (sendData !== "") {
		    //     // console.log("sent " + sendData);
		    //     socketServer.emit('update', sendData);
		    // }
     	});
    }); 
}

exports.start = startServer;