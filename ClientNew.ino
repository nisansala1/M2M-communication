// Setup the client to send sensor data to the server
#include <ESP8266WiFi.h>
#include "DHT.h"

 

// Initialize sensor parameters
#define DHTPIN D2 
//float volts = 0.0, temperatureC = 0.0;
// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

 

DHT dht(DHTPIN, DHTTYPE);

 

// current temperature , updated in loop()
float t = 0.0;


 

 

// Initialize network parameters
const char* ssid = "ESP8266_SSID";
const char* password = "123456789";
const char* host = "192.168.11.4"; // as specified in server.ino

 

// Set up the client objet
WiFiClient client;


 // The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

 // Updates DHT readings every 10 seconds
const long interval = 10;  

 
const int sleepTimeSeconds =2;

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  
  return String();
}

 

void setup() {
  // open serial for monitoring
  Serial.begin(115200);
  dht.begin();
    

 

  // Connect to the server
  WiFi.begin(ssid,password);
  //Serial.begin(115200);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address (AP): "); Serial.println(WiFi.localIP());

}

 

void loop(  ) {

 

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
  
    float newT = dht.readTemperature();
    
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.print("Temperature - ");
      Serial.println(t);
    }
    
    
    
  }  
   // Connect to the server and send the data as a URL parameter
  if(client.connect(host,80)) {
    String url = "/update?value=";
    url += String(t);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" + 
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    delay(10);
    // Read all the lines of the response and print them to Serial
    Serial.println("Response: ");
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
  

  delay(1000);
  
  Serial.print("RSSI value - ");
  Serial.println( WiFi.RSSI());

 // Deep sleep mode
Serial.println("ESP8266 in sleep mode");
ESP.deepSleep(sleepTimeSeconds * 1e6);
 

}
