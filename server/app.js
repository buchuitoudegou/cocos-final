const express = require('express')
const bodyParser = require('body-parser')
const path = require('path')
const logger = require('morgan')
const router = require('./router')

const app = express()

// app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({'extended':'true'}));
app.use(express.static(path.join(__dirname, 'public')));
app.get('/', (req, res)=>{
  res.sendFile(path.resolve(__dirname, 'public/test.html'))
})
app.use('/api', router);

app.use(function(err, req, res, next) {
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};
  res.status(err.status || 500).send(err.message);
});

module.exports = app;