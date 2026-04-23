

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ESP32Servo.h>

// Required helpers (IMPORTANT)
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// -------- WIFI --------
#define WIFI_SSID "Maruf Hossain"
#define WIFI_PASSWORD "01966365888"

// -------- FIREBASE --------
#define API_KEY "AIzaSyBb8-foiqLQg3o-7rGHmFHBX4ZE5f8VW00"
#define DATABASE_URL "https://servo-control-ed374-default-rtdb.firebaseio.com/"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Servo
Servo servoLower;

void setup() {
  Serial.begin(115200);

  // Servo setup
  servoLower.attach(18);
  servoLower.write(0);

  // ---------------- WIFI ----------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // ---------------- FIREBASE CONFIG ----------------
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // IMPORTANT: Token callback (fixes your error)
  config.token_status_callback = tokenStatusCallback;

  // SIMPLE LOGIN (no Gmail needed)
  auth.user.email = "";
  auth.user.password = "";

  // Start Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase Ready!");
}

void loop() {

  // Check WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost!");
    return;
  }

  // Read value from Firebase
  if (Firebase.RTDB.getInt(&fbdo, "/control/servo")) {

    int value = fbdo.intData();

    Serial.print("Servo Value: ");
    Serial.println(value);

    // Move servo
    servoLower.write(value);

  } else {
    Serial.print("Firebase Error: ");
    Serial.println(fbdo.errorReason());
  }

  delay(2000);
}