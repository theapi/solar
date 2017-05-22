
#include "config.h"

#include "Payload.h"
#include <ESP8266WiFi.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 3


WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];
uint8_t clients[MAX_SRV_CLIENTS];



Payload rx_payload = Payload();
uint8_t input_string[Payload_SIZE];
uint8_t payload_state = 0;
uint8_t serial_byte_count = 0;
uint8_t msg_id = 0;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(115200);
  Serial.println();

  rx_payload.setMsgId(254);
  rx_payload.setA(0);
  rx_payload.setB(72);
  rx_payload.setC(105);
  
  
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


  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {

    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) {
          serverClients[i].stop();
        }
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.println(i);
        continue;
      }
    }
    
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
    Serial.println("Regected connection");
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

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    rx_payload.setMsgId(++msg_id);
    rx_payload.setA((int)currentMillis);
    size_t len = Payload_SIZE;
    uint8_t sbuf[len];
    rx_payload.serialize(sbuf);
    
    //push payload data to all connected clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write('\t'); // Payload start byte
        serverClients[i].write(sbuf, len);
        serverClients[i].write('\n');
        //delay(1);
      }
    }
  }

}

