
#include "internet.h"
#include <ArduinoJson.h>



StaticJsonBuffer<10000> jsonBuffer;
String json;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    continue;

  Serial.println("");
  Serial.print("Spajam se: ");
  Serial.println(initWifi());


}

void loop()
{
  {
    getWeatherData(&json);
  }
  JsonObject &root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
    delay(5000);
  }
  else
  {

    String toSend = "^$";

    String tmp;
    double temp_now = root["current"]["temp_c"];
    double temp_now_feels = root["current"]["feelslike_c"];

    String description = root["current"]["condition"]["text"];
    int clouds = root["current"]["cloud"];
    String date = root["location"]["localtime"];
    description.toLowerCase();

    tmp = String(date);
    tmp.trim();
    toSend += tmp + "$";

    tmp = String(temp_now, 0);
    tmp.trim();
    toSend += tmp + "$";

    tmp = String(temp_now_feels, 0);
    tmp.trim();
    toSend += tmp + "$";

    toSend += description + "$";

    {
      double maxtemp  = root["forecast"]["forecastday"][0]["day"]["maxtemp_c"];
      double mintemp  = root["forecast"]["forecastday"][0]["day"]["mintemp_c"];
      double percip   = root["forecast"]["forecastday"][0]["day"]["totalprecip_mm"];
      String text     = root["forecast"]["forecastday"][0]["day"]["condition"]["text"];
      text.toLowerCase();

      tmp = String(maxtemp, 0);
      tmp.trim();
      toSend += tmp + "$";

      tmp = String(mintemp, 0);
      tmp.trim();
      toSend += tmp + "$";

      tmp = String(percip, 0);
      tmp.trim();
      toSend += tmp + "$";

      toSend += text + "$";
    }
    {
      double maxtemp  = root["forecast"]["forecastday"][0]["day"]["maxtemp_c"];
      double mintemp  = root["forecast"]["forecastday"][0]["day"]["mintemp_c"];
      double percip   = root["forecast"]["forecastday"][0]["day"]["totalprecip_mm"];
      String text     = root["forecast"]["forecastday"][0]["day"]["condition"]["text"];
      text.toLowerCase();


      tmp = String(maxtemp, 0);
      tmp.trim();
      toSend += tmp + "$";

      tmp = String(mintemp, 0);
      tmp.trim();
      toSend += tmp + "$";

      tmp = String(percip, 0);
      tmp.trim();
      toSend += tmp + "$";

      toSend += text + "$";
    }

    Serial.println(toSend);
  }
  json = "";
  jsonBuffer.clear();
  delay(20000);
}


