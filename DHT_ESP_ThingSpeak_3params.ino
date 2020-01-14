#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D1
#define LDR A0
#define Rl 3300

String apiKey = "LXLNTR9QYEGSS2XP"; // LXLNTR9QYEGSS2XP
const char* ssid = "Ayush RedMi";
const char* pass = "12345678";
const char* server = "api.thingspeak.com";

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

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("\|/|");
    
  }
  Serial.println("");
  Serial.println(" -- Wifi Connected -- ");
}

void loop() {
  // put your setup code here, to run once:
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  // calculate the light intensity
  float light_read = analogRead(light);
  float pot_diff = (light_read/1024)*5;
  float light_in = 500/((Rl*3300)/(5 - pot_diff));

  light_in = 0.0;
  
  if(client.connect(server, 80))
  {
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
  client.stop();
  Serial.println("Sending..");
  delay(10000);
  
  }
