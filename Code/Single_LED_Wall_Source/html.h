/*****************************************************************    
 *    htmlCode[]
 * 
 *    Store the webpage in Program Memory during upload
 *****************************************************************/
const char htmlCode[] PROGMEM =
R"=====(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP Web Server</title>
</head>

<body>
   <h1>The ESP8266 is ready...</h1>
  <script>
    websock = new WebSocket('ws://'+window.location.hostname+':81/'); 
  </script>
</body>

</html>
)=====";
