var buffer = require('./buffer')
module.exports = {
  // 正在排队的用户
  waitinglist : [],
  // 对战队列，包括了对战双方用户名信息，和对战id
  battle : [],
  // 添加到等待队列
  addWaitinglist(username){
    if (this.waitinglist.length == 0)
      this.waitinglist.push(username)
    else if (this.waitinglist.length == 1 && 
      this.waitinglist[0] != username && !this.userInBattle(username)) {
      user1 = this.waitinglist[0]
      this.createBattle(user1, username)
      this.waitinglist = []
      return
    }
    console.log('fail to wait')
  },
  // 创建对战
  createBattle(user1, user2){
    let id = this.battle.length
    this.battle.push({
      id: id,
      user1: user1,
      user2: user2
    })
    console.log(this.battle)
    buffer.battleBuffer.push({
      id: id,
      battleMessage: []
    })
    buffer.eventBuffer.push({
      id: id,
      events: []
    })
  },
  // 查询用户是否在对战中
  userInBattle(username) {
    let data = false
    this.battle.forEach(ele=>{
      if (ele.user1 == username || ele.user2 == username) {
        data = true
        return
      }
    })
    return data
  },
  // 处理新事件
  eventHandle({id, event}){
    buffer.addEvent(id, event)
  },
  // 轮询获取新事件
  getNewEvent(id, LSN){
    return buffer.getEvent(id, LSN)
  },
  // 对战开始
  battleBegin(username){
    let data = null
    this.battle.forEach(ele=>{
      if (ele.user1 == username || ele.user2 == username) {
        data = ele.id
      }
    })
    return data
  },
  // 信息同步
  syncHandle(){
    // todo
  },
  // 结束对战
  endBattle(){
    // todo
    // 清理缓冲池
  }
}
