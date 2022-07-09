#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WIFI_SSID "AndroidAPB72D"
#define WIFI_PASS "cvpz7792"

/************************* Adafruit.io Setup (entering the server URL, port, username, and key) ****************************/

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "raghuls72001"
#define AIO_KEY "aio_UBOh24C9YrHtUp1Xte2svAgTahng"

int led = D0;
int THRESHOLD=250;
int analogValue ; 
/************ Setup the MQTT client ******************/

// Setup the MQTT server and port
WiFiServer server(80);
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Subscribe Methane = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Methane");
Adafruit_MQTT_Publish GasStatus = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/GasStatus");


void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);


  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi.");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(50);
  }

  Serial.println("OK!");
  server.begin();
  //Subscribe to the onoff topic
  mqtt.subscribe(&Methane);


}

void loop()
{
  //Connect/Reconnect to MQTT
  analogValue = analogRead(A0);
  Serial.println(analogValue);
  

  MQTT_connect();
  

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    
    //If we're in here, a subscription updated...
    if (subscription == &Methane)
    {
      //Print the new value to the serial monitor
      Serial.print("Gas: ");
      Serial.println((char*) Methane.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) Methane.lastread, "gas detected"))
      {
        //active low logic
        digitalWrite(led,HIGH);
        GasStatus.publish(analogValue);
        if(analogValue>=THRESHOLD)
        {
          digitalWrite(led,LOW);
        }
      }
      else GasStatus.publish("Error");
      
    }
    else Serial.println("error");
  }
WiFiClient client = server.available();
if (client) 
{ 

Serial.println("Web Client connected ");
String request = client.readStringUntil('\r'); 
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 10");  // update the page after 10 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<style>html { font-family: Cairo; display: block; margin: 0px auto; text-align: center;color: #333333; background-color: ##f3ffee;}");
client.println("body{margin-top: 50px;}");
client.println("h1 {margin: 50px auto 30px; font-size: 50px;  text-align: center;}");
client.println(".side_adjust{display: inline-block;vertical-align: middle;position: relative;}");
client.println(".text1{font-weight: 180; padding-left: 5px; font-size: 50px; width: 170px; text-align: left; color: #3498db;}");
client.println(".data1{font-weight: 180; padding-left: 1px; font-size: 50px;color: #3498db;}");
client.println(".data{padding: 1px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div id=\"webpage\">");   
client.println("<h1>Food Monitoring</h1>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Status:</div>");
client.println("<div class=\"side_adjust data1\">");
client.print(analogValue);
//client.print("<script>if analogValue >= 250 document.write(\"Spoiled\")</script>");
client.println("<div class=\"side_adjust text1\">PPM</div>");
client.println("</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
}

}

void MQTT_connect()
{

  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}
