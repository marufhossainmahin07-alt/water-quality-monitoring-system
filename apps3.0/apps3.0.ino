#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ESP32Servo.h>

// ═══════════════════════════════════════════
//         CHANGE ONLY THESE 3 LINES
// ═══════════════════════════════════════════
#define WIFI_SSID ""                                                // your WiFi name
#define WIFI_PASSWORD ""                                              // your WiFi password
#define API_KEY ""                        // your Firebase API key
#define DATABASE_URL ""  // your Firebase URL
// ═══════════════════════════════════════════
//         DO NOT CHANGE BELOW THIS
// ═══════════════════════════════════════════

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Servo myServo;
bool firebaseReady = false;
int lastAngle = -1;  // track last angle to avoid repeat writes

void setup() {
  Serial.begin(115200);
  delay(1000);

  // ── Servo Setup ──
  myServo.attach(18);  // GPIO 18 → change if your servo is on different pin
  myServo.write(60);   // start fully CLOSED
  Serial.println("✅ Servo attached. Starting at 60° (CLOSED)");

  // ── WiFi Connect ──
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected: " + WiFi.localIP().toString());

  // ── Firebase Setup ──
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  // Anonymous login
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("✅ Firebase Auth OK");
    firebaseReady = true;
  } else {
    Serial.println("❌ Auth Failed: " + String(config.signer.signupError.message.c_str()));
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set Firebase starting value to 60 (closed)
  if (firebaseReady) {
    Firebase.RTDB.setInt(&fbdo, "/control/servo", 60);
  }
}

void loop() {
  // ── Wait for Firebase ──
  if (!firebaseReady || !Firebase.ready()) {
    Serial.println("⏳ Waiting for Firebase...");
    delay(1000);
    return;
  }

  // ── Read Angle from Firebase ──
  if (Firebase.RTDB.getInt(&fbdo, "/control/servo")) {
    int angle = fbdo.intData();

    // Safety: keep within 0 to 60 only
    angle = constrain(angle, 0, 60);

    // Only move servo if angle actually changed
    if (angle != lastAngle) {
      myServo.write(angle);
      lastAngle = angle;

      // ── Print Status ──
      if (angle == 0) {
        Serial.println("💧 Fully OPEN   → 0°  (Max water flow)");
      } else if (angle == 60) {
        Serial.println("🚫 Fully CLOSED → 60° (No water flow)");
      } else {
        Serial.println("🔧 Partial OPEN → " + String(angle) + "° (Pressure: " + String(map(angle, 0, 60, 100, 0)) + "%)");
      }
    }

  } else {
    Serial.println("❌ Firebase Read Error: " + fbdo.errorReason());
  }

  delay(500);
}
