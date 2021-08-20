// AT TOP

#include <WiFiNINA.h>
#include "arduino_secrets.h"

// Get WIFI Secrets
char ssid[] = SECRET_SSID
char pass[] = SECRET_PASS
int status = WL_IDLE_STATUS;

char server[] = "https://use1-wap.tplinkcloud.com?token=02333182-ATymet4gnC5PxYrSrsSu6j2";    // name address for Google (using DNS)

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, 80);

// IN SETUP

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

// IN LOOP



  // Temp Start Program
  if (light > 80) {

    // Connect to KASA Light Switch and Turn on
    kasaSwitch(int 1)
    
    // Temp Do While
    do {
      //goDisplay = DisplayValues(Environment.readTemperature(), Environment.readHumidity());'

    } while (Environment.readHumidity() > 50);

  }
  else if ( light < 50 && Environment.readHumidity() < 50) {
    // Connect to KASA Light Switch and Turn OFF
    kasaSwitch(int 0)
  }


//
  Print IP Info
  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  Serial.println(ip);

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  

// We will want a HTTP Function here
void kasaSwitch(int status) {
  Serial.println("Connecting to TP-Link KASA Endpoint");
  String contentType = "application/json";
  String postDataOff = "{\"method\":\"passthrough\", \"params\": {\"deviceId\": \"8006C51E784EC3B488F073D5BB0641E41D88CD71\", \"requestData\": \"{\\\"system\\\":{\\\"set_relay_state\\\":{\\\"state\\\":0}}}\" }}"
  String postDataOn = "{\"method\":\"passthrough\", \"params\": {\"deviceId\": \"8006C51E784EC3B488F073D5BB0641E41D88CD71\", \"requestData\": \"{\\\"system\\\":{\\\"set_relay_state\\\":{\\\"state\\\":1}}}\" }}"

  if (status == 1) {
    client.post("/posts", contentType, postDataOn)
  }
  else {
    client.post("/posts", contentType, postDataOff)
  }

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  String response = client.responseBody();
  Serial.print("Response: ");
  Serial.println(response);

}


// Test Functions

int DisplayValues(int temp, int humidity) {

  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0, 33);
  Oled.print("Temp: ");
  Oled.print(temp);
  Oled.refreshDisplay();
  delay(5000);
  Oled.print("Humidity: ");
  Oled.print(humidity);
  Oled.print("%");
}
