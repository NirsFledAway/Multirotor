#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "jute.h"
//
#include <String>

/* Установите здесь свои SSID и пароль */
const char* ssid = "The Gate";       // SSID
const char* password = "143900600";  // пароль

/* Настройки IP адреса */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LED1pin = D6;
bool LED1status = LOW;

void setup() 
{
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // подключиться к вашей локальной wi-fi сети
  WiFi.begin(ssid, password);

  // проверить, подключился ли wi-fi модуль к wi-fi сети
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("." + String(WiFi.status()));
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/cmd", HTTP_POST, handleCmd);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
  if(LED1status)
    digitalWrite(LED1pin, HIGH);
  else
    digitalWrite(LED1pin, LOW);
}

void handle_OnConnect() 
{ 
  Serial.print("GPIO6 Status: ");
  if(LED1status)
    Serial.print("ON");
  else
    Serial.print("OFF");

  Serial.println("");
  server.send(200, "text/html", SendHTML(LED1status)); 
}

void handle_led1on() 
{
  LED1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_led1off() 
{
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat)
{
  String ptr = "<!DOCTYPE html> <html>\n"
"<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"
"<title>LED Control</title>\n"
"<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"
"body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n"
".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n"
".button-on {background-color: #1abc9c;}\n"
".button-on:active {background-color: #16a085;}\n"
".button-off {background-color: #34495e;}\n"
".button-off:active {background-color: #2c3e50;}\n"
"p {font-size: 14px;color: #888;margin-bottom: 10px;}\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<h1>ESP8266 Web Server</h1>\n"
"<h3>Using Access Point(AP) Mode</h3>\n";
  
  if(led1stat)
    ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  else
    ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

struct CmdInput {
  float thrust;
  float roll;
  float pitch;
  String serialize() {
    String dest = "";
    dest += "{";
    dest += "\"thrust\":" + String(thrust) + ",";
    dest += "\"roll\":" + String(roll) + ","; 
    dest += "\"pitch\":" + String(pitch);
    dest += "}";
    return dest;
  };

  void deserialize(String src) {
    jute::jValue value = jute::parser::parse(src.c_str());
    thrust = value["thrust"].as_double();
    roll = value["roll"].as_double();
    pitch = value["pitch"].as_double();
  }
};



void handleCmd() {
  Serial.println("Server args: " + String(server.args()));
  if (server.args() < 1)
  {
    Serial.println("Bad request");
    server.send(400, "text/json","Bad payload"); 
    return;
  }
  String body = server.arg(0);

  CmdInput data;
  data.deserialize(body);
  Serial.println("T: " + String(data.thrust) + ", R: " + String(data.roll) + ", P: " + String(data.pitch));
  
  server.send(200, "text/json","200");  
}
