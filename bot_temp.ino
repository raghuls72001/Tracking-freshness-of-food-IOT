#include "CTBot.h"
#include<ESP8266WiFi.h>
CTBot mybot;

String ssid="AndroidAPB72D";
String pass="cvpz7792";
String token="5294780206:AAEcH2FGyg2mN8fpvY-ZTjWrR1zw_8w17rA";
int led=D0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting TelegramBot..");
  mybot.wifiConnect(ssid,pass);
  mybot.setTelegramToken(token);
  if (mybot.testConnection())
      Serial.println("\ntestConnection OK");
  else
      Serial.println("\nerror");
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
TBMessage msg;
int sensorValue = analogRead(A0);
float millivolts = (sensorValue*(3300/1024));
float celsius=millivolts;
if(CTBotMessageText==mybot.getNewMessage(msg))
      {
          if(msg.text.equalsIgnoreCase("temp")){
            digitalWrite(led,LOW);
            mybot.sendMessage(msg.sender.id,String(celsius)+"°C");
            if(celsius > 40)
              mybot.sendMessage(msg.sender.id,"Your are in a hot region");
            
          }
          else
          {
            String reply;
            reply=(String)"Welcome " +"Raghul "+ (String)"Want to know the temperature ?\n just type temp";
            mybot.sendMessage(msg.sender.id,reply);
          }
          
       }
       delay(500);
}
