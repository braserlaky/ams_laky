/* -----------------------------------------
  themes.js v.0.14
  part of Arduino Mega Server project
  Themes functions
-------------------------------------------- */

var strLED1 = "";
var strRD2 = "";
var reload = false;

var oldValRD2 = "";
var oldWorkMode = "";

function cycleThemes() {
  var request = new XMLHttpRequest();
  request.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        if (this.responseXML != null) {
          var count;
        
          // RD2
          var valRD2 = this.responseXML.getElementsByTagName('RD2')[0].childNodes[0].nodeValue;

          if (oldValRD2 != "" && oldValRD2 != valRD2) {reload = true;}

          switch (valRD2) {
            case "1": document.getElementById("rd1").checked = true; break;
            case "2": document.getElementById("rd2").checked = true; break;
            case "3": document.getElementById("rd3").checked = true; break;
            case "4": document.getElementById("rd4").checked = true; break;
            case "5": document.getElementById("rd5").checked = true; break;
            case "6": document.getElementById("rd6").checked = true; break;
            case "8": document.getElementById("rd8").checked = true; break;
            default:  document.getElementById("rd7").checked = true;
          }
          oldValRD2 = valRD2;
          
        } // if (this.responseXML != null)
      } // if (this.status == 200)
    } // if (this.readyState == 4)
  } // request.onreadystatechange = function()

  if (reload) {
    reload = false;
    alert('Сейчас стиль оформления будет изменён. Перезагрузка может занять несколько секунд...')
    location.reload();
  }

  // send HTTP GET request with strCommands
  request.open("GET", "request_themes" + strLED1 + strRD2 + randomNoCache(), true);
  request.send(null);
  setTimeout('cycleThemes()', 1000);
  strLED1 = "";
  strRD2 = "";

} // cycleSettings()

function setRadio() {
  strRD2 = "&RD2=0";
  if (document.getElementById("rd1").checked) {strRD2 = "&RD2=1";}
  if (document.getElementById("rd2").checked) {strRD2 = "&RD2=2";}
  if (document.getElementById("rd3").checked) {strRD2 = "&RD2=3";}
  if (document.getElementById("rd4").checked) {strRD2 = "&RD2=4";}
  if (document.getElementById("rd5").checked) {strRD2 = "&RD2=5";}
  if (document.getElementById("rd6").checked) {strRD2 = "&RD2=6";}
  if (document.getElementById("rd8").checked) {strRD2 = "&RD2=8";}  
  if (document.getElementById("rd7").checked) {strRD2 = "&RD2=7";}
}

/* end */