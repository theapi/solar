
#include "config.h"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


unsigned long previousMillis = 0;
const long interval = 100;

const long ping_interval = 3000;
unsigned long ping_last = 0;
uint8_t ping_msg_id = 0;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

// Multicast declarations
//IPAddress ipMulti(192, 168, 0, 77);
IPAddress ipMulti(239, 0, 0, 57);
unsigned int portMulti = 12345;      // local port to listen on

void setup() {
  Serial.begin(115200);
  Serial.println();

  
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }


 // print your IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  Serial.println("Sending UDP multicast packet");
  // send a reply, to the IP address and port that sent us the packet we received
  Udp.beginPacketMulticast(ipMulti, portMulti,WiFi.localIP());
  Udp.write(++ping_msg_id);
  Udp.write(" - UDP Multicast packet sent by ");
  Udp.println(WiFi.localIP());
  Udp.endPacket();  

  delay(3000);
  
}

