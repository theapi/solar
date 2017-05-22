
#include "config.h"

#include "Payload.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

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

WiFiUDP Udp;
unsigned int localUdpPort = 12345;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back



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

  Udp.beginMulticast(WiFi.localIP(), ipMulti, portMulti);
  Serial.printf("Now listening to IP %s, UDP port %d\n", ipMulti.toString().c_str(), localUdpPort);

}

void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

//    // send back a reply, to the IP address and port we got the packet from
//    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//    Udp.write(replyPacekt);
//    Udp.endPacket();
  }
  
}

