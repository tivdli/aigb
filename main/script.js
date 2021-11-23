var gW = `ws://${window.location.hostname}/ws`;
var ws;

window.addEventListener("beforeunload", (event) => {
  ws.close();
  // Cancel the event as stated by the standard.
  event.preventDefault();
  // Older browsers supported custom message
  event.returnValue = "";
});

window.onload = function () {
    //initWS();
};

function initWS(){
    ws = new WebSocket(gW);
    ws.onopen = onOpen;
    ws.onclose = onClose;
    ws.onmessage = onMessage;
}

function onOpen(event){
  ws.send("start");
}

function onClose(event){
    setTimeout(initWS, 2000);
}

function onMessage(event){
  msg = JSON.parse(event.data);
  switch (msg['key']){
    case "start":
      fillPage(msg);
    break;
  }
}
var buttonStates =[0];
var sliderStates =[0];
var light_sensor =[0];
var light_color  =[0];
var feed_values  =[0];
var air_sensor   =[0];
var air_input    =[0];

function processButton(elem) {
  var first = false;
  if (typeof elem == "string"){
    first = true;
  }
  num = parseInt(elem.id.split("")[1]);
  if (num < (buttonStates.length + 1))
  {
    if (buttonStates[num-1] == 1) {
      document.getElementById(elem.id + "_c").style.color = "red";
      buttonStates[num-1] = 0;
    } else {
      document.getElementById(elem.id + "_c").style.color = "green";
      buttonStates[num-1] = 1;
    }
  } else if (num == (buttonStates.length + 1)){
    document.getElementById(elem.id + "_c").style.color = "green";
    setTimeout(function(){document.getElementById(elem.id + "_c").style.color = "black"},250);
  }
}
function fillPage(msg) {
  buttonStates = msg['btn'];
  sliderStates = msg['sld'];
  light_sensor = msg['llm'];
  light_color  = msg['clr'];
  feed_values  = msg['fvl'];
  air_sensor   = msg['ase'];
  air_input    = msg['ain'];
  if (msg['prf'] != 0){
    ws.send(msg['prf']);
  }
}


function processSlider(elem){
  document.getElementById(elem.id + "_v").innerHTML = elem.value;
}
//Profile script
profile_name_climate = ["Tropical", "Dry", "Temperate", "Continental"];
profile_name_stage = ["Seedling", "Vegetative", "Budding", "Ripening"];
profile_name_type = ["Common", "Leaf", "Fruit", "Flower"];
profile_name_mode = ["Phase", "Stable", "Custom", "Stress"];
function concName(n1,n2,n3,n4) {
  return (
    profile_name_climate[n1] +
    " " +
    profile_name_stage[n2] +
    " " +
    profile_name_type[n3] +
    " " +
    profile_name_mode[n4]
  );
}

function concNameFromByte(b){
  n1 = b & 0b11000000 >>> 6;
  n2 = b & 0b00110000 >>> 4;
  n3 = b & 0b00001100 >>> 2;
  n4 = b & 0b00000011;
  return concName(n1,n2,n3,n4);
}

function profileOptions(optionlist){
  select = document.getElementById("pss_sct");
  for (var i = 0; i <optionlist.length; i++){
    var option = document.createElement("option");
    option.setAttribute("value", i);
    option.innerHTML = concNameFromByte(optionlist[i]);
    select.appendChild(option)
  }
}
function fillProfiles() {
  for (var i = 1; i < 5; i++) {
    switch (i) {
      case 1:
        prof_array = profile_name_climate;
        break;
      case 2:
        prof_array = profile_name_stage;
        break;
      case 3:
        prof_array = profile_name_type;
        break;
      case 4:
        prof_array = profile_name_mode;
        break;
    }
    opt_array = document.getElementsByName("prof_opt_" + String(i));
    for (var y = 0; y < 4; y++) {
      opt_array[y].innerText = prof_array[y];
    }
  }
}

function updatePage(){
  buttonStates.forEach((x,i) => processButton(document.getElementById("b" + i+1)));
  sliderStates.forEach((x,i) => processSlider(document.getElementById("s" + i+1)));
  light_sensor.forEach((x,i)
  light_color  = msg['clr'];
  feed_values  = msg['fvl'];
  air_sensor   = msg['ase'];
  air_input    = msg['ain'];
}

function idConv(id){

}
//
//Feed control script
