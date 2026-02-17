#ifndef IMSMS_TYPES_H
#define IMSMS_TYPES_H

#include <stdint.h>

/**
 * @brief Machine telemetry data structure
 * Sensensed parameters sent from Station to Gateway.
 */
typedef struct {
    char station_name[16];   // e.g., "STA_XXXX"
    float temperature;       // in Celsius
    float humidity;          // in %
    float current;           // in Amperes
    bool vibration;          // true if vibration detected
    uint32_t uptime_seconds; // total run time in seconds
} machine_data_t;

// Network configuration constants
#define GATEWAY_AP_SSID_PREFIX "IMSMS_GATEWAY_"
#define GATEWAY_STATIC_IP      "192.168.4.1"
#define GATEWAY_PORT           8080

#endif // IMSMS_TYPES_H
