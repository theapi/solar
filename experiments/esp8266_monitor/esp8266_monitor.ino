
#include "config.h"
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "Payload.h"
#include <ESP8266WiFi.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1


WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);

Payload rx_payload = Payload();
uint8_t input_string[Payload_SIZE];
uint8_t payload_state = 0;
uint8_t serial_byte_count = 0;

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

  // Socket server.
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }

  server.begin();
  server.setNoDelay(true);
  
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
}

void loop() {



  while (Serial.available()) {
    // get the new byte:
    uint8_t in = (uint8_t) Serial.read();
    
    
    if (payload_state == 0) {
      // Check for the start of the payload
      // which is device id "1".
      if (in == 1) {
        payload_state = 1;
      }
    }
    
    if (payload_state == 1) {
      Serial.print(in, HEX);
      // add it to the inputString:
      input_string[serial_byte_count] = in;
      ++serial_byte_count;
      
      // if the the last byte is received, set a flag
      // so the main loop can do something about it:
      if (serial_byte_count == Payload_SIZE - 1) {
        serial_byte_count = 0;
        payload_state = 2;
        rx_payload.unserialize(input_string);
        Serial.println();
        int id = rx_payload.getMsgId();
        int A = rx_payload.getA();
        int B = rx_payload.getB();
        Serial.print(id); Serial.print(" ");
        Serial.print(A); Serial.print(" ");
        Serial.print(B); Serial.print(" ");
        Serial.println();
      }
    }
  }









  
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) {
          serverClients[i].stop();
        }
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        // Ignore client input
        serverClients[i].flush();
      }
    }
  }

  if (payload_state == 2) {
    payload_state = 0;
    size_t len = Payload_SIZE;
    uint8_t sbuf[len];
    rx_payload.serialize(sbuf);
    
    //push payload data to all connected clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }


  display.clear();

  display.drawString(0, 0, String(millis()));
  display.drawString(0, 20, String(rx_payload.getMsgId()));
  display.drawString(0, 45, String(rx_payload.getA()));
  display.drawString(80, 45, String(rx_payload.getB())); 
  
  display.display();
  
  delay(10);
}

