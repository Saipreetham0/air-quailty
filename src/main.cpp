
#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>

// ===== DWIN UART SETTINGS =====
const byte rxPin = 16;
const byte txPin = 17;
HardwareSerial dwin(1);

// ===== PIN DEFINITIONS =====
const int measurePin = 36; // GP2Y1010 Vo
const int ledPin = 4;      // GP2Y LED control
const int MQ135_PIN = 35;  // MQ135 analog pin
const int DHT_PIN = 15;    // DHT11 data pin

// ===== DHT SETTINGS =====
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// ===== DWIN ADDRESSES =====
#define add_61   0x61  // PM2.5
#define add_62   0x62  // PM10
#define add_63   0x63  // AQI
#define add_64   0x64  // Temperature
#define add_65   0x65  // Humidity
#define add_66   0x66  // VOCs
#define add_67   0x67  // NH3
#define add_68   0x68  // Benzene-type
#define add_69   0x69  // Smoke level
#define add_71   0x71  // CO2

// ===== DWIN FRAMES =====
unsigned char S61[8] = {0x5a,0xa5,0x05,0x82, add_61, 0x00,0x00,0x00};
unsigned char S62[8] = {0x5a,0xa5,0x05,0x82, add_62, 0x00,0x00,0x00};
unsigned char S63[8] = {0x5a,0xa5,0x05,0x82, add_63, 0x00,0x00,0x00};
unsigned char S64[8] = {0x5a,0xa5,0x05,0x82, add_64, 0x00,0x00,0x00};
unsigned char S65[8] = {0x5a,0xa5,0x05,0x82, add_65, 0x00,0x00,0x00};
unsigned char S66[8] = {0x5a,0xa5,0x05,0x82, add_66, 0x00,0x00,0x00};
unsigned char S67[8] = {0x5a,0xa5,0x05,0x82, add_67, 0x00,0x00,0x00};
unsigned char S68[8] = {0x5a,0xa5,0x05,0x82, add_68, 0x00,0x00,0x00};
unsigned char S69[8] = {0x5a,0xa5,0x05,0x82, add_69, 0x00,0x00,0x00};
unsigned char S71[8] = {0x5a,0xa5,0x05,0x82, add_71, 0x00,0x00,0x00};

// ===== AQI BREAKPOINT STRUCTURE =====
struct AQIBreakpoint {
  float Clow;
  float Chigh;
  int Ilow;
  int Ihigh;
};

// US EPA PM2.5 AQI TABLE
AQIBreakpoint pm25_table[] = {
  {0.0, 12.0, 0, 50},
  {12.1, 35.4, 51, 100},
  {35.5, 55.4, 101, 150},
  {55.5, 150.4, 151, 200},
  {150.5, 250.4, 201, 300},
  {250.5, 350.4, 301, 400},
  {350.5, 500.4, 401, 500},
};

// ===== AQI Calculation Function =====
int calculateAQI(float concentration) {
  for (int i = 0; i < 7; i++) {
    if (concentration >= pm25_table[i].Clow &&
        concentration <= pm25_table[i].Chigh) {

      return ((pm25_table[i].Ihigh - pm25_table[i].Ilow) /
              (pm25_table[i].Chigh - pm25_table[i].Clow)) *
             (concentration - pm25_table[i].Clow) +
             pm25_table[i].Ilow;
    }
  }
  return -1; // Out of range
}


