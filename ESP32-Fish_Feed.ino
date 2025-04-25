#include <WiFi.h>
#include <Wire.h>
#include <RTClib.h>
#include <ESP32Servo.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid     = "ESP32-WiFi";
const char* password = "123456666";

WebServer server(80);
RTC_DS3231 rtc;
Servo myservo;

int Hour, Minute, Second;
int HourRTC, MinuteRTC, SecondRTC;

void handleRoot() {
  String html = "<h1>Set RTC Time</h1>";
  html += "<form action='/setTime'>";
  html += "Hour: <input type='number' name='hour'><br>";
  html += "Minute: <input type='number' name='minute'><br>";
  html += "Second: <input type='number' name='second'><br>";
  html += "<input type='submit' value='Set Time'>";
  html += "</form>";

  server.send(200, "text/html", html);
}

void handleSetTime() {
  if (server.hasArg("hour") && server.hasArg("minute") && server.hasArg("second")) {
    Hour = server.arg("hour").toInt();
    Minute = server.arg("minute").toInt();
    Second = server.arg("second").toInt();

    server.send(200, "text/html", "<h2>RTC Time Updated!</h2><a href='/'>Back</a>");
  } else {
    server.send(400, "text/html", "<h2>Invalid Parameters</h2>");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));

  myservo.attach(4);  // pakai GPIO 4

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/setTime", handleSetTime);
  server.begin();
}

void loop() {
  server.handleClient();

  DateTime now = rtc.now();
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d\n", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  HourRTC = now.hour();
  MinuteRTC = now.minute();
  SecondRTC = now.second();

  if (HourRTC == Hour && MinuteRTC == Minute && SecondRTC == Second) {
    myservo.write(90);
    delay(3000);
    myservo.write(0);
  }
}
