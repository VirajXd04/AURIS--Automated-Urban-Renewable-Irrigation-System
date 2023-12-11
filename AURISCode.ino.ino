#include <RTClib.h>
const char* ssid = "VVP";        // WIFI network SSID
const char* password = "virajvedang";       // WIFI network PASSWORD
int port=8000;                             // Virtuino default Server port
const char* serverIP = "192.168.1.150";    // The three first numbers have to be the same with the router IP
RTC_DS3231 rtc;

#define RELAY_PIN 3
#define SOIL_SENSOR_PIN A0
#define dry 600
#define good 500
#define wet 400

float V[32];  // Adjust the array size based on your needs
boolean debug = true;

float V0_lastValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SOIL_SENSOR_PIN, INPUT);
  rtc.begin();
}

void loop() {
  int sensorValue = analogRead(SOIL_SENSOR_PIN);

  if (sensorValue > 500) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Pump ON! Soil is dry");
    delay(2000);
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pump OFF! Soil is good");
    delay(2000);
  }

  if (V[0] != V0_lastValue) {
    Serial.println("V0=" + String(V[0]));
    V0_lastValue = V[0];

    int sensorValue = analogRead(SOIL_SENSOR_PIN);
    V[1] = sensorValue;
  }

  int moisturePercentage = map(sensorValue, wet, dry, 0, 100);
  DateTime now = rtc.now();
  int timeNow = now.hour() * 60 + now.minute();
  int plannedPumpTime = 16 * 60 + 30;  // 4:30 PM in minutes
  int pumpDuration = 40;               // 40 seconds
  int checkInterval = 60 * 1000;       // 1 minute in milliseconds

  if (timeNow >= plannedPumpTime) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Pump ON! Scheduled irrigation.");
    delay(pumpDuration * 1000);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pump OFF! Scheduled irrigation complete.");
  }

  delay(checkInterval);
}
