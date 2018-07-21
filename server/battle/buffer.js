module.exports = {
  /*事件
  eventType: string (monster/end-battle)
  monstertype: string (turret/monster/null)
  master: string (username)
  action: string(die/create/null)
  positionX: number | null
  positionY: number | null
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
  addEvent(id, event){
    this.eventBuffer.forEach(ele=>{
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
  getEvent(id, LSN){
    let data = null
    this.eventBuffer.forEach(ele=>{
      if (ele.id == id) {
        if (LSN < ele.events[ele.events.length - 1].LSN) {
          data = ele.events[ele.events.length - 1]
          return;
        }
      }
    })
    return data
  }
}