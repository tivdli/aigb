var gW=`ws:var ws;window.addEventListener("beforeunload",(event)=>{ws.close();event.preventDefault();event.returnValue="";});window.onload=function(){o=JSON.parse('{"key":"start",'+
'"btn":[1,1,1,1],'+
'"sld":[70,30,40],'+
'"llm":[580, 270, 400],'+
'"clr":"#A000FF",'+
'"fvl":[12,1.5],'+
'"ase":[55, 14, 70],'+
'"ain":[20, 110]}');p=JSON.parse('{"key":"profl","nam":136,"col":"#A000FF","onh":6,"ofh":22, "onm":0,"ofm":30,"fin":12,"fqy":1.5,"tpd":20,"tpn":12,"hmd":80,"hmn":90}');fillPage(o);updatePage();setProfileRead(p);};function initWS(){ws=new WebSocket(gW);ws.onopen=onOpen;ws.onclose=onClose;ws.onmessage=onMessage;}
function onOpen(event){ws.send("start");}
function onClose(event){setTimeout(initWS,2000);}
function onMessage(event){msg=JSON.parse(event.data);switch(msg['key']){case"start":fillPage(msg);break;}}
var buttonStates=[0];var sliderStates=[0];var light_sensor=[0];var light_color=0;var feed_values=[0];var air_sensor=[0];var air_input=[0];function processButton(elem){var first=false;if(typeof elem=="string"){first=true;elem=document.getElementById(elem);}
num=parseInt(elem.id.split("")[1]);if(num<(buttonStates.length+1))
{if(buttonStates[num-1]==1){document.getElementById(elem.id+"_c").style.color="red";if(!first){buttonStates[num-1]=0};}else{document.getElementById(elem.id+"_c").style.color="green";if(!first){buttonStates[num-1]=0};}}else if(num==(buttonStates.length+1)){document.getElementById(elem.id+"_c").style.color="green";setTimeout(function(){document.getElementById(elem.id+"_c").style.color="black"},250);}}
function fillPage(msg){console.log(msg);buttonStates=msg['btn'];sliderStates=msg['sld'];light_sensor=msg['llm'];light_color=msg['clr'];feed_values=msg['fvl'];air_sensor=msg['ase'];air_input=msg['ain'];if(typeof msg['prf']!="undefined"){ws.send(msg['prf']);}}
function processSlider(elem){document.getElementById(elem.id+"_v").innerHTML=elem.value;}
profile_name_climate=["Tropical","Dry","Temperate","Continental"];profile_name_stage=["Seedling","Vegetative","Budding","Ripening"];profile_name_type=["Common","Leaf","Fruit","Flower"];profile_name_mode=["Phase","Stable","Custom","Stress"];function concName(n1,n2,n3,n4){return(profile_name_climate[n1]+
" "+
profile_name_stage[n2]+
" "+
profile_name_type[n3]+
" "+
profile_name_mode[n4]);}
function concNameFromByte(b){n1=b&0b11000000>>>6;n2=b&0b00110000>>>4;n3=b&0b00001100>>>2;n4=b&0b00000011;return concName(n1,n2,n3,n4);}
function profileOptions(optionlist){select=document.getElementById("pss_sct");for(var i=0;i<optionlist.length;i++){var option=document.createElement("option");option.setAttribute("value",i);option.innerHTML=concNameFromByte(optionlist[i]);select.appendChild(option);}}
function fillProfiles(){for(var i=1;i<5;i++){switch(i){case 1:prof_array=profile_name_climate;break;case 2:prof_array=profile_name_stage;break;case 3:prof_array=profile_name_type;break;case 4:prof_array=profile_name_mode;break;}
opt_array=document.getElementsByName("prof_opt_"+String(i));for(var y=0;y<4;y++){opt_array[y].innerText=prof_array[y];}}}
function updatePage(){buttonStates.forEach((x,i)=>processButton("b"+(i+1)));sliderStates.forEach((x,i)=>processSlider(document.getElementById("s"+(i+1))));light_sensor.forEach((x,i)=>document.getElementById("l"+(i+1)).innerText=x);feed_values.forEach((x,i)=>document.getElementById("f"+(i+1)).value=x);air_sensor.forEach((x,i)=>document.getElementById("a"+(i+1)).innerText=x);air_input.forEach((x,i)=>document.getElementById("i"+(i+1)).value=x);document.getElementById("c1").value=light_color.toString(16);}
function setProfileRead(msg){pref="r";document.getElementById(pref+"p9").innerText=msg['onm']?msg['onm']<10:"0"+msg['onm'];document.getElementById(pref+"p7").innerText=msg['onm']?msg['onm']<10:"0"+msg['onm'];document.getElementById(pref+"p5").innerText=msg['col'];document.getElementById(pref+"p6").innerText=msg['onh'];document.getElementById(pref+"p8").innerText=msg['ofh'];document.getElementById(pref+"p10").innerText=msg['fin'];document.getElementById(pref+"p11").innerText=msg['fqy'];document.getElementById(pref+"p12").innerText=msg['tpd'];document.getElementById(pref+"p13").innerText=msg['tpn'];document.getElementById(pref+"p14").innerText=msg['hmd'];document.getElementById(pref+"p15").innerText=msg['hmn'];}
function setProfileWrite(){document.getElementById(pref+"p5").innerText=msg['col'];document.getElementById(pref+"p6").innerText=msg['onh'];document.getElementById(pref+"p8").innerText=msg['ofh'];document.getElementById(pref+"p10").innerText=msg['fin'];document.getElementById(pref+"p11").innerText=msg['fqy'];document.getElementById(pref+"p10").innerText=msg['fin'];document.getElementById(pref+"p11").innerText=msg['fqy'];document.getElementById(pref+"p10").innerText=msg['fin'];document.getElementById(pref+"p11").innerText=msg['fqy'];}