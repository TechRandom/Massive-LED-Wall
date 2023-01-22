//-----------------------------------------------
//                Tech Random DIY
//               LED_WALL_Receiver
//                 Chris Parker
//-----------------------------------------------
// WebSocket Globals
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;
//-----------------------------------------------
// FastLED Gloabals
#include <FastLED.h>
#define WIDTH 16
#define HEIGHT 36
#define LED_PIN 2

const int NUM_LEDS = WIDTH * HEIGHT;
CRGB leds[NUM_LEDS];
//-----------------------------------------------
const char* ssid = "WiFi art thou Romeo?";
const char* password = "Never Eat Sour Watermelon";
//-----------------------------------------------

/*****************************************************************    
 *    webSocketEvent()
 *    Parameters: WStype_t type, uint8_t * payload, size_t length
 *    Returns: void
 * 
 *    Receive data from the server as a payload,
 *    then store that data into the LED array buffer before
 *    pushing the pixel data to the display.
 *****************************************************************/
void webSocketEvent(WStype_t type, uint8_t * payload, size_t welength) {
  // Check for data
  if (type == WStype_TEXT){
    // Loop through LED array
    for (int i = 0; i < NUM_LEDS; i++){
      // Copy payload data to led array
      leds[i].r = *(payload + (3*i));
      leds[i].g = *(payload + (3*i) + 1);
      leds[i].b = *(payload + (3*i) + 2);
    }
    // Refresh Display
    FastLED.show();
  }
}
/*****************************************************************    
 *    setup()
 * 
 *    Start serial, connect to WiFi, then start the websocket.
 *****************************************************************/
void setup() {
	Serial.begin(115200);
  // Initialize LED Array
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
//-----------------------------------------------
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
	WiFi.begin(ssid, password);

	while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
		delay(500);
	}
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
//-----------------------------------------------
	// server address, port and URL
	webSocket.begin("192.168.1.121", 81, "/");
	// event handler
	webSocket.onEvent(webSocketEvent);
  // try again if connection has failed
  webSocket.setReconnectInterval(5000);
//-----------------------------------------------
  leds[0].r = 100;
  FastLED.show();
}
/*****************************************************************    
 *    loop()
 * 
 *    Handles web socket.
 *****************************************************************/
void loop() {
	webSocket.loop();
}
