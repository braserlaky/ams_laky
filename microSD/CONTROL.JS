/* -----------------------------------------   

  control.js v.0.13
  part of Arduino Mega Server project
  Settings functions
-------------------------------------------- */

var light1 = "";
var light2 = "";
var light3 = "";
var light4 = "";
var light5 = "";

var light1state = true;
var light2state = true;
var light3state = true;
var light4state = true;
var light5state = true;

function getControl() {
  var request = new XMLHttpRequest();

  request.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        if (this.responseXML != null) {

          try {
            var lite1 = this.responseXML.getElementsByTagName('lite1')[0].childNodes[0].nodeValue;
          } catch (err) {
              lite1 = "-";
            }          
          try {
            if (lite1 == "1") {
              document.getElementById("light1gliph").style.color = '#98D52F';
              light1state = true;
            } 
            else if (lite1 == "0") {
              document.getElementById("light1gliph").style.color = "#555555";
              light1state = false;
            } else {
                document.getElementById("light1gliph").style.color = "#333333";
                document.getElementById("light1text").style.color = '#555555';
              }
          } catch (err) { }
          
          try {
            var lite2 = this.responseXML.getElementsByTagName('lite2')[0].childNodes[0].nodeValue;
          } catch (err) {
              lite2 = "-";
            }          
          try {
            if (lite2 == "1") {
              document.getElementById("light2gliph").style.color = '#98D52F';
              light2state = true;
            } 
            else if (lite2 == "0") {
              document.getElementById("light2gliph").style.color = "#555555";
              light2state = false;
            } else {
                document.getElementById("light2gliph").style.color = "#333333";
                document.getElementById("light2text").style.color = '#555555';
              }
          } catch (err) { }

          try {
            var lite3 = this.responseXML.getElementsByTagName('lite3')[0].childNodes[0].nodeValue;
          } catch (err) {
              lite3 = "-";
            }          
          try {
            if (lite3 == "1") {
              document.getElementById("light3gliph").style.color = '#98D52F';
              light3state = true;
            } 
            else if (lite3 == "0") {
              document.getElementById("light3gliph").style.color = "#555555";
              light3state = false;
            } else {
                document.getElementById("light3gliph").style.color = "#333333";
                document.getElementById("light3text").style.color = '#555555';
              }
          } catch (err) { }

          try {
            var lite4 = this.responseXML.getElementsByTagName('lite4')[0].childNodes[0].nodeValue;
          } catch (err) {
              lite4 = "-";
            }          
          try {
            if (lite4 == "1") {
              document.getElementById("light4gliph").style.color = '#98D52F';
              light4state = true;
            } 
            else if (lite4 == "0") {
              document.getElementById("light4gliph").style.color = "#555555";
              light4state = false;
            } else {
                document.getElementById("light4gliph").style.color = "#333333";
                document.getElementById("light4text").style.color = '#555555';
              }
          } catch (err) { }

          try {
            var lite5 = this.responseXML.getElementsByTagName('lite5')[0].childNodes[0].nodeValue;
          } catch (err) {
              lite5 = "-";
            }          
          try {
            if (lite5 == "1") {
              document.getElementById("light5gliph").style.color = '#98D52F';
              light5state = true;
            } 
            else if (lite5 == "0") {
              document.getElementById("light5gliph").style.color = "#555555";
              light5state = false;
            } else {
                document.getElementById("light5gliph").style.color = "#333333";
                document.getElementById("light5text").style.color = '#555555';
              }
          } catch (err) { }          
          
        } // if (this.responseXML != null)
      } // if (this.status == 200)
    } // if (this.readyState == 4)
  } // request.onreadystatechange = function()

  // send HTTP GET request with LEDs to switch on/off if any
  request.open("GET", "request_control" + light1 + light2 + light3 + light4 + light5 + randomNoCache(), true);
  request.send(null);
  setTimeout('getControl()', 1000);
  
  light1 = "";
  light2 = "";
  light3 = "";
  light4 = "";
  light5 = "";
} // getControl()

/* -------------------------------------------
  service LEDs when checkbox checked/unchecked
---------------------------------------------- */

/* -----------------------------------------
  control
-------------------------------------------- */

function light1action() {
  if (light1state) {
    document.getElementById("light1text").style.color = '#777777';
    light1 = "&light1=0";
  } else {
      document.getElementById("light1text").style.color = '#aaaaaa';
      light1 = "&light1=1";
    }
}

function light2action() {
  if (light2state) {
    document.getElementById("light2text").style.color = '#777777';
    light2 = "&light2=0";
  } else {
      document.getElementById("light2text").style.color = '#aaaaaa';
      light2 = "&light2=1";
    }
}

function light3action() {
  if (light3state) {
    document.getElementById("light3text").style.color = '#777777';
    light3 = "&light3=0";
  } else {
      document.getElementById("light3text").style.color = '#aaaaaa';
      light3 = "&light3=1";
    }
}

function light4action() {
  if (light4state) {
    document.getElementById("light4text").style.color = '#777777';
    light4 = "&light4=0";
  } else {
      document.getElementById("light4text").style.color = '#aaaaaa';
      light4 = "&light4=1";
    }
}

function light5action() {
  if (light5state) {
    document.getElementById("light5text").style.color = '#777777';
    light5 = "&light5=0";
  } else {
      document.getElementById("light5text").style.color = '#aaaaaa';
      light5 = "&light5=1";
    }
}

/* end */