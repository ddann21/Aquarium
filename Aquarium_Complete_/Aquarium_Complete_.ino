#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Servo.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define FIREBASE_HOST "feesh-93f92-default-rtdb.asia-southeast1.firebasedatabase.app"  
#define FIREBASE_AUTH "3DY7nzymcfUYGLKqO6tvcyXLs98NizHYBusUmRSz"  
#define WIFI_SSID "ddann~"
#define WIFI_PASSWORD "MineIsMine34"

#define ONE_WIRE_BUS 4  

Servo servo;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800);

void setup()
{
  Serial.begin(9600);
  sensors.begin(); 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  timeClient.begin();
}

void loop()
{
  timeClient.update();
  sensors.requestTemperatures();   

  String timer = Firebase.getString("times/time");
  int fireStatus = Firebase.getInt("feed");
  
  String schedule = timer.substring(0, 5);
  
  String currentTime = String(timeClient.getHours(), DEC);
  currentTime += ':';
  if (timeClient.getMinutes() < 10) {
    currentTime += '0';
  }
  currentTime += String(timeClient.getMinutes(), DEC);
  
  Serial.println(currentTime + " --> " + schedule);
  Serial.print("Temperature: ");
  Serial.println(sensors.getTempCByIndex(0));
  Firebase.setFloat("Temperature", sensors.getTempCByIndex(0)); 
  delay(1000);

  if (fireStatus == 1){
    servo.attach(D4);
    servo.write(100);
    Serial.println("ON");
    delay(3000);
    servo.detach();
    Firebase.setInt("feed", 0);                                                                                                                                                                     
    delay(5000);
  } else {
    if (currentTime == schedule) {
      servo.attach(D4);
      servo.write(100);
      Serial.println("ON");
      delay(3000);
      servo.detach();                                                                                                                                                                   
      delay(60000);
    }
    delay(1000); 
  }
}
