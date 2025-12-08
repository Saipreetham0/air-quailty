#ifndef CONFIG_H
#define CONFIG_H

// ===== WIFI CREDENTIALS =====
// Replace with your WiFi network credentials
#define WIFI_SSID "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_WiFi_Password"

// ===== THINGSPEAK SETTINGS =====
// Replace with your ThingSpeak channel ID and Write API Key
// Get these from https://thingspeak.com after creating a channel
#define THINGSPEAK_CHANNEL_ID 0000000  // Your Channel ID (e.g., 1234567)
#define THINGSPEAK_WRITE_API_KEY "YOUR_WRITE_API_KEY"  // Your Write API Key (16 characters)

/*
 * THINGSPEAK SETUP INSTRUCTIONS:
 * 1. Go to https://thingspeak.com and create a free account
 * 2. Create a new channel
 * 3. Add 8 fields with these names:
 *    - Field 1: PM2.5
 *    - Field 2: PM10
 *    - Field 3: AQI
 *    - Field 4: Temperature
 *    - Field 5: Humidity
 *    - Field 6: VOCs
 *    - Field 7: CO2
 *    - Field 8: Smoke Level
 * 4. Copy your Channel ID and Write API Key to the defines above
 * 5. Update your WiFi SSID and Password
 */

#endif // CONFIG_H
