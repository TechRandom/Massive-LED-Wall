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
#define WIDTH 16
#define HEIGHT 36
const int NUM_LEDS = WIDTH * HEIGHT;
char leds[NUM_LEDS * 3];
//-----------------------------------------------
const char* ssid = "WiFi art thou Romeo?";
const char* password = "Never Eat Sour Watermelon";
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
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength) {
  return;
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
    case 0x41: // Read frame data
      Serial.readBytes(leds, NUM_LEDS * 3);
      //broadcast
      webSocket.broadcastTXT((char *)leds, NUM_LEDS * 3);
      Serial.write(0x06); //acknowledge
      break;
  }
}
