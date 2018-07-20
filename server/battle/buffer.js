module.exports = {
  /*事件
  eventType: string (create/end-battle)
  monstertype: string
  positionX: number
  positionY: number
  */
  /*对战信息
  lastLSN: number
  monsterPosition: {
    monster1 : {
      X: number,
      Y: number
    },
    ...
  }
  */
  eventBuffer: [],
  battleBuffer: [],
  addEvent: (id, event)=>{
    eventBuffer.forEach(ele=>{
      if (ele.id == id) {
        let newLSN = ele.events.length
        ele.events.push({
          LSN: newLSN,
          event: event
        })
      }
    })
  },
  addBattleInfo: (id, battleInfo)=>{
    // todo
  },
  getEvent: (id, LSN)=>{
    eventBuffer.forEach(ele=>{
      if (ele.id == id) {
        if (LSN < ele.events[ele.events.length - 1].LSN)
          return ele.events[ele.events.length - 1]
      }
    })
    return null
  }
}