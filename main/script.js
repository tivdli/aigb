var ws;
var gW = "ws://" + window.location.hostname + "/ws";
const PROFVAR = 16;
profile_name_climate = ["Tropical", "Dry", "Temperate", "Continental"];
profile_name_stage = ["Seedling", "Vegetative", "Budding", "Ripening"];
profile_name_type = ["Common", "Leaf", "Fruit", "Flower"];
profile_name_mode = ["Phase", "Stable", "Custom", "Stress"];
var buttonStates = [0, 0, 0, 0];
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
};

function initWS() {
  ws = new WebSocket(gW);
  ws.onopen = onOpen;
  ws.onclose = onClose;
  ws.onmessage = onMessage;
}

function onOpen(event) {
  message("stt", 0, 0);
  fillProfiles();
}

function onClose(event) {
  setTimeout(initWS, 2000);
}

function onMessage(event) {
  console.log(event)
  msg = JSON.parse(event.data);
  console.log(msg);
  switch (msg["key"]) {
    case "stt":
      fillPage(msg);
      updatePage();
      break;
    case "pfr":
      setProfileRead(msg);
      break;
    case "pfw":
      handleProfWrite(msg);
      break;
    case "pfo":
      profileOptions(msg);
  }
}

function handleProfWrite(msg) {
  if (msg["go"] == 0) {
    go = msg["go"];
  } else {
    go = 0 ? window.confirm("Profile already exists, do you want to overwrite it?") : 1;
  }
  if (go == 0) {
    pre = "wp";
    console.log("COLOR:" + document.getElementById("wp5"));
    const data = [];
    for (i = 1; i <= PROFVAR; i++) {
      if (i < 6 || i > 9) {
        if (
          !["Climate", "Stage", "Type", "Mode", undefined, ""].includes(
            document.getElementById(pre + i).value
          )
        ) {
          if (i > 5) {
            data.push(parseInt(document.getElementById(pre + i).value));
          } else if (i == 4) {
            data.push(getNameNumber());
          } else if (i == 5) {
            col_str = document.getElementById(pre + i).value.split("#").join("");
            for (c = 0; c < 3; c++){
              data.push(parseInt(col_str.substring(c*2, c*2 + 2), 16));
            }
          }
        } else {
          window.alert(
            "Profile write selection not valid (have you selected all name types?)"
          );
          return false;
        }
      } else if (i % 2 == 0) {
        clock = document.getElementById(pre + i).value.split(":");
        if (clock[1] == undefined) {
          window.alert("Profile write selection not valid: time missing");
          return false;
        }
        data.push(parseInt(clock[0]));
        data.push(parseInt(clock[1]));
      }
    }
    console.log(data);
    message("pfw", data);
  }
}

function processButton(elem) {
  var first = false;
  if (typeof elem == "string") {
    first = true;
    elem = document.getElementById(elem);
    buttonStates.forEach((x, i) => buttonStates[i] = 0 ? x : 1);
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
  } else if (num >= buttonStates.length + 1) {
    elem.style.color = "green";
    setTimeout(function () {
      elem.style.color = "black";
    }, 250);
    if (elem.id == "b6"){
      message("btn", elem.id, document.getElementById("pss_sct").value);
    }
    else {
      message("btn", elem.id, getNameNumber());
    }
  }
}

function getNameNumber() {
  ret = 0;
  for (a = 1; a < 5; a++) {
    ret += (document.getElementById("wp" + a).value - 1) * 2 ** (8 - (a * 2));
  }
  return ret;
}

function fillPage(msg) {
  buttonStates = [btn_helper(+msg["bt0"]),btn_helper(+msg["bt1"]),btn_helper(+msg["bt2"]),btn_helper(+msg["bt3"])];
  sliderStates = [msg["sl0"],msg["sl1"],msg["sl2"]];
  light_sensor = [msg["ll0"],msg["ll1"]];
  light_color = msg["clr"];
  feed_values = [msg["fv0"],msg["fv1"]];
  air_sensor = [msg["as0"], msg["as1"], msg["as2"], msg["as3"], msg["as4"]];
  air_input = [msg["ai0"], msg["ai1"]];
  if (msg["prf"]  > 0) {
    message("prr", 0, 0);
  }
}

function processSlider(elem) {
  document.getElementById(elem.id + "_v").innerHTML = elem.value;
}

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
  n1 = (b & 0b11000000) >>> 6;
  n2 = (b & 0b00110000) >>> 4;
  n3 = (b & 0b00001100) >>> 2;
  n4 = b & 0b00000011;
  console.log(b);

  return concName(n1, n2, n3, n4);
}

function profileOptions(optionlist) {
  select = document.getElementById("pss_sct");
  curr_options = [...document.querySelector("#pss_sct").options].map( opt => opt.value );
  for (var i = 0; i < (Object.keys(optionlist).length - 1); i++) {
    if (curr_options.includes(optionlist[i.toString()]) == false)
    {
      var option = document.createElement("option");
      option.setAttribute("value", optionlist[i.toString()]);
      option.innerHTML = concNameFromByte(optionlist[i.toString()]);
      select.appendChild(option);
    }
  }
}

//Put name options from array into options
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

  document.getElementById("c1").value = "#" + light_color.toString(16).padStart(6, "0");
}

function request_profile()
{
  doc_sel = document.getElementById("pss_sct");
  message("pfs", 0, doc_sel.value);
}
function setProfileRead(msg) {
  pref = "r";
  document.getElementById(pref + "p9").innerText = msg[7].toString().padStart(2,"0");
  document.getElementById(pref + "p7").innerText = msg[5].toString().padStart(2,"0");
  document.getElementById(pref + "p5").innerText = "#" + msg[1].toString(16).padStart(2, "0") + msg[2].toString(16).padStart(2, "0") + msg[3].toString(16).padStart(2, "0");
  document.getElementById(pref + "p6").innerText = msg[4];
  document.getElementById(pref + "p8").innerText = msg[6];
  document.getElementById(pref + "p10").innerText = msg[8];
  document.getElementById(pref + "p11").innerText = msg[9];
  document.getElementById(pref + "p12").innerText = msg[10];
  document.getElementById(pref + "p13").innerText = msg[11];
  document.getElementById(pref + "p14").innerText = msg[12];
  document.getElementById(pref + "p15").innerText = msg[13];
  document.getElementById(pref + "p16").innerText = msg[14];
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

//Quick helper function, time's up
function btn_helper(i)
{
  return 0 ? i==1 : 0;
}

function updateTime(){
  go = window.confirm("Do you want to set AIGB time to local system time?");
  if (go) {
    time = new Date();
    data = [time.getFullYear(), time.getMonth()+1, time.getDate(), time.getHours(), time.getMinutes(), time.getSeconds()];
    console.log(data);
    message("utv", 0, data);
  }
}