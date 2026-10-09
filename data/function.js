var xmlHttp=createXmlHttpObject();
var P_lang = "EN";
function createXmlHttpObject(){
  if(window.XMLHttpRequest){
    xmlHttp=new XMLHttpRequest();
  }else{
    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
  }
  return xmlHttp;
}

function load(){
  if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
    xmlHttp.open('GET','/configs.json',true);
    xmlHttp.send(null);
    xmlHttp.onload = function(e) {
      jsonResponse=JSON.parse(xmlHttp.responseText);
      loadBlock(xmlHttp.onload);
      updateWeather();
      loadWeather();
    }
    //alert();
  }
}

function loadBlock() {
  var data2 = JSON.parse(xmlHttp.responseText);
  data = document.getElementsByTagName('body')[0].innerHTML;
  var new_string;
  for (var key in data2) {
    new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
    data = new_string;
  }
  document.getElementsByTagName('body')[0].innerHTML = new_string;
  var inputs = document.getElementsByTagName("input");
  var selects = document.getElementsByTagName("select");
  var ranges = document.querySelectorAll('input[type="range"]'); // <-- ДОБАВЛЕНО
  for (var key in data2) {

    // Для чекбоксов
    if(data2[key] == 'checked'){
       //alert(key);                            // выводим название найденной переменной
       for (var i = 0; i < inputs.length; i++) {
         if (inputs[i].id === key) {
           inputs[i].checked = "true";
         }
       }
    }
        
    // Для select
     for (var i = 0; i < selects.length; i++) {
      if (selects[i].id === key) {
        //alert(key +  " : " + data2[key]);
        document.getElementById(key).value = data2[key];
      }
    }

    // Для слайдеров (range)
    for (var i = 0; i < ranges.length; i++) {
      if (ranges[i].id === key) {
        ranges[i].value = data2[key];
      }
    }
  }
   handleServerResponse();
}

function val(id){
  var v = document.getElementById(id).value;
  return v;
  
}

function val_sw(nameSwitch) {
  switchOn = document.getElementById(nameSwitch);
  if (switchOn.checked){
    return 1;
  }
  return 0;
}

// function send_request(submit,server){
//   request = new XMLHttpRequest();
//   request.open("GET", server, true);
//   request.send();
//   save_status(submit,request);
// }

async function send_request(submit, server) {
  try {
    const response = await fetch(server, { method: 'GET' });
    const data = await response.text(); // или response.json() в зависимости от данных
    save_status(submit, data);
  } catch (error) {
    console.error('Ошибка при отправке запроса:', error);
  }
}

function save_status(submit,request){
  old_submit = submit.value;
  request.onreadystatechange = function() {
    if (request.readyState != 4) return;
    submit.value = request.responseText;
    setTimeout(function(){
      submit.value=old_submit;
      submit_disabled(false);
    }, 1000);
  }
  submit.value = 'Зачекайте...';
  submit_disabled(true);
}

function submit_disabled(request){
  var inputs = document.getElementsByTagName("input");
  for (var i = 0; i < inputs.length; i++) {
    if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
  }
}

function toggle(target) {
  var curVal = document.getElementById(target).className;
  document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
