// Includes

#include "Arduino_SensorKit.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

// Get WIFI Secrets
char ssid[] = SECRET_SSID
char pass[] = SECRET_PASS
int status = WL_IDLE_STATUS;

IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

WiFiClient client;

// Init Plugs
int light_sensor = A3; 

void setup() {
  
  // Setup Light Sensor
  Serial.begin(9600)
  Environment.begin();

  // Setup LED
  
  Oled.begin();
  Oled.setFlipMode(true); 

  //Connect to Wifi
  
  while (status != WL_CONNECTED) {
    
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
     delay(10000);
  }

  Serial.print("You're connected to the network");

  printCurrentNet();

  printWifiData();

}

void loop() {

  // 
  Print IP Info
  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  Serial.println(ip);

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // Setup light reads
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100); // maps values of 0, 1023 from sensor to 0 100 output for humans


  // Temp Start Program

  if(light > 80) {

    // Connect to KASA Light Switch and Turn on

    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.println("GET /search?q=arduino HTTP/1.1");
      client.println("Host: www.google.com");
      client.println("Connection: close");
      client.println();
    }
    
    // Temp Do While
    do {
      //goDisplay = DisplayValues(Environment.readTemperature(), Environment.readHumidity());'
      
    } while (Environment.readHumidity() > 50);
    
  }
  else if ( light < 50 && Environment.readHumidity() < 50) {
    // Connect to KASA Light Switch and Turn OFF

    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.println("GET /search?q=arduino HTTP/1.1");
      client.println("Host: www.google.com");
      client.println("Connection: close");
      client.println();
    }  
  }

  // Output Info

  // Output Light
  Serial.print("Light Level: ");
  Serial.println(light);

  // Output Temp
  Serial.print("Temp: ");
  Serial.println(Environment.readTemperature());

  Serial.print("Humidity: ");
  Serial.print(Environment.readHumidity());
  Serial.println("%");
  
  //Output Temp to LED
  
  //goDisplay = DisplayValues(Environment.readTemperature(), Environment.readHumidity());
  
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0,33);
  Oled.print("Temp: ");
  Oled.print(Environment.readTemperature());
  Oled.refreshDisplay();
  //We can sleep this and have the display switch text, to the humidity after and add this to an internal loop.

  delay(5000); // Sleep 5 second
}

// Test Functions

int DisplayValues(int temp,int humidity){
  
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0,33);
  Oled.print("Temp: ");
  Oled.print(temp);
  Oled.refreshDisplay();
  delay(5000);
  Oled.print("Humidity: ");
  Oled.print(humidity);
  Oled.print("%");
}

// We will want a HTTP Function here
