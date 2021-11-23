var gW = `ws://${window.location.hostname}/ws`;
var ws;
console.log = log;

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

function initWS(){
    ws = new WebSocket(gW);
    ws.onopen = onOpen;
    ws.onclose = onClose;
    ws.onmessage = onMessage;
    log("websocket started");
}

function onopen(event){
    log("connection made");
}

function onClose(event){
    log("connection closed");
    setTimeout(initWS, 2000);
}

function onMessage(event)
function processButton(elem) {}

function fillPage() {
  return True;
}
// Pump cntrol script
var wpp_el = document.getElementById("wpp_out");
var wpp_val = document.getElementById("wpp_plc");
wpp_el.oninput = function () {
  wpp_val.innerHTML = this.value;
};

var wpb_el = document.getElementById("wpb_btn");
var wpb_val = 0;
wpb_el.onclick = function () {
  if (wpb_val == 1) {
    document.getElementById("wpb_out").style.color = "red";
    wpb_val = 0;
  } else {
    wpb_val = 1;
    document.getElementById("wpb_out").style.color = "green";
  }
};

//Light control script
var lcp_el = document.getElementById("lcp_out");
var lcp_val = document.getElementById("lcp_plc");
lcp_el.oninput = function () {
  lcp_val.innerHTML = this.value;
};
var lcc_rel = document.getElementById("lcc_rout");
var lcc_gel = document.getElementById("lcc_gout");
var lcc_bel = document.getElementById("lcc_bout");
var lcc_r = 0;
var lcc_g = 0;
var lcc_b = 0;
setCol = function () {
  v =
    "#" +
    (lcc_r.toString(16).length == 1
      ? "0" + lcc_r.toString(16)
      : lcc_r.toString(16)) +
    (lcc_g.toString(16).length == 1
      ? "0" + lcc_g.toString(16)
      : lcc_g.toString(16)) +
    (lcc_b.toString(16).length == 1
      ? "0" + lcc_b.toString(16)
      : lcc_b.toString(16));

  document.getElementById("lcc_in").style.color = v;
};
lcc_rel.oninput = function () {
  lcc_r = parseInt(this.value);
  v = lcc_r.toString(16);
  if (v.length == 1) {
    v = "0" + v;
  }
  v = "#" + v + "0000";
  this.style.background = v;
  setCol();
};
lcc_gel.oninput = function () {
  lcc_g = parseInt(this.value);
  v = lcc_g.toString(16);
  if (v.length == 1) {
    v = "0" + v;
  }
  v = "#00" + v + "00";
  this.style.background = v;
  setCol();
};
lcc_bel.oninput = function () {
  lcc_b = parseInt(this.value);
  v = lcc_b.toString(16);
  if (v.length == 1) {
    v = "0" + v;
  }
  v = "#0000" + v;
  this.style.background = v;
  setCol();
};
var lcb_el = document.getElementById("lcb_btn");
var lcb_val = 0;
lcb_el.onclick = function () {
  if (lcb_val == 1) {
    document.getElementById("lcb_out").style.color = "red";
    lcb_val = 0;
  } else {
    lcb_val = 1;
    document.getElementById("lcb_out").style.color = "green";
  }
};

var acp_el = document.getElementById("acp_out");
var acp_val = document.getElementById("acp_plc");
acp_el.oninput = function () {
  acp_val.innerHTML = this.value;
};

//Profile script
profile_name_climate = ["Tropical", "Dry", "Temperate", "Continental"];
profile_name_stage = ["Seedling", "Vegetative", "Budding", "Ripening"];
profile_name_type = ["Common", "Leaf", "Fruit", "Flower"];
profile_name_mode = ["Phase", "Stable", "Custom", "Stress"];
function conc_name(n1, n2, n3, n4) {
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

function profileOptions(){
  
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
    console.log("prof_opt_" + String(i));
    opt_array = document.getElementsByName("prof_opt_" + String(i));
    console.log(opt_array);
    for (var y = 0; y < 4; y++) {
      opt_array[y].innerText = prof_array[y];
    }
  }
}
//
//Feed control script
