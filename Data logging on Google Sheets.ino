#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
int pot = 34;
int ir = 12;
// WiFi credentials
const char* ssid = "PTCL-MM";         // change SSID
const char* password = "1308mimi";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbwC9FY65Pll4ET2xc4pOvO8_ag5gkp9Y0JGVrB5GhrkD55niKMuq4UIKSQHQCE5MwbN";    // change Gscript ID
int count = 0;
void setup() {
  pinMode(pot, INPUT);
  pinMode(ir, INPUT);
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void loop() {
   if (WiFi.status() == WL_CONNECTED) {
      int irreading = analogRead(ir);
  int potreading = analogRead(pot);
  Serial.print("IR READING: ");
  Serial.println(irreading);
  Serial.print("Potentiometer READING: ");
  Serial.println(potreading);
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"irsensor=" + String(irreading) + "&potentiometer=" + String(potreading)+ "&datetime=" + asString;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
  count++;
  delay(1000);
} 
