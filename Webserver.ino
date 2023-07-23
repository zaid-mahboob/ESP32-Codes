#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "PTCL-MM";
const char *password = "1308mimi";

WebServer server(80);  // here 80 is the port for https request

uint8_t  LEDPin1 = 15;
bool LED1status = LOW;
uint8_t LEDPin2 = 5;
bool LED2status = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);

  delay(1000);
    
    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);  // Here all they mean is server.on means that if this website is reached it will excecute these functions such as
  server.on("/led2on", handle_led2on);    //    192.168.10.25/led2on
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server has started");

  

}

void loop() {
  // to handle http request we want to call the actual handle client function
  server.handleClient();
  if (LED1status){digitalWrite(LEDPin1, HIGH);}else {digitalWrite(LEDPin1, LOW);}
  if (LED2status){digitalWrite(LEDPin2, HIGH);}else{digitalWrite(LEDPin2, LOW);}
 
}

void handle_OnConnect(){
  LED1status = LOW;
  LED2status = LOW;     //Visit HTTP Status codes for more knowledge 
  Serial.println("GPIO 1: OFF and GPIO 5: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status));    // here 200 is okay as it is a HTTP code, then we specify contetn type that it is text or html, then we call sendhtml function
}

void handle_led1off(){
  LED1status = LOW;     //Visit HTTP Status codes for more knowledge 
  Serial.println("GPIO 1: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status));    // here 200 is okay as it is a HTTP code, then we specify contetn type that it is text or html, then we call sendhtml function
}

void handle_led1on(){
  LED1status = HIGH;
  Serial.println("GPIO 1: ON");
  server.send(200, "text/html", SendHTML(true, LED2status));    // here 200 is okay as it is a HTTP code, then we specify contetn type that it is text or html, then we call sendhtml function
}

void handle_led2on(){
  LED2status = HIGH;     //Visit HTTP Status codes for more knowledge 
  Serial.println("GPIO 5: ON");
  server.send(200, "text/html", SendHTML(LED1status, true));    // here 200 is okay as it is a HTTP code, then we specify contetn type that it is text or html, then we call sendhtml function
}

void handle_led2off(){
  LED2status = LOW;     //Visit HTTP Status codes for more knowledge 
  Serial.println("GPIO 5: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false));    // here 200 is okay as it is a HTTP code, then we specify contetn type that it is text or html, then we call sendhtml function
}

void handle_NotFound(){
  Serial.println("NOT FOUND");
  server.send(404, "text/html", "NOT FOUND");    // here 200 is okay as it is a HTTP code, then we specify contetn type that it is text or html, then we call sendhtml function
}

String SendHTML(uint8_t led1stat, uint8_t led2stat){
  String ptr = R"rawliteral(
  <!DOCTYPE html>
  <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
      <title>LED Control</title>
      <link href="https://fonts.googleapis.com/css?family=Montserrat:300,400,700|Roboto+Mono&display=swap" rel="stylesheet">
      <style>
        html {
          font-family: 'Roboto Mono', monospace;
          display: inline-block;
          margin: 0px auto;
          text-align: center;
          background-color: #f4f4f4;
        }
        body {
          margin-top: 50px;
        }
        h1 {
          color: #3498db;
          margin: 50px auto 30px;
          font-family: 'Montserrat', sans-serif;
        }
        h3 {
          color: #2c3e50;
          margin-bottom: 30px;
          font-family: 'Montserrat', sans-serif;
        }
        p {
          font-size: 16px;
          color: #888;
          margin-bottom: 10px;
        }
        .button {
          display: inline-block;
          width: 120px;
          height: 45px;
          background-color: #3498db;
          color: #fff;
          border: none;
          box-shadow: 0px 5px #2980b9;
          text-align: center;
          line-height: 45px;
          transition: all 0.3s ease;
          cursor: pointer;
          font-size: 18px;
          margin: 10px;
          border-radius: 5px;
        }
        .button:hover {
          background-color: #2980b9;
          box-shadow: 0px 2px #2980b9;
        }
        .button:active {
          background-color: #3498db;
          box-shadow: 0 0 #2980b9;
          transform: translateY(4px);
        }
        .button-on {
          background-color: #2ecc71;
          box-shadow: 0px 5px #27ae60;
        }
        .button-on:hover {
          background-color: #27ae60;
          box-shadow: 0px 2px #27ae60;
        }
        .button-on:active {
          background-color: #2ecc71;
          box-shadow: 0 0 #27ae60;
          transform: translateY(4px);
        }
        .button-off {
          background-color: #e74c3c;
          box-shadow: 0px 5px #c0392b;
        }
        .button-off:hover {
          background-color: #c0392b;
          box-shadow: 0px 2px #c0392b;
        }
        .button-off:active {
          background-color: #e74c3c;
          box-shadow: 0 0 #c0392b;
          transform: translateY(4px);
        }
      </style>
    </head>
    <body>
      <h1>ESP32 Web Server</h1>
      <h3>Using AP Mode</h3>
  )rawliteral";

  if(led1stat)
  {
    ptr += R"rawliteral(
    <p>LED1 Status: ON</p>
    <a class="button button-off" href="/led1off">OFF</a>
    )rawliteral";
  }
  else
  {
    ptr += R"rawliteral(
    <p>LED1 Status: OFF</p>
    <a class="button button-on" href="/led1on">ON</a>
    )rawliteral";
  }

  if(led2stat)
  {
    ptr += R"rawliteral(
    <p>LED2 Status: ON</p>
    <a class="button button-off" href="/led2off">OFF</a>
    )rawliteral";
  }
  else
  {
    ptr += R"rawliteral(
    <p>LED2 Status: OFF</p>
    <a class="button button-on" href="/led2on">ON</a>
    )rawliteral";
  }

  ptr += R"rawliteral(
    </body>
  </html>
  )rawliteral";

  return ptr;
}
