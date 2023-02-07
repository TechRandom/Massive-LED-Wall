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
#define PAYLOAD_MAX 0x1F
#define PAYLOAD_MAX_G 0x3F
//-----------------------------------------------
// FastLED Gloabals
#include <FastLED.h>
#define WIDTH 16
#define HEIGHT 36
#define LED_PIN 2
#define MAX_BRIGHTNESS 200

const int NUM_LEDS = WIDTH * HEIGHT;
CRGB leds[NUM_LEDS];
//-----------------------------------------------
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";
//-----------------------------------------------

/*****************************************************************    
 *    webSocketEvent()
 *    Parameters: WStype_t type, uint8_t * payload, size_t length
 *    Returns: void
 * 
 *    On connection the server will ask "Who?" and expect a 
 *    response with the device number.
 *    Receives data from the server as a payload,
 *    then store that data into the LED array buffer before
 *    pushing the pixel data to the display.
 *****************************************************************/
void webSocketEvent(WStype_t type, uint8_t * payload, size_t welength) {
  // Check for data
  if (type == WStype_TEXT){
    if (strcmp((char *)payload, "Who?") == 0){
      webSocket.sendTXT("Device 4");
      return;
    }
    // Loop through LED array
    for (int i = 0; i < NUM_LEDS; i++){
      // Seperate payload data into individual colors
      // Get the first 5 bits of byte 1
      int red = *(payload + (2*i)) >> 3;
      // Get the last 3 bits of byte 1 and the first 3 bits of byte 2
      int green = ((*(payload + (2*i)) & 0x07) << 3) | (*(payload + (2*i) + 1) >> 5);
      // Get the last 5 bits of byte 
      int blue = *(payload + (2*i) + 1) & 0x1F;
      leds[i].r = map(red, 0, PAYLOAD_MAX, 0, MAX_BRIGHTNESS);
      leds[i].g = map(green, 0, PAYLOAD_MAX_G, 0, MAX_BRIGHTNESS);
      leds[i].b = map(blue, 0, PAYLOAD_MAX, 0, MAX_BRIGHTNESS);
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
