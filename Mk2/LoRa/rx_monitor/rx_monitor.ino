/**
 * Takes a serial payload from the LoRa client (LoRa_Radiohead_RX.ino)
 * and publishes it on various protocols.
 *
 * To subscribe to the UDP data: listen for broadcasts on 239.0.0.57 port 12345
 * To subscribe to the MQTT data: mosquitto_sub -t solar
 * To subscribe to the websocket go to WiFi.localIP() port 80
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include "config.h"
#include "SSD1306.h"
#include "Payload.h"
#include "GardenPayload.h"
#include "SignalPayload.h"
#include "html.h"

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

// Multicast declarations
IPAddress ipMulti(239, 0, 0, 57);
unsigned int portMulti = 12345;      // local port to listen on

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);

theapi::GardenPayload rx_payload = theapi::GardenPayload();
theapi::SignalPayload signal_payload = theapi::SignalPayload();
uint8_t input_string[theapi::GardenPayload::SIZE];
uint8_t payload_state = 0;
uint8_t current_payload;
uint8_t serial_byte_count = 0;

unsigned long previousMillis = 0;
const long interval = 100;

const long ping_interval = 3000;
unsigned long ping_last = 0;

const char* mqtt_server = MQTT_SERVER;
uint16_t mqtt_port = MQTT_PORT;

String ip_end;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "{\"status\": \"Connected\"}");
      }
      break;
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialise the display.
  display.init();
  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);

  rx_payload.setMsgId(254);
  rx_payload.setVcc(1234);
  rx_payload.setChargeMv(5678);

  display.clear();
  display.drawString(0, 20, String(rx_payload.getMsgId()));
  display.drawString(0, 45, String(rx_payload.getVcc()));
  display.drawString(80, 45, String(rx_payload.getChargeMv()));
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

  mqtt_client.setServer(mqtt_server, mqtt_port);

  Serial.print("Web server on: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ready! Listen for UDP broadcasts on 239.0.0.57 port 12345");

  String ip = WiFi.localIP().toString();
  ip_end = ip.substring(ip.lastIndexOf('.'));

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
    } else if (payload_state == 1) {

      // Check the first byte for the payload type.
      if (serial_byte_count == 0) {
        current_payload = in;
      }

      //Serial.print(in, HEX);
      // add it to the inputString:
      input_string[serial_byte_count] = in;
      ++serial_byte_count;

      // if the the last byte is received, set a flag
      // so the main loop can do something about it:
      switch (current_payload) {
        case theapi::Payload::SIGNAL:
          // Use SignalPayload
          if (serial_byte_count == signal_payload.size()) {
            serial_byte_count = 0;
            payload_state = 2;
            signal_payload.unserialize(input_string);
          }
          break;
        default:
          // Use GardenPayload
          if (serial_byte_count == rx_payload.size()) {
            serial_byte_count = 0;
            payload_state = 2;
            rx_payload.unserialize(input_string);
          }
          break;
      }
    } else {
      // Passthru other serial messages.
      Serial.print(char(in));

    }
  }

  unsigned long currentMillis = millis();

  // Send payload to listeners when ready.
  if (payload_state == 2) {
    payload_state = 0;

    // No need to ping if we're sending real data.
    ping_last = currentMillis;
    udpBroadcast();
    websocketBroadcast();
    mqttBroadcast();
    serialPrintPayload();

    current_payload = 0;
  }
  // Send the data continually, as its UDP some may get missed.
  else if (currentMillis - ping_last >= ping_interval) {
    ping_last = currentMillis;
    udpBroadcast();
  }

  // Update the display
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    display.clear();

    if (currentMillis < 1000000000) {
      // Show the edn of the ip address if the miils count isn't too large.
      display.drawString(100, 0, ip_end);
    }
    display.drawString(0, 0, String(currentMillis));

    display.drawString(0, 20, String(rx_payload.getMsgId()));
    display.drawString(35, 20, String(rx_payload.getVcc()));
    display.drawString(80, 20, String(rx_payload.getTemperature()));

//    display.drawString(0, 45, String(signal_payload.getRssi()));
//    display.drawString(35, 45, String(signal_payload.getSnr()));
//    display.drawString(80, 45, String(signal_payload.getFreqError()));

    display.drawString(0, 45, String(rx_payload.getChargeMv()));
    display.drawString(40, 45, String(rx_payload.getChargeMa()));
    display.drawString(80, 45, String(rx_payload.getLight()));

    display.display();
  }

}

void serialPrintPayload() {
  switch (current_payload) {
    case theapi::Payload::SIGNAL:
      Serial.print("SIGNAL: ");
      Serial.print(signal_payload.getMsgType()); Serial.print(", ");
      Serial.print(signal_payload.getMsgId()); Serial.print(", ");
      Serial.print(signal_payload.getRssi()); Serial.print(", ");
      Serial.print(signal_payload.getSnr()); Serial.print(", ");
      Serial.println(signal_payload.getFreqError());
      Serial.println();
    break;
    default:
      Serial.print("GARDEN: ");
      Serial.print(rx_payload.getMsgType()); Serial.print(", ");
      Serial.print(rx_payload.getMsgId()); Serial.print(", ");
      Serial.print(rx_payload.getVcc()); Serial.print(", ");
      Serial.print(rx_payload.getChargeMv()); Serial.print(", ");
      Serial.print(rx_payload.getChargeMa()); Serial.print(", ");
      Serial.print(rx_payload.getLight()); Serial.print(", ");
      Serial.print(rx_payload.getCpuTemperature()); Serial.print(", ");
      Serial.println(rx_payload.getTemperature());
      Serial.println();
    break;
  }
}

void udpBroadcast() {
  Udp.beginPacketMulticast(ipMulti, portMulti, WiFi.localIP());
  Udp.write('\t'); // Payload start byte

  // Send the contents of the buffer.
  switch (current_payload) {
    case theapi::Payload::SIGNAL:
      {
        size_t len = signal_payload.size();
        uint8_t sbuf[len];
        signal_payload.serialize(sbuf);
        Udp.write(sbuf, len);
      }
      break;
    default:
      {
        size_t len = rx_payload.size();
        uint8_t sbuf[len];
        rx_payload.serialize(sbuf);
        Udp.write(sbuf, len);
      }
      break;
  }

  Udp.write('\n');
  Udp.endPacket();
  Udp.stop();
}

void websocketBroadcast() {
  String str;
  switch (current_payload) {
    case theapi::Payload::SIGNAL:
      // Send the signal data.
      str = "{\"signal\":{";
      str += "\"msg_type\":" + String(signal_payload.getMsgType()) + ",";
      str += "\"msg_id\":" + String(signal_payload.getMsgId()) + ",";
      str += "\"rssi\":" + String(signal_payload.getRssi()) + ",";
      str += "\"snr\":" + String(signal_payload.getSnr()) + ",";
      str += "\"freq_error\":" + String(signal_payload.getFreqError());
      str += "}}";
      webSocket.broadcastTXT(str);
    break;
    default:
      // Send the garden data as json
      str = "{\"garden\":{";
      str += "\"msg_type\":" + String(rx_payload.getMsgType()) + ", ";
      str += "\"msg_id\":" + String(rx_payload.getMsgId()) + ",";
      str += "\"vcc\":" + String(rx_payload.getVcc()) + ",";
      str += "\"solar_mv\":" + String(rx_payload.getChargeMv()) + ",";
      str += "\"charge_ma\":" + String(rx_payload.getChargeMa()) + ",";
      str += "\"light\":" + String(rx_payload.getLight()) + ",";
      str += "\"cpu\":" + String(rx_payload.getCpuTemperature()) + ",";
      str += "\"temperature\":" + String(rx_payload.getTemperature());
      str += "}}";
      webSocket.broadcastTXT(str);
    break;
  }
}

bool mqttConnect() {
  // Connect if needed.
  if (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("SolarClient")) {
      Serial.println("connected");
      return true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      return true;
    }
  }
  return true;
}

void mqttBroadcast() {

  String str;
  char msg[50];

  switch (current_payload) {
    case theapi::Payload::SIGNAL:
    {
      // Send the signal data.
      str = String(signal_payload.getMsgType()) + ",";
      str += String(signal_payload.getMsgId()) + ",";
      str += String(signal_payload.getRssi()) + ",";
      str += String(signal_payload.getSnr()) + ",";
      str += String(signal_payload.getFreqError());
      memset(msg, 0, sizeof msg);
      str.toCharArray(msg, sizeof msg);
      if (mqttConnect()) {
        mqtt_client.publish("solar/signal", msg);
      }
    }
    break;
    default:
    {
      str = String(rx_payload.getMsgType()) + ",";
      str += String(rx_payload.getMsgId()) + ",";
      str += String(rx_payload.getVcc()) + ",";
      str += String(rx_payload.getChargeMv()) + ",";
      str += String(rx_payload.getChargeMa()) + ",";
      str += String(rx_payload.getLight()) + ",";
      str += String(rx_payload.getCpuTemperature()) + ",";
      str += String(rx_payload.getTemperature());
      memset(msg, 0, sizeof msg);
      str.toCharArray(msg, sizeof msg);
      if (mqttConnect()) {
        mqtt_client.publish("solar/garden", msg);
      }
    }
    break;
  }

}
