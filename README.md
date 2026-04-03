# 🌡️ Cocoon Shed Automation System (ESP8266 + Blynk)

An IoT-based automation system designed to maintain optimal environmental conditions inside a cocoon shed using temperature and humidity monitoring. The system automatically controls a fan and a bulb using an ESP8266 and Blynk cloud.

---

## 🚀 Features

- 📡 Real-time temperature & humidity monitoring (DHT22)
- 🤖 Automatic control based on temperature thresholds
- 🎛 Manual control via Blynk app
- 🔌 Relay-based switching (Fan & Bulb)
- 📊 Live data visualization in Blynk dashboard
- 🔄 Auto + Manual mode switching

---

## 🛠️ Hardware Requirements

- ESP8266 (NodeMCU)
- DHT22 Temperature & Humidity Sensor
- 2-Channel Relay Module (Active HIGH)
- Fan
- Bulb
- Jumper wires & power supply

---

## 📌 Pin Configuration

| Component     | GPIO Pin | NodeMCU Pin |
|--------------|----------|------------|
| DHT22        | GPIO14   | D5         |
| Fan Relay    | GPIO5    | D1         |
| Bulb Relay   | GPIO4    | D2         |

---

## 📱 Blynk Virtual Pins

| Virtual Pin | Function                     |
|------------|------------------------------|
| V0         | Mode Switch (Auto/Manual)    |
| V1         | Temperature Display          |
| V2         | Humidity Display             |
| V3         | Fan Status Indicator         |
| V4         | Manual Fan Control           |
| V5         | Bulb Status Indicator        |
| V6         | Manual Bulb Control          |

---

## ⚙️ Working Logic

### 🔹 AUTO Mode
- First cycle: All devices remain OFF (safety initialization)
- If temperature > **28°C** → Fan turns ON
- If temperature < **23°C** → Bulb turns ON
- Otherwise → Both OFF

### 🔹 MANUAL Mode
- User directly controls:
  - Fan via Blynk (V4)
  - Bulb via Blynk (V6)

---

## 🔐 Configuration

Update these credentials in the code:

```cpp
#define BLYNK_TEMPLATE_ID   "Your_Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Template_Name"
#define BLYNK_AUTH_TOKEN    "Your_Auth_Token"

char ssid[] = "Your_WiFi_Name";
char pass[] = "Your_WiFi_Password";
