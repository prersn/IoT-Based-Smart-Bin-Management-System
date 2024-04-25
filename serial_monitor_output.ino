#include <Arduino.h>
#include <HX711.h>

const int LOADCELL_DOUT_PIN = 2;   
const int LOADCELL_SCK_PIN = 4;    
HX711 scale;

const int LASER_PIN = 5;  
const int LED_PIN = 18;  
unsigned long laserCutOffStartTime = 0;  

void setup() {
  Serial.begin(115200);
  pinMode(LASER_PIN, INPUT_PULLUP);  
  pinMode(LED_PIN, OUTPUT); 
  
 
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(107.883);  
  scale.tare();
  
  
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
  } 
  else if (weight > 100)
  {
    Serial.print("Bin full, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, HIGH); 
  }
  else if (isLaserCutOffFor(5000))
  {
    Serial.print("Bin full, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, HIGH); 
  }
  else {
  
    Serial.print("Bin underweight, Weight: ");
    Serial.print(weight);
    Serial.println(" g");
    digitalWrite(LED_PIN, LOW); 
  }
  
  
  delay(1000);
}
