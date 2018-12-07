var buffer = require('./buffer')
module.exports = {
  // 正在排队的用户
  waitinglist : [],
  // 对战队列，包括了对战双方用户名信息，和对战id
  battle : [],
  // 添加到等待队列
  addWaitinglist(username){
    if (this.waitinglist.length == 0) {
      this.waitinglist.push(username)
      return
    }
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
    let id = (new Date()).valueOf() % 1000000000
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
  eventHandle(data){
    data.id = parseInt(data.id)
    data.positionX = Number(data.positionX)
    data.positionY = Number(data.positionY)
    data.monsterType = Number(data.monsterType)
    buffer.addEvent(data.id, data)
  },
  // 轮询获取新事件
  getNewEvent(id, LSN){
    return buffer.getEvent(id, LSN)
  },
  // 对战开始
  battleBegin(username){
    let data = null
    let side = 0
    this.battle.forEach(ele=>{
      if (ele.user1 == username) {
        data = ele.id
        side = 1
        return
      } else if (ele.user2 == username) {
        data = ele.id
        side = 2
        return
      }
    })
    // console.log(data, side)
    return {
      id: data,
      side: side
    }
  },
  // 信息同步
  syncHandle(){
    // todo
  },
  // 结束对战
  endBattle(username){
    // todo
    // 清理缓冲池
    let data = null
    let index = -1
    this.battle.forEach(ele=>{
      index ++;
      if (ele.user1 == username || ele.user2 == username) {
        data = ele;
        return;
      }
    })
    this.battle.splice(index, 1)
    return data
  }
}
