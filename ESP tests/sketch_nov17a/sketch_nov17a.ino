#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "include/mavlink/tamerlanchik/mavlink.h"

const char* ssid = "QuadRot";
const char* password = "";

ESP8266WiFiMulti WiFiMulti;
WiFiUDP udp;
const size_t kBufSize = 1000;
char buf[kBufSize];
const size_t kReceivePort = 8081;
const size_t kDebugSerialBaudrate = 115200;


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
  udp.begin(kReceivePort);
  delay(500);
}

void receiveMessage(const char* /*buf*/, const size_t /*len*/);
void handle_heartbeat(const mavlink_message_t*);

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Package received. Len=");
    Serial.println(packetSize);
    int len = udp.read(buf, kBufSize);
    if(len > 0) {
      buf[len] = '\0';
      Serial.println(buf);
      // sendAnswer();
    }
    receiveMessage(buf, len);
  }
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
#define printf Serial.println;
  Serial.println("Got hearthbeat");
  mavlink_heartbeat_t heartbeat;
  mavlink_msg_heartbeat_decode(msg, &heartbeat);
  printf("Got heartbeat from ");
  switch (heartbeat.type) {
      case MAV_TYPE_GENERIC:
          printf("generic");
          break;
      case MAV_TYPE_FIXED_WING:
          printf("Fixed wing");
          break;
      case MAV_TYPE_QUADROTOR:
          printf("Quadrotor");
          break;
      default:
          printf("other");
          break;
  }
  printf(" vehicle\n");
}

