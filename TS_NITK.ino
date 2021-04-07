#include <ESP8266WiFi.h>
#include <DHT.h>
#include<SoftwareSerial.h>

#define DHTPIN D1
#define LDR A0
#define Rl 3300

String apiKey = "LXLNTR9QYEGSS2XP"; // LXLNTR9QYEGSS2XP
const char* ssid = "NITK-NET";
const char* pass = "2K16NITK";
const char* server = "api.thingspeak.com";
#define IO_USERNAME  "ravana1804"
#define IO_KEY       "aio_HhxC06kGhpvV93CHRocJUGkWIMHq"
float hum, temp, light;

DHT dht(DHTPIN, DHT11);
WiFiClient client;
void setup() {
  
  // put your main code here, to run repeatedly:
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
  String data = "mode=191&username=181134&password=pravan18&producttype=0&a=0";
  Serial.print("Requesting POST: ");
   // Send request to the server:
  client.println("POST / HTTP/1.1");
  client.println("Host: nac.nitk.ac.in:8090/login.xml");
  client.println("Connection:close");
  client.println("Content-Type: html/xml");
  client.print("Content-Length:256 ");
  client.println(data.length());
  client.println();
  client.print(data);
  delay(2000);
  }
  Serial.println("");
  Serial.println(" -- Wifi Connected -- ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  if(WiFi.status() ==  WL_CONNECTED)
  {
    Serial.println("Connected");
  }
  

  // calculate the light intensity
  float light_read = analogRead(light);
  float pot_diff = (light_read/1024)*5;
  float light_in = 500/((Rl*3300)/(5 - pot_diff));

  light_in = 0.0;
  
  if(client.connect(server, 80))
  {
    bool check=client.connect(server,80);
    Serial.println(check);
    String sendData = apiKey+"&field1="+String(temp)+"&field2="+String(hum)+"&field3="+String(light_in)+"\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(sendData.length());
    client.print("\n\n");
    client.print(sendData);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("C. Humidity: ");
    Serial.print(hum);
    Serial.print("%. Light Intensity: ");
    Serial.print(". Connecting to Thingspeak..");
  }
  Serial.println("Sending..");
  delay(2500);
  
  }
