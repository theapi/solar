
#include "config.h"

#include "Payload.h"
#include <ESP8266WiFi.h>

// Use WiFiClient class to create TCP connections
WiFiClient client;
const uint16_t port = 23;
const char * host = "192.168.0.34"; // ip or dns



Payload rx_payload = Payload();



const long socket_timeout = 5000;
unsigned long socket_watchdog = 0;
unsigned long last_msg = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
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
  if (client.available()) {
    Serial.print("time diff: ");
    Serial.println(now - last_msg);
    socket_watchdog = now;
    last_msg = socket_watchdog;
    

    char buf[Payload_SIZE + 5];
    int nbytes;
    nbytes = client.readBytesUntil('\n', buf, Payload_SIZE + 5);
//    Serial.print("Number of bytes read: ");
//    Serial.println(nbytes);
//    Serial.println("buf: ");
//    Serial.println(buf);

//Serial.print(buf[0], HEX); Serial.print(' ');
//Serial.print(buf[1], HEX); Serial.print(' ');
//Serial.println();
   

    // Check for payload signifier;
    if (buf[0] == '\t') {
      
      uint8_t payload_buf[Payload_SIZE];
      memcpy(payload_buf, buf+1, Payload_SIZE);
      
     // rx_payload.unserialize((uint8_t*)buf);
      rx_payload.unserialize(payload_buf);
      //Serial.print("Payload unserialised: ");
      Serial.print(rx_payload.getDeviceId()); Serial.print(", ");
      Serial.print(rx_payload.getMsgId()); Serial.print(", ");
      Serial.print(rx_payload.getA()); Serial.print(", ");
      Serial.print(rx_payload.getB()); Serial.print(", ");
      Serial.print(rx_payload.getC()); Serial.print(", ");
      Serial.print(rx_payload.getD()); Serial.print(", ");
      Serial.print(rx_payload.getE()); Serial.print(", ");
      Serial.println(rx_payload.getF());
      Serial.println();
    }
    

    
  }
  
}

