/* -----------------------------------------   

  settings.js v.0.13
  part of Arduino Mega Server project
  Settings functions
-------------------------------------------- */

var strLED1 = "";
var strLED2 = "";
var reload = false;
var oldWorkMode = "";

function cycleSettings() {
  var request = new XMLHttpRequest();
  request.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        if (this.responseXML != null) {

          // Work Mode One/Major
          var workMode = this.responseXML.getElementsByTagName('LED')[0].childNodes[0].nodeValue;
          
          if (oldWorkMode != "" && oldWorkMode != workMode) {reload = true;}
          
          if (workMode === "checked") {
            document.LED_form.LED1.checked = true;
          } else {
              document.LED_form.LED1.checked = false;
            }
          oldWorkMode = workMode;
                 

          // Force Mode
          var forceMode = this.responseXML.getElementsByTagName('LED2')[0].childNodes[0].nodeValue;
          
          if (forceMode === "checked") {
            document.LED_form.LED2.checked = true;
          } else {
              document.LED_form.LED2.checked = false;
            }
  
        } // if (this.responseXML != null)
      } // if (this.status == 200)
    } // if (this.readyState == 4)
  } // request.onreadystatechange = function()

  if (reload) {
    reload = false;
    alert('Сейчас режим работы будет изменён. Перезагрузка может занять несколько секунд...')
    location.reload();
  }

  // send HTTP GET request with strCommands
  request.open("GET", "request_settings" + strLED1 + strLED2 +randomNoCache(), true);
  request.send(null);
  setTimeout('cycleSettings()', 1000);
  strLED1 = "";
  strLED2 = "";
} // cycleSettings()

function GetCheck() {
  if (LED_form.LED1.checked) {
    strLED1 = "&LED1=1";
  } else {
      strLED1 = "&LED1=0";
    }
}

function GetCheck2() {
  if (LED_form.LED2.checked) {
    strLED2 = "&LED2=1";
  } else {
      strLED2 = "&LED2=0";
    }
}

/* end */