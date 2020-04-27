/*
 * Auteur : G.Menez
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> 
#include <Wire.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include "net_misc.h"

/*============= GPIO ======================*/
const int ledPin = 22; // LED Pin
const int photo_resistor_pin =A5; //Photoresistor
OneWire oneWire(23);
DallasTemperature tempSensor(&oneWire);

WiFiClient espClient; // Wifi
PubSubClient client(espClient) ; // MQTT client

String whoami; // Identification de CET ESP au sein de la flotte

//StaticJsonBuffer<200> jsonBuffer;

/*===== MQTT broker/server and TOPICS ========*/
//const char* mqtt_server = "192.168.1.100";
const char* mqtt_server = "212.115.110.52";

#define TOPIC_TEMP "sensors/temp/zakaria"
#define TOPIC_LED "sensors/led/zakaria"
#define TOPIC_LIGHT "sensors/photoresistor/zakaria"
#define TOPIC_CALLBACK "callback/zakaria"

/*=============== SETUP =====================*/

void setup () {
  // Gpio
  pinMode (ledPin , OUTPUT);
  // Serial
  Serial.begin (9600);
  
  /* Wifi */
  connect_wifi();
  
  /*  L'ESP est un client du mqtt_server */
  client.setServer(mqtt_server, 1818);
  // set callback when publishes arrive for the subscribed topic
  // methode a effet local => on n'a pas a initier/verifier la connection.
  client.setCallback(mqtt_pubcallback);


  /* Choix d'une identification pour cet ESP ---*/
   whoami = "Zakaria"; 
  //whoami =  String(WiFi.macAddress());
}

/*============== MQTT CALLBACK ===================*/

void mqtt_pubcallback(char* topic, byte* message, unsigned int length) {
  /* 
   *  Callback if a message is published on this topic.
   */
   char data[80];
  String payload;
  
  // Byte list to String ... plus facile a traiter ensuite !
  // Mais sans doute pas optimal en performance => heap ?
  String messageTemp ;
  for(int i = 0 ; i < length ; i++) {
    messageTemp += (char) message[i];
  }
  
  Serial.print("Message : ");
  Serial.println(messageTemp);
  Serial.print("arrived on topic : ");
  Serial.println(topic) ;
 
  // Analyse du message et Action 
  if(String (topic) == TOPIC_CALLBACK) {
     // Par exemple : Changes the LED output state according to the message   
    Serial.print("Action : Changing output to ");
    if(messageTemp == "onlamp") {
      Serial.println("on");
      set_pin(ledPin,HIGH);
     
    } else if (messageTemp == "offlamp") {
      Serial.println("offlamp");
      set_pin(ledPin,LOW);
    }

    else if (messageTemp == "gettemp") {
      Serial.println("---get temp---");

       payload = "{\"field\": \"";
  payload += "zakaria";   
  payload += "\", \"value\": " ;
  payload += get_temperature(); 
  payload += "}";
  
  payload.toCharArray(data, (payload.length() + 1)); // Convert String payload to a char array
  Serial.println(data);
  client.publish(TOPIC_TEMP, data);  // publish it 
      
    }
    else if (messageTemp == "getphoto") {
      Serial.println("--get photoresistor----");

       payload = "{\"field\": \"" + whoami + "\", \"value\": " + get_light() + "}";
  payload.toCharArray(data, (payload.length() + 1));
  Serial.println(data);
  client.publish(TOPIC_LIGHT, data);
      
    }
  }
  
}





/*============= MQTT SUBSCRIBE =====================*/

void mqtt_mysubscribe(char* topic) {
  /*
   * ESP souscrit a ce topic. Il faut qu'il soit connecte.
   */
  while(!client.connected()) { // Loop until we are reconnected
    Serial.print("Attempting MQTT connection...");
    if(client.connect("esp32", "try", "try")) { // Attempt to connect 
      Serial.println("connected");
      client.subscribe(topic); // and then Subscribe
    } else { // Connection failed
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5*1000);
    }
  }
}



/*============= ACCESSEURS ====================*/

float get_temperature() {
  float temperature;
  tempSensor.requestTemperaturesByIndex(0);
  delay (750);
  temperature = tempSensor.getTempCByIndex(0);
  return temperature;
}

float get_light(){
  return analogRead(photo_resistor_pin);
}

void set_pin(int pin, int val){
 digitalWrite(pin, val) ;
}

int get_pin(int pin){
  return digitalRead(pin);
}

/*================= LOOP ======================*/
void loop () {
  char data[80];
  String payload; // Payload : "JSON ready" 
  int32_t period = 30 * 100; // Publication period
  
  /* Subscribe to TOPIC_LED if not yet ! */
  if (!client.connected()) {
    mqtt_mysubscribe((char*) (TOPIC_CALLBACK));
  }
  
  /* Publish Temperature & Light periodically */
 

  /* char tempString[8];
     dtostrf(temperature, 1, 2, tempString);
     client.publish(TOPIC_TEMP, tempString); */



  Serial.println("waiting mqtt");
  
  client.loop();


  

  delay(period);
  client.loop(); // Process MQTT ... obligatoire une fois par loop()
}
