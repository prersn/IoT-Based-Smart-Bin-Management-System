#include "header.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 4;
HX711 scale;

const int LASER_PIN = 5;
const int LED_PIN = 18;
unsigned long laserCutOffStartTime = 0;

#define AWS_IOT_PUBLISH_TOPIC   ""
#define AWS_IOT_SUBSCRIBE_TOPIC ""

 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
 
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}
 
void setup() {
  Serial.begin(115200);
  pinMode(LASER_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(107.883);
  scale.tare();

  connectWiFi();
  connectAWS();

  laserCutOffStartTime = 0;
}

bool isLaserCutOff() {
  int laserStatus = digitalRead(LASER_PIN);
  return (laserStatus == LOW);
}

bool isLaserCutOffFor(unsigned long duration) {
  if (isLaserCutOff()) {
    unsigned long currentTime = millis();
    if (currentTime - laserCutOffStartTime >= duration) {
      return true;
    }
  } else {
    laserCutOffStartTime = millis();
  }
  return false;
}

void publishMessage(float weight) {
  StaticJsonDocument(200) doc;
  doc["weight"] = weight;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void loop() {
  float weight = scale.get_units(10);

  if (isLaserCutOff()) {
    Serial.println("Laser Cut Off");
  } else {
    Serial.println("Laser Receiving");
  }

  if (weight > 100 && isLaserCutOffFor(5000)) {
    Serial.print("Bin full, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, HIGH);
    publishMessage(weight); // Publish weight to AWS IoT
  } 
  else if (weight > 100) {
    Serial.print("Bin full, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, HIGH);
    publishMessage(weight); // Publish weight to AWS IoT
  } 
  else if (isLaserCutOffFor(5000)) {
    Serial.print("Bin full, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, HIGH);
    publishMessage(weight); // Publish weight to AWS IoT
  } 
  else {
    Serial.print("Bin underweight, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
  client.loop(); // Maintain MQTT connection
}
