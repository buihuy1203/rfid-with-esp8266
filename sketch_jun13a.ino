#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
ESP8266WebServer server(80);
char htmlResponse[10000];
int i = 0;
const char* ssid     = "Huy's Galaxy A32";
const char* password = "tmlw1997";
const char openDoor_status[] PROGMEM = R"=====(<h3 style="text-align:center">Cửa đang mở</h3>)=====";
const char closeDoor_status[] PROGMEM = R"=====(<h3 style="text-align:center">Cửa đang đóng</h3>)=====";
const char webPage_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
        <title> Cửa tự động RFID </title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
 <style>
    .b{width: 100px;height: 40px;font-size: 21px;color: #FFF;background-color:#4caf50;border-radius: 10px;}
    .t{width: 100px;height: 40px;font-size: 21px;color: #FFF;background-color:#f44336;border-radius: 10px;}
        table.center {
            margin-left: "auto";
            margin-right: "auto";
          }
  </style>
    </head>
    <body>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
        <script type='text/javascript'>
            function openDoor() { 
              var open;
              $('#bat1').click(function(e){
                e.preventDefault();
                open = "bat1";
            $.get('/save?open=' + open, function(data){
             console.log(data);
            });
            });
        }
            function closeDoor() { 
                var close;
              $('#tat1').click(function(e){
                e.preventDefault();
                close = "tat1";
            $.get('/save?close=' + close, function(data){
             console.log(data);
            });
            });
              }
              function deleteCard() { 
                document.getElementById("demo").innerHTML = "Xóa thẻ thành công";
                var del;
              $('#bat2').click(function(e){
                e.preventDefault();
                del = "bat2";
            $.get('/save?del=' + del, function(data){
             console.log(data);
            });
            });
              }
              function swapCard(){
                var swap;
              $('#tat2').click(function(e){
                e.preventDefault();
                swap = "tat2";
            $.get('/save?swap=' + swap, function(data){
             console.log(data);
            });
            });
              }
            </script>
        <div style="width: 330px;height: auto;margin: 0 auto;margin-top: 70px">
            <h1 style="text-align:center">Điều khiển cửa qua WIFI</h1>
            <table class ="center">
                <tr>
                    <td><button id = "bat1" class='b' type="button" onclick="openDoor()">Mở cửa </button><td>
                    <td><button id = "tat1" class='t' type="button" onclick="closeDoor()">Đóng cửa </button><td>
                    </tr>
                <tr>
                    <td><button id = "bat2" class='b' type="button" onclick="deleteCard()">Xóa thẻ</button><td>
                    <td><button id = "tat2" class='t' type="button" onclick="swapCard()">Đổi thẻ</button><td>
                    </tr>
            </table>
            <h2 style="text-align:center" id = "demo">
            </h2>
        </div>
    </body>
</html>)=====";
void handleRoot() {
  snprintf(htmlResponse, 10000, webPage_html);
  server.send ( 200, "text/html", htmlResponse );
}
void handleSave() {
  if (server.arg("open") != "") {
    Serial.println(server.arg("open"));
  } else if (server.arg("close") != "") {
    Serial.println(server.arg("close"));
  } else if (server.arg("del") != "") {
    Serial.println(server.arg("del"));
  } else if (server.arg("swap") != "") {
    Serial.println(server.arg("swap"));
  }
}
void setup(void) {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Starting . . .");
  while(i < 20){
    Serial.println(i);
    i=i+1;
  }
  i = 0;
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on ( "/", handleRoot );
  server.on ("/save", handleSave);
  server.begin();
  Serial.println ( "HTTP server started" );
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  while (Serial.available()) {
    String income = "";
    income = Serial.readString();
    if (income == "openDoor") {
      snprintf(htmlResponse, 10000, openDoor_status);
    }
    else if (income == "closeDoor") {
      snprintf(htmlResponse, 10000, closeDoor_status);
    }
  }
}
