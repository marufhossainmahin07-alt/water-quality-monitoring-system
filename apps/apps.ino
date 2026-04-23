#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP32Servo.h>

const char* ssid = "Ma***********";
const char* password = "******************";

// 🔥 Use correct Firebase URL
String firebaseURL = "https://servo-control-ed374-default-rtdb.firebaseio.com/control.json";

Servo servoLower;
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);

  servoLower.attach(18);
  servoLower.write(0);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop() {

  client.setInsecure();  // IMPORTANT

  HTTPClient https;

  if (https.begin(client, firebaseURL)) {

    int httpCode = https.GET();

    if (httpCode > 0) {

      String payload = https.getString();
      Serial.println("Data:");
      Serial.println(payload);

      // -------- SIMPLE PARSING --------
      if (payload.indexOf("\"servo\":60") != -1) {
        servoLower.write(60);   // CLOSE
        Serial.println("Servo CLOSE");
      }
      else if (payload.indexOf("\"servo\":0") != -1) {
        servoLower.write(0);    // OPEN
        Serial.println("Servo OPEN");
      }

    } else {
      Serial.println("Read fail");
    }

    https.end();

  } else {
    Serial.println("Connection failed");
  }

  delay(3000);
}