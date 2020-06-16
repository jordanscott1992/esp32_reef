//esp32_reef

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>
#include "SPIFFS.h"

//DS18B20 libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to digital pin 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

AsyncWebServer server(80);

//Wi-Fi Credentials

const char* ssid = "HUAWEI-B310-56DA";
const char* password = "";

//temp sensors

String tempOne;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    if(!MDNS.begin("reef")) {
      Serial.println("Error mDNS");
      return;
    }

    


    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", tempOne);
    });


    server.onNotFound(notFound);

    server.begin();
}

void loop() {

  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 

  tempOne = String(sensors.getTempCByIndex(0)); 

  delay(1000);
}
