window.onload = function() {
  $('#register').click(()=>{
    let username = document.getElementById('username').value
    let password = document.getElementById('password').value
    $.ajax('api/register', {
      type: 'POST',
      data: {
        name: username,
        password: password
      }
    }).done((result)=>{
      console.log(result)
    })
  })
  $('#login').click(()=>{
    let username = document.getElementById('username').value
    let password = document.getElementById('password').value
    $.ajax('api/login', {
      type: 'POST',
      data: {
        name: username,
        password: password
      }
    }).done((result)=>{
      console.log(result)
    })
  })
  $('.waiting').click(function() {
    let username = $(this).attr('name')
    let url = `api/battle/waiting/${username}`
    $.ajax(url, {
      type: 'GET'
    }).done((result)=>{
      console.log(result)
    })
  })
  $('.begin-battle').click(function() {
    let username = $(this).attr('name')
    $.ajax(`api/battle/begin/${username}`, {
      type: 'GET'
    }).done((result)=>{
      if (result.status === 'begin') {
        if (username == 'ff') {
          $('#battle-id-1').text(result.id)
        } else {
          $('#battle-id-2').text(result.id)
        }
      } else {
        if (username == 'ff') {
          $('#battle-id-1').text(result.status)
        } else {
          $('#battle-id-2').text(result.status)
        }
      }
    })
  })
  $('.create-monster').click(function() {
    let name = $(this).attr('name')
    let monsterType = $(this).attr('monster')
    let x = $(this).attr('x')
    let y = $(this).attr('y')
    let id = (name == 'ff') ? $('#battle-id-1').text() : $('#battle-id-2').text();
    let data = {
      eventType: 'monster',
      monsterType: monsterType,
      master: name,
      action: 'create',
      positionX: x,
      positionY: y
    }
    $.ajax('/api/battle/event', {
      type: 'POST',
      data: {
        id: Number(id),
        event: data
      }
    }).done((result)=>{
      console.log(result)
    })
  })
  $('.query-event').click(function() {
    let name = $(this).attr('name')
    let id = (name == 'ff') ? $('#battle-id-1').text() : $('#battle-id-2').text();
    let LSN = (name == 'ff') ? $('#LSN-0').text() : $('#LSN-1').text()
    $.ajax(`api/battle/event/battle-id/${id}/LSN/${LSN}`, {
      type: 'GET'
    }).done((result)=>{
      let event_list = (name == 'ff') ? $('#list1').text() : $('#list2').text()
      if (result.event != null)
        event_list += JSON.stringify(result.event.event)
      if (name == 'ff') {
        $('#list1').text(event_list)
        $('#LSN-0').text(result.event.LSN)
      } else {
        $('#list2').text(event_list)
        $('#LSN-1').text(result.event.LSN)
      }
    })
  })
}