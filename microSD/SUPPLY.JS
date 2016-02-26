/* -----------------------------------------123456789012345890123451234567890
12345678901234567890123456789012345678901234567890123456789012345678901234567890
12345678901234567890123456789012345678901234567890123456789012345678901234567890
12345678901234567890123456789012345678901234567890123456789012345678901234567890
12345678901234567890123456789012345678901234567890123456789012345678901234567890
12345678901234567890123456789012345678901234567890123456789012345678901234567890
12345678901234567890123456789012345678901234567890123456789012345678901234567890
   

  supply.jupply Power functions
-------------------------------------------- */

function cycleElectroForm() {
  var request = new XMLHttpRequest();
  request.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        if (this.responseXML != null) {

          var temp1 = 0;
          var temp2 = 0;

          var num_an = this.responseXML.getElementsByTagName('u').length;
          for (count = 0; count < num_an; count++) {
            temp1 = this.responseXML.getElementsByTagName('u')[count].childNodes[0].nodeValue;
            bufferElectro6[num_an - count] = temp1;
          }

          var num_an = this.responseXML.getElementsByTagName('i').length;
          for (count = 0; count < num_an; count++) {
            temp2 = this.responseXML.getElementsByTagName('i')[count].childNodes[0].nodeValue;
            bufferElectro8[num_an - count] = temp2;
          }
          form('freq-1', 3, temp1, '', bufferElectro6, bufferElectro8, 2, 'rgba(4, 169, 174, 1)', 'rgba(204,9,51, 1)', 'rgba(44, 140, 255, 1)', 'rgba(255, 255, 255, 1)');
        } //if (this.responseXML != null)
      } // if (this.status == 200)
    } // if (this.readyState == 4)
  } // request.onreadystatechange = function()

  // send HTTP GET request
  request.open("GET", "req_el_freq" + randomNoCache(), true);
  request.send(null);
  setTimeout('cycleElectroForm()', 5000);
} // cycleElectroForm()

/* end */