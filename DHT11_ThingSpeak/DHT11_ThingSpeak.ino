#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define DHTPIN 2
#define DHTTYPE DHT11


//uncomment the below and fill it in
//const char* ssid     = "";
//const char* password = "";

unsigned long myChannelNumber = 2461832; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "HZGQ1IDXQK7QXZAP"; //Your Write API Key
WiFiClient client;

DHT dht(DHTPIN, DHTTYPE);

void setup(){

  Serial.begin(115200);

    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    ThingSpeak.begin(client);
}

void loop(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
  }
  else {
    // set the fields with the values
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);
  
    // write to the ThingSpeak channel 
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
      //delay 15s as the thingspeak free ver only allows u to send data 15s at a time
      delay(15000);
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
}