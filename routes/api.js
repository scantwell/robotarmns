var express = require('express');
var child_process = require('child_process');

var router = express.Router();
//var serialpy = child_process.exec('serial.py');

router.get('/', function(req, res) {
  // API help page or something?
  res.send('success');
});

router.get('/ping', function(req, res)  {
  /*
  var serialpy = child_process.exec('serial.py');
  serialpy.stdin.write('ping');
  serialpy.stdout.on('data', function(data) {
    serialpy.kill();
    res.send(data);
  });
  */
  res.send('success');
});

router.post('/moveforward', function(req, res) {
  res.send('success');
});

router.post('/movebackward', function(req, res) {
  res.send('success');
});

router.post('/turnleft', function(req, res) {
  res.send('success');
});

router.post('/turnright', function(req, res) {
  res.send('success');
});

router.post('/openclaw', function(req, res) {
  res.send('success');
});

router.post('/closeclaw', function(req, res) {
  res.status(500).send('failure');
});

router.post('/raisearm', function(req, res) {
  res.send('success');
});

router.post('/lowerarm', function(req, res) {
  res.send('success');
});

module.exports = router;
