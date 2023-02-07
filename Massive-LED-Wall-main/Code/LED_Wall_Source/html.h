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
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background: rgb(0,185,255);
    background: linear-gradient(90deg, rgba(0,185,255,1) 0%, rgba(244,0,255,1) 50%, rgba(0,185,255,1) 100%);
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>

<body>
  <div class="topnav">
    <h1>LED Wall Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Connected Clients:</h2>
      <p class="state">Screen 1: <span id="state1">DISCONNECTED</span></p>
      <p class="state">Screen 2: <span id="state2">DISCONNECTED</span></p>
      <p class="state">Screen 3: <span id="state3">DISCONNECTED</span></p>
      <p class="state">Screen 4: <span id="state4">DISCONNECTED</span></p>
    </div>
  </div>
  <script>
    socket = new WebSocket('ws://'+window.location.hostname+':81/'); 
    // When a client connects, add their name to the list
    socket.onmessage = receiveMessage;

    function receiveMessage(event) {
      if (event.data == "C1"){
        document.getElementById('state1').innerHTML = "CONNECTED";
      }
      if (event.data == "C2"){
        document.getElementById('state2').innerHTML = "CONNECTED";
      }
      if (event.data == "C3"){
        document.getElementById('state3').innerHTML = "CONNECTED";
      }
      if (event.data == "C4"){
        document.getElementById('state4').innerHTML = "CONNECTED";
      }
      if (event.data == "D1"){
        document.getElementById('state1').innerHTML = "DISCONNECTED";
      }
      if (event.data == "D2"){
        document.getElementById('state2').innerHTML = "DISCONNECTED";
      }
      if (event.data == "D3"){
        document.getElementById('state3').innerHTML = "DISCONNECTED";
      }
      if (event.data == "D4"){
        document.getElementById('state4').innerHTML = "DISCONNECTED";
      }
    };
    
  </script>
</body>

</html>
)=====";
