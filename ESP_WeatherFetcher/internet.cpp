
#include "internet.h"


const char host[] = "http://samples.openweathermap.org/data/2.5/weather?q=London,uk";
const char key[] = API_KEY;

WiFiClient client;

int initWifi()
{
    char ssid[] = AP_NAME;  //  your network SSID (name)
    char pwd[] = PASS; // your network password (use for WPA, or use as key for WEP)[]

    int connAttempts = 0;
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (connAttempts > 20)
            return -5;
        connAttempts++;
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    return 1;
}