// ===== SEND DATA TO DWIN DISPLAY =====
void sendToDWIN(float pm25, float pm10, int aqi, float temp, float hum, float vocs, float nh3, float benzene, int smoke, float co2) {
  // Convert float values to integers for DWIN (you can multiply by 10 if you want decimal precision)
  int v61 = (int)pm25;
  int v62 = (int)pm10;
  int v63 = aqi;
  int v64 = (int)temp;
  int v65 = (int)hum;
  int v66 = (int)vocs;
  int v67 = (int)nh3;
  int v68 = (int)benzene;
  int v69 = smoke;
  int v71 = (int)co2;

  // Send to DWIN
  S61[6] = highByte(v61); S61[7] = lowByte(v61); dwin.write(S61, 8);
  S62[6] = highByte(v62); S62[7] = lowByte(v62); dwin.write(S62, 8);
  S63[6] = highByte(v63); S63[7] = lowByte(v63); dwin.write(S63, 8);
  S64[6] = highByte(v64); S64[7] = lowByte(v64); dwin.write(S64, 8);
  S65[6] = highByte(v65); S65[7] = lowByte(v65); dwin.write(S65, 8);
  S66[6] = highByte(v66); S66[7] = lowByte(v66); dwin.write(S66, 8);
  S67[6] = highByte(v67); S67[7] = lowByte(v67); dwin.write(S67, 8);
  S68[6] = highByte(v68); S68[7] = lowByte(v68); dwin.write(S68, 8);
  S69[6] = highByte(v69); S69[7] = lowByte(v69); dwin.write(S69, 8);
  S71[6] = highByte(v71); S71[7] = lowByte(v71); dwin.write(S71, 8);
}



void setup() {
  Serial.begin(115200);
  dwin.begin(115200, SERIAL_8N1, rxPin, txPin);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED OFF initially

  dht.begin();

  Serial.println("Air Quality Monitor with DWIN Display started");
}

void loop() {

  // === GP2Y1010 Dust Sensor (Correct Timing) ===
  digitalWrite(ledPin, LOW);   // LED ON
  delayMicroseconds(280);

  int sensorValue = analogRead(measurePin);

  delayMicroseconds(40);
  digitalWrite(ledPin, HIGH);  // LED OFF

  // Convert ADC to voltage
  float voltage = sensorValue * (3.3 / 4095.0);

  // Sharp GP2Y1010 dust calculation
  float dustDensity = (voltage - 0.1) * 1000.0;
  if (dustDensity < 0) dustDensity = 0;

  // PM10 estimate (ratio-based)
  float pm10 = dustDensity * 1.5;

  // === Clamp PM2.5 to EPA range (max 500.4) ===
  float pm25 = dustDensity;
  if (pm25 > 500.4) pm25 = 500.4;

  // === AQI Calculation ===
  int aqi = calculateAQI(pm25);

  // === MQ135 GAS SENSOR ===
  int mq135_raw = analogRead(MQ135_PIN);

  float vocs = mq135_raw / 10.0;         // VOCs estimated
  float nh3 = mq135_raw * 0.2;           // NH3 estimated
  float benzene = mq135_raw * 0.15;      // Benzene-type gases
  int smokeLevel = map(mq135_raw, 0, 4095, 0, 100); // Smoke % (relative)
  float co2_ppm = 150 + (mq135_raw * 0.8); // CO2 equivalent estimate

  // === DHT11 Temperature and Humidity ===
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // === OUTPUT SECTION ===
  Serial.println("======================================");

  Serial.print("PM2.5: ");
  Serial.print(dustDensity);
  Serial.println(" µg/m³");

  Serial.print("PM10 (estimated): ");
  Serial.print(pm10);
  Serial.println(" µg/m³");

  Serial.print("AQI: ");
  Serial.println(aqi);

  Serial.print("Voltage: ");
  Serial.println(voltage, 3);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("VOCs: ");
  Serial.print(vocs);
  Serial.println(" mg/m³");

  Serial.print("NH3 (estimated): ");
  Serial.print(nh3);
  Serial.println(" ppm");

  Serial.print("Benzene-type gases: ");
  Serial.print(benzene);
  Serial.println(" ppm");

  Serial.print("Smoke Level: ");
  Serial.print(smokeLevel);
  Serial.println(" %");

  Serial.print("CO2 (estimated): ");
  Serial.print(co2_ppm);
  Serial.println(" ppm");

  Serial.println("======================================\n");

  // === SEND DATA TO DWIN DISPLAY ===
  sendToDWIN(dustDensity, pm10, aqi, temperature, humidity, vocs, nh3, benzene, smokeLevel, co2_ppm);

  delay(1500);
}
