#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define LED 0

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://your_adress:3000";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;


void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println(WiFi.localIP());
  //The ESP8266 tries to reconnect automatically when the connection is lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}


/* ERROR CODE :
 *  1 = post sended
 *  2 = good http response 
 *  3 = bad http response
 *  4 = wifi disconnected
 */
void errorBlink(int nb){
  for(int i = 0; i<nb ; i++){
    digitalWrite(LED,HIGH);  
    delay (250);
    digitalWrite(LED,LOW);
    delay (250);
  }
}

void setup() {
  //GPIO0 as an output for the statut led
  pinMode(LED,OUTPUT);
  //While setup the led is always ON
  digitalWrite(LED,HIGH);

  //DBUG
  Serial.begin(115200);
  
  //Init wifi with auto reconnect
  initWiFi();

  //DEBUG
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  digitalWrite(LED,LOW);
}

void loop() {
  //Send an HTTP POST request every SETUP minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      
      //Tel where sending
      errorBlink(1);
      delay(500);
      
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"api_key\":\"WHATEVER YOU WANT\",\"Sensor\":\"WAN\"}");

      //DEBUG
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      //handle http response
      if (httpResponseCode == 200){
        errorBlink(2);
      }else{
        errorBlink(3);
      }
       
      // Free resources
      http.end();
    }
    //If wifi lost blink four time
    else {
      //DEBUG
      Serial.println("WiFi Disconnected");
      errorBlink(4);
    }
    lastTime = millis();
  }
}
