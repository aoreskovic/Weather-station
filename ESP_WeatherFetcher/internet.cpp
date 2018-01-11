
#include "internet.h"

const char url[] = API_KEY;
const char host[] = "samples.openweathermap.org";

WiFiClient client;

int initWifi()
{
    char ssid[] = AP_NAME; //  your network SSID (name)
    char pwd[] = PASS;     // your network password (use for WPA, or use as key for WEP)[]

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


int getWeatherData(String *returnString)
{
    String resultString;
    // Stop all previous connections
    client.stop();

    // Start new conenction
    Serial.print("connecting to ");
    Serial.println(host);
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
    {
        Serial.println("connection failed");
        return -6;
    }

    // We now create a URI for the request
    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
        if (millis() - timeout > 5000)
        { // If Timeout
            Serial.println(">>> Client Timeout !");
            client.stop();
            return -7;
        }
    }

    int i = 0;
    // Read all the lines of the reply from server and print them to serial. Save them in resultString for parsing
    while (client.available())
    {
        Serial.print(i);
        Serial.println(" Loading api result into resultString...");
        resultString = client.readStringUntil('\r');
        Serial.println(resultString);
    }

    Serial.println();
    Serial.println("closing connection");

    // redundant, should probably remove
    client.stop();
    return 1;
}
