# 💧 AquaGuard: Smart Water Quality Monitoring & Auto Shut-Off System

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Arduino%20UNO-teal.svg)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-orange.svg)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/Status-In%20Progress-yellow.svg)]()

---

## 📌 Overview

**AquaGuard** is a low-cost, real-time water quality monitoring system built on Arduino UNO. It continuously measures key water parameters and **automatically shuts off the water supply** when unsafe conditions are detected — protecting human health, agriculture, and aquatic life.

This project is designed for use in:
- 🏠 Households
- 🌾 Agriculture / Farming
- 🐟 Fish ponds / Aquaculture

---

## 🎯 Objectives

- Monitor **pH, TDS, turbidity, temperature, and hardness** in real-time
- Detect **arsenic contamination** using a color sensor and reagent kit
- Display all readings on a **16×2 I2C LCD**
- **Automatically shut off** the water pump via relay when unsafe levels are detected
- Trigger a **buzzer alarm** when water quality is poor
- Provide a **scalable and affordable** solution for water safety

---

## 🔧 Components

| Component | Description |
|---|---|
| Arduino UNO | Main microcontroller |
| pH Sensor Module (BNC) | Measures acidity/basicity |
| TDS Sensor (Gravity/Analog) | Measures dissolved solids |
| DS18B20 Waterproof Sensor | Measures water temperature |
| Turbidity Sensor (Analog) | Measures water clarity |
| TCS34725 Color Sensor | Detects arsenic color change |
| Arsenic Test Kit | Reagent-based arsenic detection |
| 16×2 I2C LCD Display | Shows all sensor readings |
| Relay Module | Controls water pump |
| Buzzer | Alarm for unsafe water |
| Servo Motor (Optional) | Controls valve |
| 1.5-inch uPVC Pipe | Water flow channel |
| 5V Adapter / USB | Power supply |
| Jumper wires, resistors, O-rings | Supporting components |

---

## 🔌 Circuit Connections

| Sensor / Component | Arduino Pin |
|---|---|
| pH Sensor | A0 |
| TDS Sensor | A1 |
| Turbidity Sensor | A2 |
| DS18B20 Temperature | D2 |
| TCS34725 Color Sensor | I2C (A4-SDA, A5-SCL) |
| 16×2 I2C LCD | I2C (A4-SDA, A5-SCL) |
| Relay Module | D8 |
| Buzzer | D9 |

> All sensors share a **common ground**.  
> All I2C devices operate on the **same I2C bus**.

---

## ⚙️ How It Works

1. Water flows through a **1.5-inch uPVC pipe** fitted with sensors at T-joints
2. Sensors **continuously measure** pH, TDS, turbidity, temperature, and hardness
3. For arsenic detection, water reacts with reagents — the **TCS34725 color sensor** reads the resulting color change and compares it to a predefined chart
4. All data is **processed by Arduino** and shown on the LCD
5. If any parameter **exceeds safe limits**:
   - 🔴 Relay shuts off the water pump
   - 🔊 Buzzer alarm activates
6. When water returns to safe levels → system **resumes automatically**

---

## 📊 Safe Limit Reference

| Parameter | Safe Range |
|---|---|
| pH | 6.5 – 8.5 |
| TDS | < 500 ppm |
| Turbidity | < 4 NTU |
| Temperature | 10°C – 25°C |
| Arsenic | < 0.01 mg/L |

---

## 🚀 Future Plans

- [ ] Add IoT connectivity (ESP8266/ESP32 WiFi module)
- [ ] Build a mobile app dashboard for remote monitoring
- [ ] Integrate AI-based water quality prediction
- [ ] Add cloud data logging (ThingSpeak / Firebase)
- [ ] Solar power support for remote areas

---

## 🤝 Contributing

This is an **open-source project** and contributions are welcome!

1. Fork this repository
2. Create your feature branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add your feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## 👤 Author

Made with ❤️ for clean and safe water for everyone.

> *"Clean water is not a luxury — it is a right."*
