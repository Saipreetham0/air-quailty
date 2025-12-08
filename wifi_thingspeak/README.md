# Air Quality Monitor - WiFi + ThingSpeak Version

This folder contains the WiFi-enabled version of your air quality monitor that sends data to ThingSpeak cloud platform.

## Files Created

- `main.cpp` - Main program with WiFi and ThingSpeak integration
- `config.h` - Configuration file for WiFi and ThingSpeak credentials
- `platformio.ini` - PlatformIO configuration with required libraries

## Setup Instructions

### 1. ThingSpeak Account Setup

1. Go to [https://thingspeak.com](https://thingspeak.com) and create a free account
2. Click on "Channels" > "New Channel"
3. Create a channel with 8 fields:
   - **Field 1**: PM2.5
   - **Field 2**: PM10
   - **Field 3**: AQI
   - **Field 4**: Temperature
   - **Field 5**: Humidity
   - **Field 6**: VOCs
   - **Field 7**: CO2
   - **Field 8**: Smoke Level
4. Save the channel
5. Go to "API Keys" tab and copy:
   - Channel ID
   - Write API Key

### 2. Configure WiFi and ThingSpeak

1. Open `config.h` file
2. Replace the following values:
   ```cpp
   #define WIFI_SSID "Your_WiFi_SSID"           // Your WiFi network name
   #define WIFI_PASSWORD "Your_WiFi_Password"    // Your WiFi password
   #define THINGSPEAK_CHANNEL_ID 0000000         // Your ThingSpeak Channel ID
   #define THINGSPEAK_WRITE_API_KEY "YOUR_KEY"   // Your Write API Key
   ```

### 3. Upload to ESP32

**Option 1: Move files to main project**
```bash
# Copy the main.cpp and config.h to your src folder
cp wifi_thingspeak/main.cpp src/main.cpp
cp wifi_thingspeak/config.h src/config.h

# Copy the platformio.ini to root
cp wifi_thingspeak/platformio.ini platformio.ini

# Then build and upload
pio run --target upload
```

**Option 2: Create new PlatformIO project**
```bash
cd wifi_thingspeak
pio project init --board esp32doit-devkit-v1

# Move main.cpp to src folder
mkdir -p src
mv main.cpp src/
mv config.h src/

# Then build and upload
pio run --target upload
```

## Features

- Reads all air quality sensors (GP2Y1010, MQ135, DHT11)
- Calculates PM2.5, PM10, AQI, Temperature, Humidity, VOCs, CO2, and Smoke levels
- Sends data to DWIN display (original functionality)
- **NEW**: Connects to WiFi
- **NEW**: Uploads data to ThingSpeak every 20 seconds
- **NEW**: Shows WiFi connection status in Serial Monitor

## Data Update Rate

- Local display updates: Every 1.5 seconds
- ThingSpeak updates: Every 20 seconds (respects ThingSpeak free tier limit of 15 seconds)

## Monitoring Your Data

After uploading your code and powering on the device:

1. Go to your ThingSpeak channel
2. View real-time charts of all 8 parameters
3. Export data for analysis
4. Create custom visualizations
5. Set up alerts and notifications (ThingSpeak features)

## Troubleshooting

**WiFi won't connect:**
- Check SSID and password in config.h
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Check WiFi signal strength

**ThingSpeak errors:**
- Verify Channel ID and Write API Key
- Ensure updates are at least 15 seconds apart
- Check free tier limits (8200 messages per day)

**Serial Monitor shows connection status:**
```
Air Quality Monitor with WiFi & ThingSpeak started
Connecting to WiFi: Your_Network
.....
WiFi Connected!
IP Address: 192.168.1.xxx
System ready!
```

## Libraries Used

- DHT sensor library (Adafruit)
- ThingSpeak library (MathWorks)
- WiFi (ESP32 built-in)

## Hardware Connections

Same as original project:
- GP2Y1010 dust sensor on GPIO36
- GP2Y LED control on GPIO4
- MQ135 gas sensor on GPIO35
- DHT11 on GPIO15
- DWIN display on UART1 (GPIO16/17)
