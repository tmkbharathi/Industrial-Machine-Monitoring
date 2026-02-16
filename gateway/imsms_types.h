#ifndef IMSMS_TYPES_H
#define IMSMS_TYPES_H

#include <stdint.h>

// Enable Ethernet mode for networking
#define ETHERNET_MODE

/**
 * @brief Machine telemetry data structure
 * Sensensed parameters sent from Station to Gateway.
 */
typedef struct {
  char station_name[16];   // e.g., "STA_XXXX"
  float temperature;       // in Celsius
  float humidity;          // in %
  float current;           // in Amps/Decimal
  uint8_t vibration;       // Boolean (0: No Vibration, 1: Vibration Detected)
  uint32_t uptime_seconds; // total run time in current shift
} machine_data_t;

// Network configuration constants
#define GATEWAY_PORT 8080

// Ethernet Configuration for Arduino Uno (PicSimLab)
#define GATEWAY_MAC {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}
#define GATEWAY_IP_AVR {192, 168, 1, 100}

#endif // IMSMS_TYPES_H
