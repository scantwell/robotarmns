var express = require('express');
var spawn = require('child_process').spawn;
var router = express.Router();

router.get('/', function(req, res) {
  // API help page or something?
  res.send('success');
});

router.get('/ping', function(req, res)  {
  res.send('success');
});

router.post('/moveforward', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'forward', '2500']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/movebackward', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'backward', '2500']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/turnleft', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'left', '1000']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/turnright', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'right', '1000']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/openclaw', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'claw', '5']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/closeclaw', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'claw', '95']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/raisearm', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'arm', '5']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

router.post('/lowerarm', function(req, res) {
  var py = spawn('python', ['/Users/omnia/WebstormProjects/robotarmns/python/robotarm.py', 'arm', '95']);
  py.on('close', function(exitCode) {
    if (exitCode == 0) {
      res.send('success')
    } else {
      res.status(500).send('failure')
    }
  });
});

module.exports = router;
