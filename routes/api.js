var express = require('express');
var child_process = require('child_process');

var router = express.Router();
var serialpy = child_process.exec('serial.py');

router.get('/', function(req, res) {
  // API help page or something?
  res.send('success');
});

router.get('/ping', function(req, res)  {
  var serialpy = child_process.exec('serial.py');
  serialpy.stdin.write('ping');
  serialpy.stdout.on('data', function(data) {
    serialpy.kill();
    res.send(data);
  });
});

router.post('/moveforward', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/movebackward', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/turnleft', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/turnright', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/openclaw', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/closeclaw', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/raisearm', function(req, res) {
  serialpy.stdin.write('Something!');
  res.send('success');
});

router.post('/lowerarm', function(req, res) {
  var serialpy = child_process.exec('serial.py');
  serialpy.stdin.write('Something!');
  serialpy.stdout.on('data', function(data) {
    serialpy.kill();
    res.send(data);
  });
});

module.exports = router;
