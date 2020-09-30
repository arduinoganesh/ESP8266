#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "Pimple Manson";
const char* password = "sai@12345";

const char* apiwritekey = "ZXIKH8UOJMJTFUOP";
const char* host  =  "api.thingspeak.com";
float temp=0;
float humidity = 0;

DHT dht(D1,DHT11);

void setup() {
  
  Serial.begin(115200);
  dht.begin();
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
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
      readsensordata();
      sendToCloud();  
      delaySeconds(10);   
  
}

void sendToCloud()
{
  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/update?api_key=";
  url += apiwritekey; 
  url += "&field1=";
  url += temp;
  url += "&field2=";
  url += humidity;
   
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  }
void readsensordata()
{
  temp=dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print("Temp  = ");
  Serial.print(temp);
  Serial.print("Humidity = ");
  Serial.println(humidity);
  delay(50);
}

  void delaySeconds(int s)
  {
    for(int i=1;i<=s;i++)
         delay(1000);
    }
