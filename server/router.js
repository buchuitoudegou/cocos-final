const express = require('express');
const router = express.Router();
const model = require('./model')
const battle = require('./battle/index')

// 登陆
router.post('/login', (req, res)=> {
  let message = model.userLogin(req.body)
  res.send(message)
  res.end()
})

// 注册
router.post('/register', (req, res)=>{
  let message = model.userRegister(req.body)
  res.send(message)
  res.end()
})
// 对战申请
router.get('/battle/waiting/*', (req, res)=>{
  let username = req.path.split('/')[3]
  battle.addWaitinglist(username)
  res.send({status: 'ok'})
})
// 对战开始轮询
router.get('/battle/begin/*', (req, res)=>{
  let username = req.path.split('/')[3]
  let id = battle.battleBegin(username)
  if (id != null) {
    res.send({status: 'begin', id: id})
  } else {
    res.send({status: 'waiting'})
  }
})
// 对战事件发出
router.post('/battle/event', (req, res)=>{
  let data = req.body
  battle.eventHandle(data)
  res.send({status: 'ok'})
})
// 对战事件轮询
router.get('/battle/event/battle-id/*/LSN/*', (req, res)=>{
  let data = req.path.split('/')
  let id = data[4]
  let LSN = data[6]
  let event = battle.getNewEvent(id, LSN)
  res.send({status: 'ok', event: event})
})
module.exports = router