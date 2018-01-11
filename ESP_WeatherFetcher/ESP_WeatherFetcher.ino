
#include "internet.h"
#include <ArduinoJson.h>



StaticJsonBuffer<10000> jsonBuffer;


void setup()
{
  Serial.begin(115200);
  while (!Serial)
    continue;

  Serial.println("");
  Serial.print("Spajam se: ");
  Serial.println(initWifi());

  String json;

  {
    getWeatherData(&json);
  } 
  Serial.println("---------u mainu sam");
  Serial.println(json); /**/


  JsonObject &root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  double hight = root["current"]["temp_c"];
  String description = root["current"]["condition"]["text"];
  int clouds = root["current"]["cloud"];
  String date = root["location"]["localtime"];


  // Print values.
  Serial.println("");
  Serial.print("Temp: ");
  Serial.println(hight);
  Serial.print("Description: ");
  Serial.println(description);
  Serial.print("Clouds: ");
  Serial.print(clouds);
  Serial.println(" %");


}

void loop()
{
}




///////////////////////////////////////////

