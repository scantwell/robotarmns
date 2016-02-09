var express = require('express');
var router = express.Router();

var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var serialPort = new SerialPort("/dev/ttyUSB1", {
  baudrate: 9600,
  parser: serialport.parsers.byteLength(4)
});

var callbackStack = [];
serialPort.on("open", function () {
  console.log('open');
  serialPort.on('data', function(data) {
    if (callbackStack.length > 0) {
      var callback = callbackStack.shift();
      callback(data);
    }
  });
});

// New Api Routes
router.post('/arm', function(req, res) {
  var commandId = 1;
  var length = 2;
  var centimeters = req.body.centimeters;

  var buffer = new Buffer(length + 2);
  buffer.writeUInt8(commandId, 0);
  buffer.writeUInt8(length, 1);
  buffer.writeUInt16LE(centimeters, 2);

  serialPort.write(buffer);
  callbackStack.push(function(data) {
    console.log("Value Read:");
    console.log(data);
    res.send('success');
  });

});

router.post('/claw', function(req, res) {
  var commandId = 2;
  var length = 2;
  var centimeters = req.body.centimeters;


  var buffer = new Buffer(length + 2);
  buffer.writeUInt8(commandId, 0);
  buffer.writeUInt8(length, 1);
  buffer.writeUInt16LE(centimeters, 2);

  serialPort.write(buffer);
  callbackStack.push(function(data) {
    console.log("Value Read:");
    console.log(data);
    res.send('success');
  });
});

router.post('/move', function(req, res) {
  var commandId = 3;
  var length = 3;
  var direction = req.body.movedirection;
  var centimeters = req.body.centimeters;

  var buffer = new Buffer(length + 2);
  buffer.writeUInt8(commandId, 0);
  buffer.writeUInt8(length, 1);
  buffer.writeUInt8(direction, 2);
  buffer.writeUInt16LE(centimeters, 3);

  serialPort.write(buffer);
  callbackStack.push(function(data) {
    console.log("Value Read:");
    console.log(data);
    res.send('success');
  });
});

router.post('/rotate', function(req, res) {
  var commandId = 4;
  var length = 3;
  var direction = req.body.rotatedirection;
  var degrees = req.body.degrees;

  var buffer = new Buffer(length + 2);
  buffer.writeUInt8(commandId, 0);
  buffer.writeUInt8(length, 1);
  buffer.writeUInt8(direction, 2);
  buffer.writeUInt16LE(degrees, 3);

  serialPort.write(buffer);
  callbackStack.push(function(data) {
    console.log("Value Read:");
    console.log(data);
    res.send('success');
  });
});

module.exports = router;
