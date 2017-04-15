
#include "config.h"
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "Payload.h"
#include <ESP8266WiFi.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 2


WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);

Payload rx_payload = Payload();
Payload ping_payload = Payload();
uint8_t input_string[Payload_SIZE];
uint8_t payload_state = 0;
uint8_t serial_byte_count = 0;

unsigned long previousMillis = 0;
const long interval = 100;

const long ping_interval = 3000;
unsigned long ping_last = 0;
uint8_t ping_msg_id = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialise the display.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);

  ping_payload.setDeviceId(254);

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

  // Socket server.
  server.begin();
  server.setNoDelay(true);
  
  Serial.print("Ready! Use 'netcat ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
}

void loop() {


  // Read the data from the LoRa receiver.
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
      //Serial.print(in, HEX);
      // add it to the inputString:
      input_string[serial_byte_count] = in;
      ++serial_byte_count;
      
      // if the the last byte is received, set a flag
      // so the main loop can do something about it:
      if (serial_byte_count == Payload_SIZE - 1) {
        serial_byte_count = 0;
        payload_state = 2;
        rx_payload.unserialize(input_string);
//        Serial.println();
//        Serial.print(rx_payload.getDeviceId()); Serial.print(", ");
//        Serial.print(rx_payload.getMsgId()); Serial.print(", ");
//        Serial.print(rx_payload.getA()); Serial.print(", ");
//        Serial.print(rx_payload.getB()); Serial.print(", ");
//        Serial.print(rx_payload.getC()); Serial.print(", ");
//        Serial.print(rx_payload.getD()); Serial.print(", ");
//        Serial.print(rx_payload.getE()); Serial.print(", ");
//        Serial.println(rx_payload.getF());
        
      }
    } else {
      // Passthru other serial messages.
      //Serial.print(char(in));
      
    }
  }

  uint8_t i;
  // check if there are any new TCP clients
  if (server.hasClient()) {
    uint8_t allocated = 0;
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (allocated == 0) {
        //find free/disconnected spot
        if (!serverClients[i] || !serverClients[i].connected()) {
          if (serverClients[i]) {
            serverClients[i].stop();
          }
          serverClients[i] = server.available();
          serverClients[i].setNoDelay(true);
          serverClients[i].setTimeout(100);
          Serial.print("New client: "); Serial.println(i);
          allocated = 1;
        }
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  
  // check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        // Ignore client input
        serverClients[i].flush();
      }
    }
  }

  unsigned long currentMillis = millis();
  
  // Send payload to TCP clients when ready.
  if (payload_state == 2) {
    payload_state = 0;

    // No need to ping if we're sending real data.
    ping_last = currentMillis;
    
    size_t len = Payload_SIZE;
    uint8_t sbuf[len];
    rx_payload.serialize(sbuf);

    // push payload data to all connected clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()) {
        if (!serverClients[i].write('\t')) { // Payload start byte
          serverClients[i].stop();
        }
        else {
          serverClients[i].write(sbuf, len);
          serverClients[i].write('\n');
        }
      }
    }
  } 
  // Keep TCP clients connected with a ping.
  else if (currentMillis - ping_last >= ping_interval) {
    ping_last = currentMillis;

    ping_payload.setMsgId(++ping_msg_id);
    //ping_payload.setF(1234);
    // Send a ping message.
    size_t len = Payload_SIZE;
    uint8_t sbuf[len];
    ping_payload.serialize(sbuf);

    // push payload data to all connected clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()) {
        if (!serverClients[i].write('\t')) { // Payload start byte
          serverClients[i].stop();
          Serial.print("Stopped: ");
          Serial.println(i);
        }
        else {
          serverClients[i].write(sbuf, len);
          serverClients[i].write('\n');
        }
      }
    }
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

