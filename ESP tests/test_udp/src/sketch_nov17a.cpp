#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ESPAsyncUDP.h"

#include "mavlink/tamerlanchik/mavlink.h"

const char* ssid = "QuadRot";
const char* password = "";

ESP8266WiFiMulti WiFiMulti;
AsyncUDP udp;
const size_t kBufSize = 1000;
char buf[kBufSize];
const size_t kReceivePort = 8081;
const size_t kDebugSerialBaudrate = 115200;

void receiveMessage(const char* /*buf*/, const size_t /*len*/);
void handle_heartbeat(const mavlink_message_t*);

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  auto res = udp.listen(kReceivePort);
  if(!res) {
    Serial.println("Cannot bind to port; Returning...");
    return;
  }
  Serial.print("Listening port ");
  Serial.println(kReceivePort);
  udp.onPacket([](AsyncUDPPacket& packet) {
    Serial.print("UDP Packet Type: ");
    Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
    Serial.print(", From: ");
    Serial.print(packet.remoteIP());
    Serial.print(":");
    Serial.print(packet.remotePort());
    Serial.print(", To: ");
    Serial.print(packet.localIP());
    Serial.print(":");
    Serial.print(packet.localPort());
    Serial.print(", Length: ");
    Serial.print(packet.length());
    // Serial.print(", Data: ");
    // Serial.write(packet.data(), packet.length());
    Serial.println();
    receiveMessage((const char*)packet.data(), packet.length());
  });
  // udp.begin(kReceivePort);
  // delay(500);
}



void loop() {
  // int packetSize = udp.parsePacket();
  // if (packetSize) {
  //   Serial.print("Package received. Len=");
  //   Serial.println(packetSize);
  //   int len = udp.read(buf, kBufSize);
  //   if(len > 0) {
  //     buf[len] = '\0';
  //     Serial.println(buf);
  //     // sendAnswer();
  //   }
  //   receiveMessage(buf, len);
  // }
  delay(1000);
}

void receiveMessage(const char* buf, const size_t len) {
  mavlink_message_t message;
  mavlink_status_t status;
  Serial.println("Start parsing message");
  for (int i = 0; i < len; ++i) {
    if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &message, &status) == 1) {
      switch (message.msgid) {
      case MAVLINK_MSG_ID_HEARTBEAT:
          handle_heartbeat(&message);
          break;
      }
    }
  }
}
void handle_heartbeat(const mavlink_message_t* msg) {
  Serial.println("Got hearthbeat");
  mavlink_heartbeat_t heartbeat;
  mavlink_msg_heartbeat_decode(msg, &heartbeat);
  Serial.print("Got heartbeat from ");
  switch (heartbeat.type) {
      case MAV_TYPE_GENERIC:
          Serial.print("generic");
          break;
      case MAV_TYPE_FIXED_WING:
          Serial.print("Fixed wing");
          break;
      case MAV_TYPE_QUADROTOR:
          Serial.print("Quadrotor");
          break;
      default:
          Serial.print("other");
          break;
  }
  Serial.print(" vehicle\n");
  heartbeat.check_value++;
  auto client_ip = udp.remoteIP();
}

