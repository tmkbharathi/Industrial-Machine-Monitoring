#ifndef IMSMS_TYPES_H
#define IMSMS_TYPES_H

#include <Arduino.h>
#include <stdint.h>


// Global Mode Flags
#define ETHERNET_MODE
#if defined(ARDUINO_ARCH_AVR)
#define SIMULATION_MODE
#endif

/**
 * @brief Machine telemetry data structure
 * Sensed parameters sent from Station to Gateway or Cloud.
 */
typedef struct {
  char station_name[16];   // e.g., "STA_XXXX"
  float temperature;       // in Celsius
  float humidity;          // in %
  float current;           // in Amperes
  bool vibration;          // true if vibration detected (Latched)
  uint32_t uptime_seconds; // total run time in seconds
} machine_data_t;

// --- Network Configuration ---
#define GATEWAY_PORT 8080
#define GATEWAY_STATIC_IP "192.168.4.1" // For AP Mode
#define GATEWAY_AP_SSID_PREFIX "IMSMS_GATEWAY_"

// Ethernet Configuration for Arduino Uno (PicSimLab)
#define GATEWAY_MAC {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}
#define STATION_MAC {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE}
#define GATEWAY_IP_AVR {192, 168, 1, 100}
#define STATION_IP_AVR {192, 168, 1, 101}

// --- Cloud Configuration ---
#define THINGSPEAK_SERVER "api.thingspeak.com"
#define THINGSPEAK_API_KEY "EV2QPG4NHABLSWQT"
#define THINGSPEAK_CHANNEL_ID 3266149

// --- Serial Simulation Constants ---
#define SERIAL_SYNC_BYTE 0xAA

#endif // IMSMS_TYPES_H
