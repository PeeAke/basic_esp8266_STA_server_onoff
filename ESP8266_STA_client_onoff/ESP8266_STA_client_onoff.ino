// https://github.com/boblemaire/asyncHTTPrequest

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "asyncHTTPrequest.h"

int break_state = 0;
asyncHTTPrequest request[85];
String host_ip;
int LED_state = 0;

void requestCB(void* optParm, asyncHTTPrequest* request, int readyState){
    host_ip = request->responseText();
    Serial.println(host_ip);
    Serial.println("out");
    request->setDebug(false);
		break_state = 1;
}

void scan(int start, int quan) {
  int time;
  int j=0;
  for (int i=start; i<start+quan; i++) {
    request[j].onData(requestCB);
    request[j].setTimeout(2);
    time = millis();
    String url = "http://192.168.1."+String(i)+"/ip";
    request[j].open("GET", url.c_str());
    request[j].send();
    Serial.print(url);
    while (time+500>millis()) {
      Serial.print(".");
      delay(100);
    }
    if (break_state == 1) {
      break;
    }
    Serial.println("/");
    j+=1;
  }
  if (break_state != 1) {
    delay(1000);
  }  
}

void WiFi_connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("YourSSID", "YourPassword");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println(" ");
}


void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  WiFi_connect();
  scan(2,81);
  if (break_state!=1) {
    scan(83,81);
  }
  if (break_state!=1) {
    scan(164,81);
  }
  Serial.println(" ");
  Serial.println("IP_SCAN");
  Serial.println("==============");
  if (break_state==1) {
    Serial.println(host_ip);
  } else {
    Serial.println("IP_SCAN Not found");
    while (true) {
      delay(100);
    }
  }
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.mode(WIFI_OFF);
      delay(100);  
      WiFi_connect();
    }
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    http.begin("http://"+host_ip+"/get");
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
        LED_state=payload.toInt();
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    digitalWrite(2,LED_state);
    delay(1000);
}
