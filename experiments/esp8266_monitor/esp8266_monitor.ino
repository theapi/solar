

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "SSD1306.h"
#include "Payload.h"
#include "html.h"

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

// Multicast declarations
IPAddress ipMulti(239, 0, 0, 57);
unsigned int portMulti = 12345;      // local port to listen on

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);

Payload rx_payload = Payload();
uint8_t input_string[Payload_SIZE];
uint8_t payload_state = 0;
uint8_t serial_byte_count = 0;

unsigned long previousMillis = 0;
const long interval = 100;

const long ping_interval = 3000;
unsigned long ping_last = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialise the display.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);

  rx_payload.setMsgId(254);
  rx_payload.setA(1234);
  rx_payload.setB(5678);
  
  display.clear();
  display.drawString(0, 20, String(rx_payload.getMsgId()));
  display.drawString(0, 45, String(rx_payload.getA()));
  display.drawString(80, 45, String(rx_payload.getB())); 
  display.display();

  
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }

    // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // handle index
  server.on("/", []() {
    // send index.html
    server.send(200, "text/html", index_html);
  });

  server.begin();

  Serial.print("Web server on: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ready! Listen for UDP broadcasts on 239.0.0.57 port 12345");
}

void loop() {
  webSocket.loop();
  server.handleClient();

  // Read the data from the LoRa receiver.
  while (Serial.available()) {
    // get the new byte:
    uint8_t in = (uint8_t) Serial.read();
    //Serial.println(in, HEX);
    if (payload_state == 0) {
      // Check for the start of the payload
      if (in == '\t') {
        payload_state = 1;
      }
    }
    
    else if (payload_state == 1) {
      //Serial.print(in, HEX);
      // add it to the inputString:
      input_string[serial_byte_count] = in;
      ++serial_byte_count;
      
      // if the the last byte is received, set a flag
      // so the main loop can do something about it:
      if (serial_byte_count == Payload_SIZE) {
        serial_byte_count = 0;
        payload_state = 2;
        rx_payload.unserialize(input_string);
//        Serial.print(rx_payload.getDeviceId()); Serial.print(", ");
//      Serial.print(rx_payload.getMsgId()); Serial.print(", ");
//      Serial.print(rx_payload.getA()); Serial.print(", ");
//      Serial.print(rx_payload.getB()); Serial.print(", ");
//      Serial.print(rx_payload.getC()); Serial.print(", ");
//      Serial.print(rx_payload.getD()); Serial.print(", ");
//      Serial.print(rx_payload.getE()); Serial.print(", ");
//      Serial.println(rx_payload.getF());
//      Serial.println();
      }
    } else {
      // Passthru other serial messages.
      //Serial.print(char(in));
      
    }
  }

  unsigned long currentMillis = millis();
  
  // Send payload to TCP clients when ready.
  if (payload_state == 2) {
    payload_state = 0;

    // No need to ping if we're sending real data.
    ping_last = currentMillis;
    broadcast_udp();
    broadcast_websocket();
  } 
  // Send the data continually, as its UDP some may get missed.
  else if (currentMillis - ping_last >= ping_interval) {
    ping_last = currentMillis;
    broadcast_udp();
    broadcast_websocket();
  }

  // Update the display  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    display.clear();
  
    display.drawString(0, 0, String(currentMillis));
    
    display.drawString(0, 20, String(rx_payload.getMsgId()));
    display.drawString(35, 20, String(rx_payload.getA()));
    display.drawString(80, 20, String(rx_payload.getB())); 
  
    display.drawString(0, 45, WiFi.localIP().toString());
    
    display.display();
  }
  
}

void broadcast_udp() {
    size_t len = Payload_SIZE;
    uint8_t sbuf[len];
    rx_payload.serialize(sbuf);
    Udp.beginPacketMulticast(ipMulti, portMulti, WiFi.localIP());
    Udp.write('\t'); // Payload start byte
    Udp.write(sbuf, len);
    Udp.write('\n');
    Udp.endPacket();  
    Udp.stop();  
}

void broadcast_websocket() {
  String str = String(rx_payload.getDeviceId()) + ",";
  str += String(rx_payload.getMsgId()) + ",";
  str += String(rx_payload.getA()) + ",";
  str += String(rx_payload.getB()) + ",";
  str += String(rx_payload.getC()) + ",";
  str += String(rx_payload.getD()) + ",";
  str += String(rx_payload.getE()) + ",";
  str += String(rx_payload.getF());
  webSocket.broadcastTXT(str);
}

