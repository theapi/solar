
#include "config.h"

#include "Payload.h"
#include <ESP8266WiFi.h>

// Use WiFiClient class to create TCP connections
WiFiClient client;
const uint16_t port = 23;
const char * host = "192.168.0.33"; // ip or dns



Payload rx_payload = Payload();
uint8_t input_string[Payload_SIZE];
uint8_t payload_state = 0;
uint8_t serial_byte_count = 0;


const long socket_timeout = 5000;
unsigned long socket_watchdog = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  rx_payload.setMsgId(254);
  rx_payload.setA(1234);
  rx_payload.setB(5678);
  
  // Socket server.
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }

  
}

void loop() {

// Process any waiting socket data.
  handleSocket();
  

}

void socketConnect(const char *host, uint16_t port) {
  if (!client.connected()) {
    Serial.print("connecting to ");
    Serial.println(host);
    if (!client.connect(host, port)) {
        Serial.println("...connection failed");
    }
    else {
      Serial.println("...connected");
    }
  }

}

void handleSocket() {

  unsigned long now = millis();
  if (now - socket_watchdog >= socket_timeout) {
    socket_watchdog = now;
    Serial.println("Socket timeout");
    client.stop();
    //@todo delay bfore trying to connect in the loop to avoid flooding.
  }
  
  // Connect to the socket server for the data feed.
  socketConnect(host, port);
  if (client.available()){
    socket_watchdog = now;
    String line = client.readStringUntil('\n');
    Serial.println(line);
//
//    uint8_t buf[Payload_SIZE];
//    rx_payload.unserialize(line);
//
//    Serial.print(rx_payload.getMsgId()); Serial.print(", ");
//    Serial.print(rx_payload.getA()); Serial.print(", ");
//    Serial.print(rx_payload.getB()); Serial.print(", ");
//    Serial.println();
  }
  
}

