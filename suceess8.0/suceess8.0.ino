#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ---------------- Pins ----------------
#define waterSensor 8
#define redLed 12
#define greenLed 13

#define ONE_WIRE_BUS 7
#define TDS_PIN A1
#define PH_PIN A2
#define TURBIDITY_PIN A0

#define SERVO_LOWER_PIN 6

// ---------------- Sensors ----------------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Servo servoLower;

// ---------------- Variables ----------------
float temperature;
float tdsValue;
float phValue;
float ntu;
float COD;
float BOD;

// ---------------- CALIBRATION ----------------
float phSlope = -6.72;
float phIntercept = 18.11;

// Distilled water offset
float tdsOffset = 83.0;

// ---------------- TURBIDITY FILTER ----------------
float readTurbidity() {
  float sum = 0;

  for (int i = 0; i < 40; i++) {
    sum += analogRead(TURBIDITY_PIN);
    delay(2);
  }

  float avg = sum / 40.0;
  return avg * (5.0 / 1023.0);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(waterSensor, INPUT_PULLUP);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  sensors.begin();

  lcd.init();
  lcd.backlight();

  servoLower.attach(SERVO_LOWER_PIN);
  servoLower.write(60);   // default CLOSED

  lcd.setCursor(0, 0);
  lcd.print("Water System BD");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  delay(2000);
  lcd.clear();
}

// ---------------- LOOP ----------------
void loop() {

  // -------- Water Check --------
  if (digitalRead(waterSensor) == HIGH) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NO WATER SUPPLY");

    servoLower.write(60);
    delay(2000);
    return;
  }

  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, LOW);

  // -------- Temperature --------
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  // -------- TURBIDITY --------
  float voltageTurb = readTurbidity();

  ntu = (2.5 - voltageTurb) * 1000;
  if (ntu < 0) ntu = 0;
  if (ntu > 3000) ntu = 3000;

  // -------- TDS --------
  float rawTDS = analogRead(TDS_PIN) * 5.0 / 1023.0;

  float compensation = 1.0 + 0.02 * (temperature - 25.0);
  float corrected = rawTDS / compensation;

  tdsValue =
    (133.42 * pow(corrected, 3)
     - 255.86 * pow(corrected, 2)
     + 857.39 * corrected) * 0.5;

  tdsValue = tdsValue - tdsOffset;
  if (tdsValue < 0) tdsValue = 0;

  // -------- pH --------
  float sumPH = 0;

  for (int i = 0; i < 10; i++) {
    sumPH += analogRead(PH_PIN) * 5.0 / 1023.0;
    delay(20);
  }

  float avgPH = sumPH / 10.0;
  phValue = phSlope * avgPH + phIntercept;

  if (phValue < 0) phValue = 0;
  if (phValue > 14) phValue = 14;

  // -------- COD & BOD (estimated) --------
  COD = tdsValue * 0.01;
  BOD = tdsValue * 0.005;

  // -------- SAFETY CHECK --------
  bool safe = true;

  if (tdsValue > 500) safe = false;
  if (phValue < 6.5 || phValue > 8.5) safe = false;
  if (ntu > 5) safe = false;
  if (COD > 10) safe = false;
  if (BOD > 2) safe = false;

  // -------- VALVE CONTROL --------
  if (safe) {
    servoLower.write(0);   // OPEN
  } else {
    servoLower.write(60);  // CLOSE
  }

  // -------- SERIAL OUTPUT --------
  Serial.println("===== WATER REPORT =====");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("TDS: "); Serial.println(tdsValue);
  Serial.print("pH: "); Serial.println(phValue);
  Serial.print("NTU: "); Serial.println(ntu);
  Serial.print("COD: "); Serial.println(COD);
  Serial.print("BOD: "); Serial.println(BOD);
  Serial.println(safe ? "SAFE" : "UNSAFE");
  Serial.println("=======================");

  // -------- LCD DISPLAY --------
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("TDS:");
  lcd.print(tdsValue, 0);
  lcd.print(" pH:");
  lcd.print(phValue, 1);

  lcd.setCursor(0, 1);
  lcd.print("NTU:");
  lcd.print(ntu, 0);
  lcd.print(" T:");
  lcd.print(temperature, 1);

  lcd.setCursor(0, 2);
  lcd.print("COD:");
  lcd.print(COD, 1);
  lcd.print(" BOD:");
  lcd.print(BOD, 1);

  lcd.setCursor(0, 3);
  lcd.print(safe ? "STATUS: SAFE" : "STATUS: UNSAFE");

  delay(3000);
}