/* -----------------------------------------   

  electro.js v.0.13
  part of Arduino Mega Server project
  Electro functions
-------------------------------------------- */

var button1str = "";
var button1state;

function cycleElectroControl() {
  var request = new XMLHttpRequest();
  request.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        if (this.responseXML != null) {

          // Button Electro ON/OFF
          var modElectro = this.responseXML.getElementsByTagName('modulElectro')[0].childNodes[0].nodeValue;
          if (modElectro != "2") {
            if (this.responseXML.getElementsByTagName('LED')[0].childNodes[0].nodeValue === "on") {
              document.getElementById("button-electro").innerHTML = "ON";
              document.getElementById("button-electro").style.background = modulActive;
              button1state = 1;
            } else {
                document.getElementById("button-electro").innerHTML = "OFF";
                document.getElementById("button-electro").style.background = modulPassive;
                button1state = 0;
              }
          }
        } // if (this.responseXML != null)
      } // if (this.status == 200)
    } // if (this.readyState == 4)
  } // request.onreadystatechange = function()

  // send HTTP GET request with strCommands
  request.open("GET", "req_el_control" + button1str + randomNoCache(), true);
  request.send(null);
  setTimeout('cycleElectroControl()', 1000);
  button1str = "";
} // cycleElectroControl()

function setButton1() {
  if (button1state === 1) {
    button1state = 0;
    button1str = "&btn1=0";
  } else {
      button1state = 1;
      button1str = "&btn1=1";
    }
}

/* end */