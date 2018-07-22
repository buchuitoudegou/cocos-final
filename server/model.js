const fs = require('fs')

class fileRW {
  constructor() {}
  getUserByName(name) {
  let userInfo = fs.readFileSync('./server/user.json').toString()
  let users = JSON.parse(userInfo)
  let query = null 
    users.forEach(element => {
      if (element.name == name) {
        query = element
        return
      }
    });
    return query  
  }
  addNewUser(user) {
    let userInfo = fs.readFileSync('./server/user.json').toString()
    let users = JSON.parse(userInfo)
    user.point = 0
    users.push(user)
    fs.writeFileSync('./server/user.json', JSON.stringify(users))
  }
  updatePoint(username) {
    let userInfo = fs.readFileSync('./server/user.json').toString()
    let users = JSON.parse(userInfo)
    let index = -1
    users.forEach(ele=>{
      index ++;
      if (ele.username == username) {
        return;
      }
    })
    if (index != -1)
      users[index].point ++   
    fs.writeFileSync('./server/user.json', JSON.stringify(users))
  }
  addBattleLog(log) {
    let battleLog = fs.readFileSync('./server/battle.json').toString()
    battleLog = JSON.parse(battleLog)
    battleLog.push(log)
    fs.writeFileSync('./server/battle.json', JSON.stringify(battleLog))
  }
  getBattleLog(username) {
    let data = []
    let battleLog = fs.readFileSync('./server/battle.json').toString()
    battleLog = JSON.parse(battleLog)
    battleLog.forEach(ele=>{
      if (ele.user1 == username || ele.user2 == username)
        data.push(ele)
    })
    return data
  }
}

const file = new fileRW()

module.exports = {
  userLogin: (userInfo)=>{
    let curUser = file.getUserByName(userInfo.name)
    if (curUser != null && curUser.password == userInfo.password)
      return {status: 'ok'}
    else if (curUser != null && curUser.password != userInfo.password) 
      return {status: 'password error'}
    else return {status: 'username error'}
  },
  userRegister: (userInfo)=>{
    let curUser = file.getUserByName(userInfo.name)
    if (curUser == null) {
      file.addNewUser(userInfo)
      return {status:'ok'}
    } else {
      return {status: 'duplicated username'}
    }
  },
  userUpgrade(username) {
    file.updatePoint(username)
  },
  battleLog(bat) {
    file.addBattleLog(bat)
  },
  getUserPoint(username) {
    let curUser = file.getUserByName(username)
    return curUser.point
  },
  getUserBattleLog(username) {
    return file.getBattleLog(username)
  }
}