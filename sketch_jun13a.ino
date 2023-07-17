/*code for NodeMCU ESP-12E*/
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

//rxPin = pin 3, txPin = pin 1; this line has been compress from the "SoftwareSerial s =  SoftwareSerial(rxPin, txPin);"
SoftwareSerial s(3,1);

// Create a webserver object named "server" that listens for HTTP request on port 80
ESP8266WebServer server(80); 

//create an array and some const to work with
char htmlResponse[10000];
const char* ssid     = "Huy's Galaxy A32";
const char* password = "tmlw1997";

/*The PROGMEM keyword is a variable modifier, it should be used only with the datatypes defined in pgmspace.h.
It tells the compiler "put this information into flash memory", instead of into SRAM, where it would normally go*/
const char openDoor_status[] PROGMEM = R"=====(<h3 style="text-align:center">Cửa đang mở</h3>)=====";
const char closeDoor_status[] PROGMEM = R"=====(<h3 style="text-align:center">Cửa đang đóng</h3>)=====";
const char webPage_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
  <style>
    .b{width: 120px;height: 40px;font-size: 21px;color: #FFF;background-color:#4caf50;border-radius: 10px;}
    .t{width: 120px;height: 40px;font-size: 21px;color: #FFF;background-color:#f44336;border-radius: 10px;}
        table.center {
            margin-left: "auto";
            margin-right: "auto";
          }
#login{ line-height: 1.5; margin: 0 auto; margin-top:150px; width:50%; font-size: 50px;background-color: white; border:2px solid black; padding:10px 10px 10px 10px; border-radius: 10px; z-index: 1000;}
  </style>
    <head>
        <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
        <title> Cửa tự động RFID </title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.0/jquery.min.js"></script>
    </head>
    <body>
      <div id = "login" >
        <form id = "login-page">
          <label for="username">ID người dùng</label><br>
          <input style="width: 75%;height:50px;font-size: 40px" type="text" id="username" placeholder="ID người dùng"><br>
          <label for = "inputPassword">Mật khẩu</label><br>
          <input style="width: 75%;height:50px;font-size: 40px" type ="password" id="inputPassword" placeholder="Mật Khẩu"><br>
          <button style="font-size: 40px" class="btn btn-lg btn-primary btn-block btn-signin" id="btnlogin" type ="submit" >Đăng nhập</button>
        </form>    
      </div>
      <div id = "main" style="width: 330px;height: auto;margin: 0 auto;margin-top:150px;display: none">
            <h1 style="text-align:center">Điều khiển cửa qua WIFI</h1>
            <table class ="center">
                <tr>
                    <td><a href = '/bat1'><button id = "bat1" class='b' type="button" >Mở cửa </button></a><td>
                    <td><a href = '/tat1'><button id = "tat1" class='t' type="button" >Đóng cửa </button></a><td>
                    </tr>
                <tr>
                    <td><a href = '/bat2'><button id = "bat2" class='b' type="button" >Xóa thẻ</button></a><td>
                    <td><a href = '/tat2'><button id = "tat2" class='t' type="button" >Đổi thẻ</button></a><td>
                    </tr>
            </table>
            <h2 style="text-align:center" id = "demo">
            </h2>
        </div>
        <script>
          var myID;
        var myPass;
        var isLoggedIn = false;
        $(document).ready(function() {
          $('#btnlogin').click(function() {
            myID = $('#username').val();
            myPass = $('#inputPassword').val();
            if (myID === "huynoname" && myPass === "15297583") {
              isLoggedIn=true;
              $('#login').remove();
              $('#main').show();
              return false;
            } else {
              alert("Sai thông tin");
            }
          });
        });
            </script>
    </body>
</html>)=====";

const char webPageUpdate_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
  <style>
    .b{width: 120px;height: 40px;font-size: 21px;color: #FFF;background-color:#4caf50;border-radius: 10px;}
    .t{width: 120px;height: 40px;font-size: 21px;color: #FFF;background-color:#f44336;border-radius: 10px;}
        table.center {
            margin-left: "auto";
            margin-right: "auto";
          }
  </style>
    <head>
        <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
        <title> Cửa tự động RFID </title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.0/jquery.min.js"></script>
    </head>
    <body>
      <div style="width: 330px;height: auto;margin: 0 auto;margin-top:150px;">
            <h1 style="text-align:center">Điều khiển cửa qua WIFI</h1>
            <table class ="center">
                <tr>
                    <td><a href = '/bat1'><button id = "bat1" class='b' type="button" >Mở cửa </button></a><td>
                    <td><a href = '/tat1'><button id = "tat1" class='t' type="button" >Đóng cửa </button></a><td>
                    </tr>
                <tr>
                    <td><a href = '/bat2'><button id = "bat2" class='b' type="button" >Xóa thẻ</button></a><td>
                    <td><a href = '/tat2'><button id = "tat2" class='t' type="button" >Đổi thẻ</button></a><td>
                    </tr>
            </table>
            <h2 style="text-align:center" id = "demo">
            </h2>
        </div>
    </body>
</html>)=====";

//
void handleRoot() {
  snprintf(htmlResponse, 10000, webPage_html);
  server.send ( 200, "text/html", htmlResponse );
}

//
void bat1(){
  Serial.println("bat1");
  s.print("bat1\n");
  snprintf(htmlResponse, 10000, webPageUpdate_html);
  server.send ( 200, "text/html", htmlResponse );
}

//
void tat1(){
  s.print("tat1\n");
  Serial.println("tat1");
  snprintf(htmlResponse, 10000, webPageUpdate_html);
  server.send ( 200, "text/html", htmlResponse );
}

//
void bat2(){
  s.print("bat2\n");
  Serial.println("bat2");
  snprintf(htmlResponse, 10000, webPageUpdate_html);
  server.send ( 200, "text/html", htmlResponse );
}

//
void tat2(){
  s.print("tat2\n");
  Serial.println("tat2");
  snprintf(htmlResponse, 10000, webPageUpdate_html);
  server.send ( 200, "text/html", htmlResponse );
}

//Setup the hardware for the ESP broad
void setup() {
  //Set the baud rate for the SoftwareSerial object ("s") and normal serial to 115200 bits per second
  s.begin(115200);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Starting . . .");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //connect to the wifi with the said ssid and password
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
  server.on ("/bat1", bat1);
  server.on ("/tat1", tat1);
  server.on ("/bat2", bat2);
  server.on ("/tat2", tat2);
  server.begin();
  Serial.println ( "HTTP server started" );
  delay(1000);
}

//equivalent of main in .c file. Loop the handleClient function of server.
void loop() {
  server.handleClient();
  }

