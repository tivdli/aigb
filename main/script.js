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
    profileOptions([132,54,255,0,142,57,26,27,222,195]);
    fillProfiles();
};

function initWS(){
    ws = new WebSocket(gW);
    ws.onopen = onOpen;
    ws.onclose = onClose;
    ws.onmessage = onMessage;
}

function onOpen(event){
}

function onClose(event){
    setTimeout(initWS, 2000);
}

//function onMessage(event)
buttonStates = [0,0,0,0];
sliderStates = [0,0,0];
function processButton(elem) {
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
function fillPage() {
  return True;
}

function processSlider(elem){
  console.log(elem.value);
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
//
//Feed control script
