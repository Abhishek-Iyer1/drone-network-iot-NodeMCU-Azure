#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
int count = 0;  
float dev;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup() {

  Serial.begin(115200);                            //Serial connection
  WiFi.begin("WIFI NAME", "PASSWORD");   //WiFi connection
  randomSeed(analogRead(0));
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 timeClient.begin();
}
 
void loop() {
  timeClient.update();
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 

    count = count+1;
    JSONencoder["sensorType"] = "GPS:[lattitude, longitude, altitude,message number]";
 
    JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
    float latitude = 23.015578; //dummy data to check without GPS sensor
    float longitude = 72.467098; 
    float my_altitude = 20; //in meters
    dev = random(200)/100.0;
    values.add(latitude); //Add value to array
    values.add(longitude); //Add value to array
    values.add(my_altitude+dev); //Add value to array
    values.add(count);
 
    JsonArray& timestamps = JSONencoder.createNestedArray("timestamps"); //JSON array
    timestamps.add(timeClient.getFormattedTime()); //Add value to array
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://USERNAME.pythonanywhere.com/postjson");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(5000);  //Send a request every 30 seconds
 
}
