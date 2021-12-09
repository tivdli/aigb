var ws;
var gW = "ws://" + window.location.hostname + "/ws";
const PROFVAR = 16;
profile_name_climate = ["Tropical", "Dry", "Temperate", "Continental"];
profile_name_stage = ["Seedling", "Vegetative", "Budding", "Ripening"];
profile_name_type = ["Common", "Leaf", "Fruit", "Flower"];
profile_name_mode = ["Phase", "Stable", "Custom", "Stress"];
var buttonStates = [0,0,0,0];
var sliderStates = [0];
var light_sensor = [0];
var light_color = 0;
var feed_values = [0];
var air_sensor = [0];
var air_input = [0];

window.addEventListener("beforeunload", (event) => {
  ws.close();
  // Cancel the event as stated by the standard.
  event.preventDefault();
  // Older browsers supported custom message
  event.returnValue = "";
});

window.onload = function () {
  initWS();
  o = JSON.parse(
    '{"key":"start",' +
      '"btn":[1,1,1,1],' +
      '"sld":[70,30,40],' +
      '"llm":[580, 270, 400],' +
      '"clr":"#A000FF",' +
      '"fvl":[12,1.5],' +
      '"ase":[55, 14, 70],' +
      '"ain":[20, 110]}'
  );
  p = JSON.parse(
    '{"key":"profl","nam":136,"col":"#A000FF","onh":6,"ofh":22, "onm":0,"ofm":30,"fin":12,"fqy":1.5,"tpd":20,"tpn":12,"hmd":80,"hmn":90}'
  );
  fillProfiles();
  fillPage(o);
  updatePage();
};

function initWS() {
  ws = new WebSocket(gW);
  ws.onopen = onOpen;
  ws.onclose = onClose;
  ws.onmessage = onMessage;
}

function onOpen(event) {
  message("stt", 0,0);
}

function onClose(event) {
  setTimeout(initWS, 2000);
}

function onMessage(event) {
  msg = JSON.parse(event.data);
  switch (msg["key"]) {
    case "stt":
      fillPage(msg);
      updatePage();
      break;
    case "pfr":
      setProfileRead(p);
    case "pfw":
      handleProfWrite(msg);
  }
}

function handleProfWrite(msg){
  go = msg["go"] ? msg["go"] : window.confirm("Profile already exists, do you want to overwrite it?");
  if (){
    pre="wp";
    const data = [];
    for (i = 1; i <= PROFVAR; i++){
      if (i <6 || i > 9)
      {
        if (!["Climate", "Stage", "Type", "Mode", undefined, ""].includes(document.getElementById(pre+i).value)){
          data.push(document.getElementById(pre+i).value);
        }
        else{
          window.alert("Profile write selection not valid (have you selected all name types?)");
          return false;
        }
      }
      else if (i%2==0)
      {
        clock = document.getElementById(pre+i).value.split(":");
        if (clock[1] == undefined) { window.alert("Profile write selection not valid: time missing"); return false}
        data.push(clock[0]);
        data.push(clock[1]);
      }
    }
    message("pfw", data);
  }
}

function processButton(elem) {
  var first = false;
  if (typeof elem == "string") {
    first = true;
    elem = document.getElementById(elem);
  }
  num = parseInt(elem.id.split("")[1]);
  if (num < buttonStates.length + 1) {
    if (buttonStates[num - 1] == 1) {
     elem.style.color = "red";
      if (!first) {
        buttonStates[num - 1] = 0;
      }
    } else {
      elem.style.color = "green";
      if (!first) {
        buttonStates[num - 1] = 1;
      }
    }
    if (!first) {
      message("btn", elem.id, buttonStates[num - 1]);
    }
  } else if (num == buttonStates.length + 1) {
    elem.style.color = "green";
    setTimeout(function () {
      elem.style.color = "black";
    }, 250);
    message("btn", elem.id, 1);
  }
}
function fillPage(msg) {
  buttonStates = msg["btn"];
  sliderStates = msg["sld"];
  light_sensor = msg["llm"];
  light_color = msg["clr"];
  feed_values = msg["fvl"];
  air_sensor = msg["ase"];
  air_input = msg["ain"];
  if (typeof msg["prf"] != "undefined") {
    ws.send(msg["prr"]);
  }
}

function processSlider(elem) {
  document.getElementById(elem.id + "_v").innerHTML = elem.value;
}
//Profile script

function concName(n1, n2, n3, n4) {
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

function concNameFromByte(b) {
  n1 = b & (0b11000000 >>> 6);
  n2 = b & (0b00110000 >>> 4);
  n3 = b & (0b00001100 >>> 2);
  n4 = b & 0b00000011;
  return concName(n1, n2, n3, n4);
}

function profileOptions(optionlist) {
  select = document.getElementById("pss_sct");
  for (var i = 0; i < optionlist.length; i++) {
    var option = document.createElement("option");
    option.setAttribute("value", i);
    option.innerHTML = concNameFromByte(optionlist[i]);
    select.appendChild(option);
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

function updatePage() {
  buttonStates.forEach((x, i) => processButton("b" + (i + 1)));
  sliderStates.forEach((x, i) =>
    processSlider(document.getElementById("s" + (i + 1)))
  );
  light_sensor.forEach(
    (x, i) => (document.getElementById("l" + (i + 1)).innerText = x)
  );
  feed_values.forEach(
    (x, i) => (document.getElementById("f" + (i + 1)).value = x)
  );
  air_sensor.forEach(
    (x, i) => (document.getElementById("a" + (i + 1)).innerText = x)
  );
  air_input.forEach(
    (x, i) => (document.getElementById("i" + (i + 1)).value = x)
  );

  document.getElementById("c1").value = light_color.toString(16);
}

function setProfileRead(msg) {
  pref = "r";
  document.getElementById(pref + "p9").innerText = msg["onm"]
    ? msg["onm"] < 10
    : "0" + msg["onm"];
  document.getElementById(pref + "p7").innerText = msg["onm"]
    ? msg["onm"] < 10
    : "0" + msg["onm"];
  document.getElementById(pref + "p5").innerText = msg["col"];
  document.getElementById(pref + "p6").innerText = msg["onh"];
  document.getElementById(pref + "p8").innerText = msg["ofh"];
  document.getElementById(pref + "p10").innerText = msg["fin"];
  document.getElementById(pref + "p11").innerText = msg["fqy"];
  document.getElementById(pref + "p12").innerText = msg["tpd"];
  document.getElementById(pref + "p13").innerText = msg["tpn"];
  document.getElementById(pref + "p14").innerText = msg["hmd"];
  document.getElementById(pref + "p15").innerText = msg["hmn"];
}

function setProfileWrite() {
  pref = "w";
  document.getElementById(pref + "p5").innerText = msg["col"];
  document.getElementById(pref + "p10").innerText = msg["fin"];
  document.getElementById(pref + "p11").innerText = msg["fqy"];
  document.getElementById(pref + "p12").innerText = msg["tpd"];
  document.getElementById(pref + "p13").innerText = msg["tpn"];
  document.getElementById(pref + "p14").innerText = msg["hmd"];
  document.getElementById(pref + "p15").innerText = msg["hmn"];
}

function message(type, id, data) {
  comm = {
    n: type,
    i: id,
    d: data,
  };
  console.log(JSON.stringify(comm));
  ws.send(JSON.stringify(comm));
}
