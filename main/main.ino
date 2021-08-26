// Includes

#include "Arduino_SensorKit.h"
#include <SPI.h>
#include <WiFiNINA.h>
//#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

// Get WIFI Secrets
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

int HTTP_PORT = 4567;
String HTTP_METHOD = "GET";
char HOST_NAME[] = "192.168.86.149";    // name address for Google (using DNS)
String PATH_NAME_ON = "/api/v1/on";
String PATH_NAME_OFF = "/api/v1/off";

int fan_running = 0;

WiFiClient wifi;

//HttpClient client = HttpClient(wifi, server, 443);

// Init Plugs
int light_sensor = A3;
int temp_US;

void setup() {

  Serial.begin(9600);

  // Setup Temperature and Humidity Sensor
  Environment.begin();

  // Setup LED Display
  
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

  printWiFiData();

}

void loop() {

  // Setup light reads
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100); // maps values of 0, 1023 from sensor to 0 100 output for humans

  // Output Info //

  // Output Light
  Serial.print("Light Level: ");
  Serial.println(raw_light);

  // Output Temperature
  //Serial.print("Temperature: ");
  temp_US = (Environment.readTemperature() * 1.8000 + 32);
  //Serial.println(temp_US);

  //Serial.print("Humidity: ");
  //Serial.print(Environment.readHumidity());
  //Serial.println("%");

  // Output Temp to LED

  int random_value = analogRead(A0);
  
  Oled.setFont(u8x8_font_chroma48medium8_r); 
  Oled.setCursor(0, 33);    // Set the Coordinates 
  Oled.print("Temp:");   
  Oled.print(temp_US); // Print the Values  
  Oled.setCursor(0, 43);    // Set the Coordinates 
  Oled.print("Humidity:");   
  Oled.print(Environment.readHumidity()); // Print the Values
  Oled.setCursor(0, 53);    // Set the Coordinates 
  Oled.print("Light:");   
  Oled.print(raw_light); // Print the Values
  Oled.refreshDisplay();    // Update the Display 
  delay(5000);

  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  //Serial.print("IP Address: ");

  //Serial.println(ip);

  Serial.println(fan_running);

  if (raw_light > 500) {

    if (fan_running == 0) {
      Serial.println("Fan wasn't running so we are turning it on for the first time");
        // connect to web server on port 80:
        if(wifi.connect(HOST_NAME, HTTP_PORT)) {
          // if connected:
          Serial.println("Connected to server");
          // make a HTTP request:
          // send HTTP header
          wifi.println(HTTP_METHOD + " " + PATH_NAME_ON + " HTTP/1.1");
          wifi.println("Host: " + String(HOST_NAME));
          wifi.println("Connection: close");
          wifi.println(); // end HTTP header
      
          while(wifi.connected()) {
            if(wifi.available()){
              // read an incoming byte from the server and print it to serial monitor:
              char c = wifi.read();
              Serial.print(c);
            }
          }
      
          // the server's disconnected, stop the client:
          wifi.stop();
          Serial.println();
          Serial.println("Setting Fan to Running status");
          fan_running = 1;
          Serial.println("disconnected");
        } else {// if not connected:
          Serial.println("connection failed");
        }
        
      } else {
        Serial.println("Fan Already Running");
      }
    } else if (raw_light < 500 && fan_running == 1 ) {
      Serial.println("Turning Fan Off");
      // connect to web server on port 80:
        if(wifi.connect(HOST_NAME, HTTP_PORT)) {
          // if connected:
          Serial.println("Connected to server");
          // make a HTTP request:
          // send HTTP header
          wifi.println(HTTP_METHOD + " " + PATH_NAME_OFF + " HTTP/1.1");
          wifi.println("Host: " + String(HOST_NAME));
          wifi.println("Connection: close");
          wifi.println(); // end HTTP header
      
          while(wifi.connected()) {
            if(wifi.available()){
              // read an incoming byte from the server and print it to serial monitor:
              char c = wifi.read();
              Serial.print(c);
            }
          }
      
          // the server's disconnected, stop the client:
          wifi.stop();
          Serial.println();
          fan_running = 0;
          Serial.println("disconnected");
        } else {// if not connected:
          Serial.println("connection failed");
        }
      
    } else {
      Serial.println("Not bright enough to turn on Fan");
    }
}



void printWiFiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);

  Serial.print("Subnet mask: ");
  Serial.println((IPAddress)WiFi.subnetMask());

  Serial.print("Gateway IP : ");
  Serial.println((IPAddress)WiFi.gatewayIP());

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
  Serial.println();
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}
