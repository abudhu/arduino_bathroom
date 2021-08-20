// Includes

#include "Arduino_SensorKit.h"
#include <SPI.h>

// Init Plugs
int light_sensor = A3;

void setup() {

  Serial.begin(9600)

  // Setup Temperature and Humidity Sensor
  Environment.begin();

  // Setup LED Display
  Oled.begin();
  Oled.setFlipMode(true);

}

void loop() {

  // Setup light reads
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100); // maps values of 0, 1023 from sensor to 0 100 output for humans

  // Output Info //

  // Output Light
  Serial.print("Light Level: ");
  Serial.println(light);

  // Output Temperature
  Serial.print("Temperature: ");
  Serial.println(Environment.readTemperature());

  Serial.print("Humidity: ");
  Serial.print(Environment.readHumidity());
  Serial.println("%");

  // Output Temp to LED

  // goDisplay = DisplayValues(Environment.readTemperature(), Environment.readHumidity());

  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0, 33);
  Oled.print("Temp: ");
  Oled.print(Environment.readTemperature());
  Oled.refreshDisplay();
  delay(5000); // Sleep 5 second
}
