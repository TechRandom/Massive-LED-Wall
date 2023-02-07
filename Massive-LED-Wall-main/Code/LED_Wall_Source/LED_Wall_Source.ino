//-----------------------------------------------
//                Tech Random DIY
//               LED_WALL_Source
//                 Chris Parker
//-----------------------------------------------
// WebSocket Globals
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#include "html.h"   //Seperate file with webpage
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//-----------------------------------------------
// LED Gloabals
#define WIDTH 64
#define HEIGHT 36
const int NUM_LEDS = WIDTH * HEIGHT;
char panel1[NUM_LEDS * 2];
char panel2[NUM_LEDS * 2];
char panel3[NUM_LEDS * 2];
char panel4[NUM_LEDS * 2];
//-----------------------------------------------
// Client Globals (One for each screen)
uint8_t screen1 = 99;
uint8_t screen2 = 99; 
uint8_t screen3 = 99;
uint8_t screen4 = 99; 
//-----------------------------------------------
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";
//-----------------------------------------------
// Set a Static IP address
IPAddress local_IP(192, 168, 1, 121);
// Set a Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
//-----------------------------------------------
/*****************************************************************    
 *    webSocketEvent()
 * 
 *    For now the server only sends data so this is empty.
 *****************************************************************/
void webSocketEvent(uint8_t clientNum, WStype_t type, uint8_t *payload, size_t welength) {
  switch(type) {
    case WStype_DISCONNECTED:
      disconnectClient(clientNum);
      break;
    case WStype_CONNECTED:
      webSocket.sendTXT(clientNum, "Who?");
      break;
    case WStype_TEXT:
      // the payload is a string containing the message from the client
      String message = String((char*)payload);
      // check if the message starts with "Device "
      if (message.startsWith("Device ")) {
        // extract the device number from the message
        saveClientNum(clientNum, message.substring(7).toInt());
      }
      break;
  }
}
/*****************************************************************    
 *    webpage()
 * 
 *    This function sends the webpage to clients
 *****************************************************************/
void webpage()
{
  server.send(200,"text/html", htmlCode);
}
/*****************************************************************    
 *    setDeviceNum()
 * 
 *    This function saves each screens clients number
 *****************************************************************/
void saveClientNum(int clientNum, int screenNum){
  switch (screenNum){
    case 1:
      screen1 = clientNum;
      webSocket.broadcastTXT("C1");
      break;
    case 2:
      screen2 = clientNum;
      webSocket.broadcastTXT("C2");
      break;
    case 3:
      screen3 = clientNum;
      webSocket.broadcastTXT("C3");
      break;
    case 4:
      screen4 = clientNum;
      webSocket.broadcastTXT("C4");
      break;
  }
  return;
}
/*****************************************************************    
 *    disconnectClient()
 * 
 *    Broadcast the client disconnect message
 *****************************************************************/
void disconnectClient(int clientNum){
  webSocket.broadcastTXT("Disconnected");
  if (clientNum == screen1)
      webSocket.broadcastTXT("D1");
  if (clientNum == screen2)
      webSocket.broadcastTXT("D2");
  if (clientNum == screen3)
      webSocket.broadcastTXT("D3");
  if (clientNum == screen4)
      webSocket.broadcastTXT("D4");
}
/*****************************************************************    
 *    setup()
 * 
 *    Connect to WiFi, start the server, start the websocket 
 *    connection, then start serial. Serial connection must be as 
 *    fast as possible for the best frame rate.
 *****************************************************************/
void setup() {
  //Configure static IP address
  WiFi.config(local_IP, gateway, subnet);
  
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", webpage);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.begin(921600);
}
/*****************************************************************    
 *    loop()
 * 
 *    Handles server and web socket. Reads data over serial then
 *    broadcasts it to all clients.
 *****************************************************************/
void loop() {
  server.handleClient();
  webSocket.loop();
  // Read header from LMCSDH
  switch(Serial.read()){ 
    case 0x05: // Request for matrix definition
      Serial.println(WIDTH);
      Serial.println(HEIGHT);
      break;
    case 0x42: // Read frame data
      // NUM_LEDS * (2 BYTES PER PIXEL)
      Serial.readBytes(panel1, NUM_LEDS / 4 * 2);
      Serial.readBytes(panel2, NUM_LEDS / 4 * 2);
      Serial.readBytes(panel3, NUM_LEDS / 4 * 2);
      Serial.readBytes(panel4, NUM_LEDS / 4 * 2);
      
      //Send Screen Data to the correct client
      // NUM_LEDS * (2 BYTES PER PIXEL)
      webSocket.sendTXT(screen1, (char *)panel1, NUM_LEDS / 4 * 2);
      webSocket.sendTXT(screen2, (char *)panel2, NUM_LEDS / 4 * 2);
      webSocket.sendTXT(screen3, (char *)panel3, NUM_LEDS / 4 * 2);
      webSocket.sendTXT(screen4, (char *)panel4, NUM_LEDS / 4 * 2);

      Serial.write(0x06); //acknowledge
      break;
  }
}
