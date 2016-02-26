/* -----------------------------------------   

  solder.js for Due 0.14
  part of Arduino Mega Server project
  nooLite functions
-------------------------------------------- */

var noo1stt = "";
var noo2stt = "";

var noo1state = true;
var noo2state = true;

var noo2Multi_10 = false;
var noo2Multi_20 = false;
var noo2Multi_30 = false;
var noo2Multi_40 = false;
var noo2Multi_50 = false;
var noo2Multi_60 = false;
var noo2Multi_70 = false;
var noo2Multi_80 = false;
var noo2Multi_90 = false;

var noo2Multi = "";

var nooChannel = "";
var nooBind = "";


function gliphColor(state) {
  switch (state) {
    case "0": return "#555555"; break;
    case "1": return "#e00"; break;
     default: return "#aaaaaa"; break;
  }
}

function textGliphColor(state) {
  switch (state) {
    case "0": return "#555555"; break;
    case "1": return "#98D52F"; break;
     default: return "#aaaaaa"; break;
  }
}

function gliphMultiColor(state) {
  switch (state) {
    case "0": return "#999999"; break;
    case "10": return "#fff"; break;
    case "20": return "#fff"; break;
    case "30": return "#fff"; break;
    case "40": return "#fff"; break;
    case "50": return "#fff"; break;
    case "60": return "#fff"; break;
    case "70": return "#fff"; break;
    case "80": return "#fff"; break;
    case "90": return "#fff"; break;
      default: return "#aaaaaa"; break;
  }
}

function getNoo() {
  var request = new XMLHttpRequest();

  request.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        if (this.responseXML != null) {
        
          try {
            var n1state = this.responseXML.getElementsByTagName('noo1state')[0].childNodes[0].nodeValue;
          } catch (err) {
              n1state = "-";
            }          
          try {
            document.getElementById("noo1-state-gliph").style.color = gliphColor(n1state);

            if (n1state == "1") {
              noo1state = true;
            } 
            else if (n1state == "0") {
              noo1state = false;
            } else {
                document.getElementById("noo1-state-text").style.color = '#555555';
              }
          } catch (err) { }
          
          try {
            var n2state = this.responseXML.getElementsByTagName('noo2state')[0].childNodes[0].nodeValue;
          } catch (err) {
              n2state = "-";
            }
          try {
            document.getElementById("noo2-state-gliph").style.color = gliphColor(n2state);

            if (n2state == "1") {
              noo2state = true;
            } 
            else if (n2state == "0") {
              noo2state = false;
            } else {
                document.getElementById("noo2-state-text").style.color = '#555555';
              }
          } catch (err) { }
          
          try {
            var n2mlt = this.responseXML.getElementsByTagName('noo2multi')[0].childNodes[0].nodeValue;
          } catch (err) {
              n2mlt = "-";
            }

            document.getElementById("noo2-multi10-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi20-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi30-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi40-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi50-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi60-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi70-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi80-gliph").style.color = gliphMultiColor("0");
            document.getElementById("noo2-multi90-gliph").style.color = gliphMultiColor("0");
            
            document.getElementById("noo2-multi10-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi20-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi30-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi40-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi50-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi60-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi70-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi80-text").style.color = '#aaaaaa';
            document.getElementById("noo2-multi90-text").style.color = '#aaaaaa';
            
            document.getElementById("noo2multi10").style.background = '#eee';
            document.getElementById("noo2multi20").style.background = '#eee';
            document.getElementById("noo2multi30").style.background = '#eee';
            document.getElementById("noo2multi40").style.background = '#eee';
            document.getElementById("noo2multi50").style.background = '#eee';
            document.getElementById("noo2multi60").style.background = '#eee';
            document.getElementById("noo2multi70").style.background = '#eee';
            document.getElementById("noo2multi80").style.background = '#eee';
            document.getElementById("noo2multi90").style.background = '#eee';
            
            if (n2mlt == "10") {
              document.getElementById("noo2-multi10-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi10-text").style.color = '#fff';
              document.getElementById("noo2multi10").style.background = '#FF8126';
            } 
            else if (n2mlt == "20") {
              document.getElementById("noo2-multi20-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi20-text").style.color = '#fff';
              document.getElementById("noo2multi20").style.background = '#FF8126';
            }
            else if (n2mlt == "30") {
              document.getElementById("noo2-multi30-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi30-text").style.color = '#fff';
              document.getElementById("noo2multi30").style.background = '#FF8126';
            }            
            else if (n2mlt == "40") {
              document.getElementById("noo2-multi40-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi40-text").style.color = '#fff';
              document.getElementById("noo2multi40").style.background = '#FF8126';
            }            
            else if (n2mlt == "50") {
              document.getElementById("noo2-multi50-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi50-text").style.color = '#fff';
              document.getElementById("noo2multi50").style.background = '#FF8126';
            }            
            else if (n2mlt == "60") {
              document.getElementById("noo2-multi60-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi60-text").style.color = '#fff';
              document.getElementById("noo2multi60").style.background = '#FF8126';
            }
            else if (n2mlt == "70") {
              document.getElementById("noo2-multi70-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi70-text").style.color = '#fff';
              document.getElementById("noo2multi70").style.background = '#FF8126';
            }
            else if (n2mlt == "80") {
              document.getElementById("noo2-multi80-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi80-text").style.color = '#fff';
              document.getElementById("noo2multi80").style.background = '#FF8126';
            }
            else if (n2mlt == "90") {
              document.getElementById("noo2-multi90-gliph").style.color = gliphMultiColor(n2mlt);
              document.getElementById("noo2-multi90-text").style.color = '#fff';
              document.getElementById("noo2multi90").style.background = '#FF8126';
            }
          
        } // if (this.responseXML != null)
      } // if (this.status == 200)
    } // if (this.readyState == 4)
  } // request.onreadystatechange = function()

  // send HTTP GET request with LEDs to switch on/off if any
  request.open("GET", "request_noo" + noo1stt + noo2stt + noo2Multi + nooChannel + nooBind + randomNoCache(), true);
  request.send(null);
  setTimeout('getNoo()', 2000);
  
  noo1stt = "";
  noo2stt = "";
  
  nooChannel = "";
  nooBind = "";
  noo2Multi = ""; 

} // getNoo()

