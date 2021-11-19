import htmlmin

text="""
<!DOCTYPE html>
<html>

<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <meta http-equiv="X-UA-Compatible" content="ie=edge" />
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.0.13/css/all.css"
    integrity="sha384-DNOHZ68U8hZfKXOrtjWvjxusGo9WQnrNx2sqG0tfsghAvtVlRW3tvkXWZh58N9jp" crossorigin="anonymous" />
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.1/css/bootstrap.min.css"
    integrity="sha384-WskhaSGFgHYWDcbwN70/dfYBj47jz9qbsMId/iRN3ewGhXQFZCSftd1LZCfmhktB" crossorigin="anonymous" />
  <title>AIGB</title>
  <style>
    .slidecontainer {
      width: 100%;
    }

    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 25px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }

    .slider:hover {
      opacity: 1;
    }

    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      background: #28A745;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      background: #28A745;
      cursor: pointer;
    }
  </style>
</head>

<body onLoad="javascript:WebSocketBegin()">
  <header id="main-header" class="py-2 bg-success text-white">
    <div class="container">
      <div class="row justify-content-md-center">
        <div class="col-md-6 text-center">
          <h1><i class="fas fa-leaf"></i> AIGB Control Panel v0</h1>
        </div>
      </div>
    </div>
  </header>

  <section class="py-5 bg-white">
    <div class="container">
      <h2 class="deck-title">Quick controls</h2>


      <!-- Quick controls -->
      <div class="card-deck">
        <!-- Pumpblock -->
        <div class="card bg-light m-2 text-center" style="min-height: 15rem;">


          <div class="card-header">Pump controls</div>
          <div class="card-body">
            <h5 class="card-title">Pump power: <em id="wpp_plc">0</em>%</h5>
            <div class="slidecontainer">
              <input type="range" min="0" max="100" value="0" class="slider" id="wpp_out">
            </div>
          </div>
          <div class="d-flex justify-content-center card-footer"><button id="wpb_btn"><i id="wpb_out"
                class="fas fa-power-off fa-2x" style="color:red;"></i></button></div>
        </div>
        <!-- Lightblock -->
        <div class="card bg-light m-2" style="min-height: 15rem;">
          <div class="card-header">Light controls</div>
          <div class="card-body">
            <h5 class="card-title">Light power: <em id="lcp_plc">0</em>%</h5>
            <div class="slidecontainer">
              <input type="range" min="0" max="100" value="0" class="slider" id="lcp_out">
            </div>
            <div class="card-text">
              <h6 class="card-title">Sensor readouts</h6>
              <table style="width:100%;">
                <tr>
                  <td>Top:</td>
                  <td id="lcs_tout">0</td>
                  <td>lm</td>
                </tr>
                <tr>
                  <td>Mid:</td>
                  <td id="lcs_mout">0</td>
                  <td>lm</td>
                </tr>
                <tr>
                  <td>Bottom:</td>
                  <td id="lcs_bout">0</td>
                  <td>lm</td>
                </tr>
              </table>
            </div>
            <h5 class="card-title d-flex justify-content-center"><i class="fas fa-lightbulb" id="lcc_in"></i>RGB</h5>
            <div class="slidecontainer">
              <input style="background:#000000;" type="range" min="0" max="255" value="0" class="slider" id="lcc_rout">
              <input style="background:#000000;" type="range" min="0" max="255" value="0" class="slider" id="lcc_gout">
              <input style="background:#000000;" type="range" min="0" max="255" value="0" class="slider" id="lcc_bout">
            </div>
          </div>
          <div class="d-flex justify-content-center card-footer"><button id="lcb_btn"><i id="lcb_out"
                class="fas fa-power-off fa-2x" style="color:red;"></i></button></div>
        </div>
        <!-- Profileblock -->
        <div class="card bg-light m-2" style="min-height: 15rem;">
          <div class="card-header">Profile selection</div>
            <div class="card-body">
              <select id="pss_sct" class="form-control card-text">
                <option disabled selected>Change profile here</option>
                <option value="1">Leafy green grow-phase</option>
                <option value="2">Leafy green flower phase</option>
                <option value="3">Fruit grow-phase</option>
                <option value="4">Fruit flower phase</option>
              </select>
              <h5 style="padding-top:10px;" class="card-title">Profile characteristics</h6>
                <p class="card-text">Light color: <em id="pss_col">0</em></p>
                <p class="card-text">Light timing: <em id="pss_lon">00:00</em>/<em id="pss_lof">00:00</em></p>
                <p class="card-text">Feed interval: <em id="pss_fin">0</em> days</p>
                <p class="card-text">Feed quantity: <em id="pss_fqy">0</em> ml</p>
            </div>
          </div>
        </div>
      </div>
    </div>
    <div class="container">
      <h2 class="deck-title">Manual controls</h2>


      <!-- Feedblock -->
      <div class="card-deck">
        <div class="card bg-light m-2 text-center" style="min-height: 15rem;">


          <div class="card-header">Feed controls</div>
            <div class="card-body">
              <h5 class="card-title">deposition volume:</h5>
              <input type="number" min="0" max="5" value="0" class="card-text" id="fpd_out"  step="0.5" value="0">
              <h5 class="card-title">Feed interval</h5>
              <input type="number" min="0" max="30" value="0" class="card-text" id="fpi_out"  step="1" value="0">
            </div>
            <div class="d-flex justify-content-center card-footer"><button id="fpb_btn"><i id="fpb_out"
                  class="fas fa-power-off fa-2x" style="color:red;"></i></button></div>
        </div>
        <!-- Airblock -->
        <div class="card bg-light m-2" style="min-height: 15rem;">
          <div class="card-header">Climate controls</div>
            <div class="card-body">
              <h5 class="card-title">Circulation power: <em id="acp_plc">0</em>%</h5>
              <div class="slidecontainer">
                <input type="range" min="0" max="100" value="0" class="slider" id="lcp_out">
              </div>
              <div class="card-text">
                <h6 class="card-title">Sensor readouts</h6>
                <table style="width:100%">
                  <tr>
                    <td>CO<sub>2</sub></td>
                    <td id="acv_co2">0</td>
                    <td>ppm</td>
                  </tr>
                  <tr>
                    <td>Temperature</td>
                    <td id="acv_tm1">0</td>
                    <td>°C</td>
                  </tr>
                  <tr>
                    <td>Rel. humidity</td>
                    <td id="acv_hum">0</td>
                    <td>%</td>
                  </tr>
                </table>
              </div>
              <h5 class="card-title d-flex justify-content-center">Settings</h5>
              <div class="card-text">
                <table style="width:100%">
                  <tr>
                    <td>Temperature</td>
                    <td><input type="number" min="15" max="30" id="aco_tem"></td>
                    <td>°C</td>
                  </tr>
                  <tr>
                    <td>Rel. humidity</td>
                    <td><input type="number" min="60" max="100" id="aco_hum"></td>
                    <td>%</td>
                  </tr>
                </table>
              </div>
            </div>
          <div class="d-flex justify-content-center card-footer"><button id="lcb_btn"><i id="lcb_out"
                class="fas fa-power-off fa-2x" style="color:red;"></i></button></div>
        </div>
        <!-- Make profile -->
        <div class="card bg-light m-2" style="min-height: 15rem;">
          <div class="card-header">Profile creation</div>
          <div class="card-body">
            <h5>Name:</h5>
            <select id="pcs_nm1" class="form-control card-text">
              <option disabled selected>Climate</option>
              <option value="1" name="prof_opt_1">0</option>
              <option value="2" name="prof_opt_1">0</option>
              <option value="3" name="prof_opt_1">0</option>
              <option value="4" name="prof_opt_1">0</option>
            </select>
            <select id="pcs_nm1" class="form-control card-text">
              <option disabled selected>Stage</option>
              <option value="1" name="prof_opt_2">0</option>
              <option value="2" name="prof_opt_2">0</option>
              <option value="3" name="prof_opt_2">0</option>
              <option value="4" name="prof_opt_2">0</option>
            </select>
            <select id="pcs_nm1" class="form-control card-text">
              <option disabled selected>Type</option>
              <option value="1" name="prof_opt_3">0</option>
              <option value="2" name="prof_opt_3">0</option>
              <option value="3" name="prof_opt_3">0</option>
              <option value="4" name="prof_opt_3">0</option>
            </select>
            <select id="pcs_nm4" class="form-control card-text">
              <option disabled selected>Mode</option>
              <option value="1" name="prof_opt_4">0</option>
              <option value="2" name="prof_opt_4">0</option>
              <option value="3" name="prof_opt_4">0</option>
              <option value="4" name="prof_opt_4">0</option>
            </select>

            <h5 style="padding-top:10px;" class="card-title">Profile characteristics</h6>
              <table style="width:100%">
              <tr>
                <td>Light color:</td>
                <td><input type="color" id="pcs_lcs"></td>
              </tr>
              <tr>
                <td>Light timing</td>
                <td><input type="time" id="pcs_lt1">/<input type="time" id="pcs_lt2"></td>
              </tr>
              <tr>
                <td>Feed interval</td>
                <td><input type="number" id="pcs_fpi" min="0" max="30" step="1"></td>
              </tr>
              <tr>
                <td>Feed volume</td>
                <td><input type="number" id="pcs_fpv" min="0" max="5" step="0.5"></td>
                <td>ml</td>
              </tr>
              </table>
          </div>
          <div class="d-flex justify-content-center card-footer"><button id="pcs_sav"><i id="lcb_out"
            class="fas fa-save fa-2x" style="color:black;"></i></button></div>
        </div>
      </div>
    </div>
    </div>
  </section>
</body>
<script type="text/javascript">
  var ws;
  window.addEventListener('beforeunload', (event) => {
    ws.close();
    // Cancel the event as stated by the standard.
    event.preventDefault();
    // Older browsers supported custom message
    event.returnValue = '';
  });

  function writeToDoc(v_name, val){
    cor = document.getElementsByName(v_name).value = val;
  }
  function WebSocketBegin() {
    if ("WebSocket" in window) {
      // Let us open a web socket
      ws = new WebSocket(
        location.hostname.match(/\.husarnetusers\.com$/) ? "wss://" + location.hostname + "/__port_8000/ws" : "ws://" + location.hostname + ":8000/ws"
      );
      // ws = new WebSocket(
      //   "wss://fc9434c5513c3543753124468f76fa507-8000.husarnetusers.com/ws"
      // );
      // ws = new WebSocket(
      //   "ws://esp32websocket:8000/ws"
      // );

      ws.onopen = function () {
        // Web Socket is connected
      };

      ws.onmessage = function (evt) {
        //create a JSON object
        var jsonObject = JSON.parse(evt.data);
        var cnt = jsonObject.counter;
        var btn = jsonObject.button;

        document.getElementById("cnt").innerText = cnt;
        if (btn == 1) {
          document.getElementById("btn").style.color = "green";
        } else {
          document.getElementById("btn").style.color = "red";
        }
      };

      ws.onclose = function (evt) {
        if (evt.wasClean) {
          alert(`[close] Connection closed cleanly, code=${evt.code} reason=${evt.reason}`);
        } else {
          // e.g. server process killed or network down
          // event.code is usually 1006 in this case
          alert('[close] Connection died');
        }
      };

      ws.onerror = function (error) {
        alert(`[error] ${error.message}`);
      }


    } else {
      // The browser doesn't support WebSocket
      alert("WebSocket NOT supported by your Browser!");
    }
  }
  // Pump cntrol script
  var wpp_el = document.getElementById("wpp_out");
  var wpp_val = document.getElementById("wpp_plc");
  wpp_el.oninput = function () {
    wpp_val.innerHTML = this.value;
  }

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
  }

  //Light control script
  var lcp_el = document.getElementById("lcp_out");
  var lcp_val = document.getElementById("lcp_plc");
  lcp_el.oninput = function () {
    lcp_val.innerHTML = this.value;
  }
  var lcc_rel = document.getElementById("lcc_rout");
  var lcc_gel = document.getElementById("lcc_gout");
  var lcc_bel = document.getElementById("lcc_bout");
  var lcc_r = 0
  var lcc_g = 0
  var lcc_b = 0
  setCol = function () {
    v = "#" + (lcc_r.toString(16).length == 1 ? "0" + lcc_r.toString(16) : lcc_r.toString(16)) +
      (lcc_g.toString(16).length == 1 ? "0" + lcc_g.toString(16) : lcc_g.toString(16)) +
      (lcc_b.toString(16).length == 1 ? "0" + lcc_b.toString(16) : lcc_b.toString(16))

    document.getElementById("lcc_in").style.color = v
  }
  lcc_rel.oninput = function () {
    lcc_r = parseInt(this.value);
    v = lcc_r.toString(16);
    if (v.length == 1) {
      v = "0" + v;
    }
    v = "#" + v + "0000";
    this.style.background = v;
    setCol();
  }
  lcc_gel.oninput = function () {
    lcc_g = parseInt(this.value);
    v = lcc_g.toString(16);
    if (v.length == 1) {
      v = "0" + v;
    }
    v = "#00" + v + "00";
    this.style.background = v;
    setCol();
  }
  lcc_bel.oninput = function () {
    lcc_b = parseInt(this.value);
    v = lcc_b.toString(16);
    if (v.length == 1) {
      v = "0" + v;
    }
    v = "#0000" + v;
    this.style.background = v;
    setCol();
  }
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
  }
  //Profile script
  profile_name_climate = ["Tropical", "Dry", "Temperate", "Continental"];
  profile_name_stage = ["Seedling", "Vegetative", "Budding", "Ripening"];
  profile_name_type = ["Common", "Leaf", "Fruit", "Flower"];
  profile_name_mode = ["Phase", "Stable", "Custom", "Stress"];
  function conc_name(n1,n2,n3,n4)
  {
    return profile_name_climate[n1]+" "+profile_name_stage[n2]+" "+profile_name_type[n3]+" "+profile_name_mode[n4];
  }
  window.onload=function() {
    document.getElementById("test").innerText = conc_name(1,0,3,2);
    for (var i = 1; i < 5; i++){
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
      for (var y = 0; y<4; y++)
      {
        opt_array[y].innerText = prof_array[y];
      }
    }
  }
  //
  //Feed control script


</script>

</html>"""
print(len(text)//1024)
out = (htmlmin.minify(text, remove_all_empty_space=True, remove_comments=True, remove_optional_attribute_quotes=True))

print(len(out)//1024)

