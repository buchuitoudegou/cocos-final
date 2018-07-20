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
}