/* -----------------------------------------
  actions
-------------------------------------------- */

function noo1StateAction() {
  if (noo1state) {
    document.getElementById("noo1-state-text").style.color = '#aaaaaa';
    noo1stt = "&noo1state=0";
  } else {
      document.getElementById("noo1-state-text").style.color = '#777777';
      noo1stt = "&noo1state=1";
    }
}

function noo2StateAction() {
  if (noo2state) {
    document.getElementById("noo2-state-text").style.color = '#aaaaaa';
    noo2stt = "&noo2state=0";
  } else {
      document.getElementById("noo2-state-text").style.color = '#777777';
      noo2stt = "&noo2state=1";
    }
}

function noo2MultiClear() {
  noo2Multi_10 = false;
  noo2Multi_20 = false;
  noo2Multi_30 = false;
  noo2Multi_40 = false;
  noo2Multi_50 = false;
  noo2Multi_60 = false;
  noo2Multi_70 = false;
  noo2Multi_80 = false;
  noo2Multi_90 = false;
  
  document.getElementById("noo2-multi10-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi20-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi30-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi40-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi50-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi60-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi70-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi80-text").style.color = '#aaaaaa';
  document.getElementById("noo2-multi90-text").style.color = '#aaaaaa';
}

function noo2Multi10() {noo2MultiClear(); noo2Multi = "&noo2multi=10"; noo2Multi_10 = true; document.getElementById("noo2-multi10-text").style.color = '#777777';}
function noo2Multi20() {noo2MultiClear(); noo2Multi = "&noo2multi=20"; noo2Multi_20 = true; document.getElementById("noo2-multi20-text").style.color = '#777777';}
function noo2Multi30() {noo2MultiClear(); noo2Multi = "&noo2multi=30"; noo2Multi_30 = true; document.getElementById("noo2-multi30-text").style.color = '#777777';}
function noo2Multi40() {noo2MultiClear(); noo2Multi = "&noo2multi=40"; noo2Multi_40 = true; document.getElementById("noo2-multi40-text").style.color = '#777777';}
function noo2Multi50() {noo2MultiClear(); noo2Multi = "&noo2multi=50"; noo2Multi_50 = true; document.getElementById("noo2-multi50-text").style.color = '#777777';}
function noo2Multi60() {noo2MultiClear(); noo2Multi = "&noo2multi=60"; noo2Multi_60 = true; document.getElementById("noo2-multi60-text").style.color = '#777777';}
function noo2Multi70() {noo2MultiClear(); noo2Multi = "&noo2multi=70"; noo2Multi_70 = true; document.getElementById("noo2-multi70-text").style.color = '#777777';}
function noo2Multi80() {noo2MultiClear(); noo2Multi = "&noo2multi=80"; noo2Multi_80 = true; document.getElementById("noo2-multi80-text").style.color = '#777777';}
function noo2Multi90() {noo2MultiClear(); noo2Multi = "&noo2multi=90"; noo2Multi_90 = true; document.getElementById("noo2-multi90-text").style.color = '#777777';}

function nooBindAction() {
  nooChannel = "&nooChannel=" + document.getElementById("channel").value;
  nooBind =    "&nooBind="    + nooBindForm.bindUnbind.value;
}

/* end */