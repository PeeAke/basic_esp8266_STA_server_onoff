#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer webServer(80);
int LED_state = 0;


void handleRoot() {

  String html = "<html><head>";
  html+="<style>";
  html+=".tab {position:absolute;left:150px; }";
  html+="}";
  html+="</style>";
  html+="<script>";
  html+=" function callback(resp) {\n";
  html+="  console.log(resp);\n";
  html+=" }\n";
  html+=" function httpGetAsync(theUrl, callback)\n";
  html+=" {\n";
  html+="  var xmlHttp = new XMLHttpRequest();\n";
  html+="  xmlHttp.onreadystatechange = function() {\n"; 
  html+="    if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {\n";
  html+="      callback(xmlHttp.responseText);\n";
  html+="    } else {\n";
  html+="      console.log(xmlHttp.responseText);\n";
  html+="    }\n";
  html+="  }\n";
  html+="  xmlHttp.open('GET', theUrl, true);\n";
  html+="  xmlHttp.send(null);\n";
  html+=" }\n";
  html+=" function ledon() {\n";
  html+="  httpGetAsync('http://'+window.location.host+'/post?led=1',callback);\n";
  html+="  document.getElementById('pin').innerHTML = 'LED STATE = 1';\n";    
  html+=" }\n";
  html+=" function ledoff() {\n";
  html+="  httpGetAsync('http://'+window.location.host+'/post?led=0',callback);\n";     
  html+="  document.getElementById('pin').innerHTML = 'LED STATE = 0';\n";    
  html+=" }\n";
  html+="</script></head><body><br><br><br>";
  html+="<span class='tab'><div width='150px' id='pin'>LED STATE = "+String(LED_state)+"</div></span><br><br>";
  html+="<span class='tab'><button onclick='ledon()'>ON</button></span><br><br>";
  html+="<span class='tab'><button onclick='ledoff()'>OFF</button></span><br><br>";  
  html+="</body></html>";
  Serial.println("Send HTML page");
  webServer.send(200, "text/html", html);
}

void handleIP() {
  String message = WiFi.localIP().toString();
  Serial.println("Send "+message);
  webServer.send(200, "text/plain", message);
}

void handlePost() {
  String message = "Number of args received:";
  message += webServer.args();  
  message += "\n";                  
  for (int i = 0; i < webServer.args(); i++) {
    message += "Arg nº" + (String)i + " –> ";
    message += webServer.argName(i) + ": ";
    message += webServer.arg(i) + "\n";
    if(webServer.argName(i)=="number") {
      Serial.println("Received "+String(webServer.arg(i)));
    }
  }
  if (webServer.args()>0) {
    LED_state = webServer.arg(0).toInt();
  }
  webServer.send(200, "text/plain", message);
}

void handleGet() {
  webServer.send(200, "text/plain", String(LED_state));  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("YourSSID","YourPassword");
  while (WiFi.status() != WL_CONNECTED) {
	  Serial.print('.');
    delay(200);
  }
  Serial.println(' ');
  Serial.println(WiFi.localIP());  
  Serial.println("Web Server Start...");
  webServer.on("/", handleRoot);
  webServer.on("/ip", handleIP);
  webServer.on("/post", handlePost); 
  webServer.on("/get", handleGet);
  webServer.onNotFound([]() {
    Serial.println("Hello World!");
    String message = "Hello World!\n\n";
    message += "URI: ";
    message += webServer.uri();

    webServer.send(200, "text/plain", message);
  });
  webServer.begin();
}

void loop() {
  webServer.handleClient();
}
