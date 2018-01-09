
#include "keys.h" //Wifi SSID, password and API keys are here
#include <ArduinoJson.h>

StaticJsonBuffer<800> jsonBuffer;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    continue;

  char json[] =
      "{\"coord\":{\"lon\":-0.13,\"lat\":51.51},\"weather\":[{\"id\":300,\"main\":\"Drizzle\",\"description\":\"light intensity drizzle\",\"icon\":\"09d\"}],\"base\":\"stations\",\"main\":{\"temp\":280.32,\"pressure\":1012,\"humidity\":81,\"temp_min\":279.15,\"temp_max\":281.15},\"visibility\":10000,\"wind\":{\"speed\":4.1,\"deg\":80},\"clouds\":{\"all\":90},\"dt\":1485789600,\"sys\":{\"type\":1,\"id\":5091,\"message\":0.0103,\"country\":\"GB\",\"sunrise\":1485762037,\"sunset\":1485794875},\"id\":2643743,\"name\":\"London\",\"cod\":200}";

  JsonObject &root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  double hight = root["main"]["temp_min"];
  double lowt = root["main"]["temp_max"];

  hight -= 273.15;
  lowt -= 273.15;

  // Print values.
  Serial.println("");
  Serial.print("Temp high: ");
  Serial.println(hight);
  Serial.print("Temp low: ");
  Serial.println(lowt);
}


void loop()
{
}
