#define BLYNK_TEMPLATE_ID   "TMPL3FAeFaz9u"
#define BLYNK_TEMPLATE_NAME "Cocoon Shed"
#define BLYNK_AUTH_TOKEN    "eeND_mqa03W1Ux9tIip_ggtgha"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

/* ===== WiFi ===== */
char ssid[] = "DESKTOP";
char pass[] = "12345678";

/* ===== Pin Definitions ===== */
#define DHTPIN 14        // D5
#define FAN_RELAY 5      // D1
#define BULB_RELAY 4     // D2

/* ===== RELAY LOGIC (FIX) ===== */
// 🔴 ACTIVE-HIGH relay module
#define RELAY_ON  HIGH
#define RELAY_OFF LOW

/* ===== DHT ===== */
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/* ===== Temperature Thresholds ===== */
float TEMP_MAX = 28.0;
float TEMP_MIN = 23.0;

BlynkTimer timer;

/* ===== Mode Variables ===== */
bool manualMode = false;   // false = AUTO
bool autoReady  = false;   // OFF → READ → ACT

/* ===== Relay Helpers ===== */
void turnFanOn() {
  digitalWrite(FAN_RELAY, RELAY_ON);
  Blynk.virtualWrite(V3, 1);
  Blynk.virtualWrite(V4, 1);
}

void turnFanOff() {
  digitalWrite(FAN_RELAY, RELAY_OFF);
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V4, 0);
}

void turnBulbOn() {
  digitalWrite(BULB_RELAY, RELAY_ON);
  Blynk.virtualWrite(V5, 1);
  Blynk.virtualWrite(V6, 1);
}

void turnBulbOff() {
  digitalWrite(BULB_RELAY, RELAY_OFF);
  Blynk.virtualWrite(V5, 0);
  Blynk.virtualWrite(V6, 0);
}

/* ===== Read Sensor ===== */
void readSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("❌ DHT read failed");
    return;
  }

  Serial.print("🌡 ");
  Serial.print(t);
  Serial.print(" °C | 💧 ");
  Serial.println(h);

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  if (!manualMode) {

    // First AUTO cycle → keep OFF
    if (!autoReady) {
      turnFanOff();
      turnBulbOff();
      autoReady = true;
      return;
    }

    // Normal AUTO logic
    turnFanOff();
    turnBulbOff();

    if (t > TEMP_MAX) {
      turnFanOn();
    }
    else if (t < TEMP_MIN) {
      turnBulbOn();
    }
  }
}

/* ===== Mode Switch ===== */
BLYNK_WRITE(V0) {
  manualMode = param.asInt();

  turnFanOff();
  turnBulbOff();
  autoReady = false;

  Serial.println(manualMode ? "🎛 MANUAL MODE" : "🤖 AUTO MODE");
}

/* ===== Manual Fan ===== */
BLYNK_WRITE(V4) {
  if (manualMode) {
    param.asInt() ? turnFanOn() : turnFanOff();
  }
}

/* ===== Manual Bulb ===== */
BLYNK_WRITE(V6) {
  if (manualMode) {
    param.asInt() ? turnBulbOn() : turnBulbOff();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(FAN_RELAY, OUTPUT);
  pinMode(BULB_RELAY, OUTPUT);

  // 🔑 FORCE OFF AT BOOT
  digitalWrite(FAN_RELAY, RELAY_OFF);
  digitalWrite(BULB_RELAY, RELAY_OFF);

  dht.begin();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    yield();
  }

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  Blynk.syncVirtual(V0, V4, V6);

  turnFanOff();
  turnBulbOff();

  timer.setInterval(5000L, readSensor);
}

void loop() {
  Blynk.run();
  timer.run();
  yield();
}
