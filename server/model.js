const fs = require('fs')

class fileRW {
  constructor() {}
  getUserByName(name) {
    let userInfo = fs.readFileSync('./user.json').toString()
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
    let userInfo = fs.readFileSync('./user.json').toString()
    let users = JSON.parse(userInfo)
    users.push(user)
    fs.writeFileSync('./user.json', JSON.stringify(users))
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
  }
}