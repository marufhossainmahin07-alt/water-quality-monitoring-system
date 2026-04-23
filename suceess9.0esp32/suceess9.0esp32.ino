#define BLYNK_TEMPLATE_ID   "TMPL6vvdjOt6Z"
#define BLYNK_TEMPLATE_NAME "water qulity monitoriny system"
#define BLYNK_AUTH_TOKEN    "MXAPYs5zjrX4_IiSDdL8tNDN4crrkWZj"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "realme C11 2021";
char pass[] = "00000000";

HardwareSerial mySerial(2);

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 16, 17);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  if (mySerial.available()) {

    String data = mySerial.readStringUntil('\n');
    Serial.println(data);

    float tds, ph, ntu, temp, cod, bod;
    int safe;

    sscanf(data.c_str(), "%f,%f,%f,%f,%f,%f,%d",
           &tds, &ph, &ntu, &temp, &cod, &bod, &safe);

    Blynk.virtualWrite(V0, safe * 100);
    Blynk.virtualWrite(V1, ph);
    Blynk.virtualWrite(V2, tds);
    Blynk.virtualWrite(V3, ntu);
    Blynk.virtualWrite(V4, temp);
    Blynk.virtualWrite(V5, cod);
    Blynk.virtualWrite(V6, bod);
  }
}

// Button control
BLYNK_WRITE(V11) {
  int valve = param.asInt();
  mySerial.print("VALVE:");
  mySerial.println(valve);
}