#include "imsms_types.h"
#include <Arduino.h>

#ifdef ETHERNET_MODE
#include <Ethernet.h>
#include <EthernetServer.h>
#endif

// Task handles or state variables
#ifdef ETHERNET_MODE
byte mac_eth[] = GATEWAY_MAC;
IPAddress ip_eth GATEWAY_IP_AVR;

// Fix for ESP32 compilation error with Ethernet library
// EthernetServer is considered abstract on ESP32 due to missing begin(uint16_t)
class ClientServer : public EthernetServer {
public:
  ClientServer(uint16_t port) : EthernetServer(port) {}
#if defined(ARDUINO_ARCH_ESP32)
  void begin(uint16_t port = 0) override { EthernetServer::begin(); }
#endif
};

ClientServer server(GATEWAY_PORT);
#endif

// Node tracking
#define MAX_NODES 10
machine_data_t nodeRegistry[MAX_NODES];
int connectedNodesCount = 0;

void handleClientConnections();
void displayNodeStatus();

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println(
      "\n--- Industrial Machine Monitoring System Gateway (Ethernet Only) ---");
  Serial.println("Initializing Gateway...");

  // Initialize registry
  connectedNodesCount = 0;

#ifdef ETHERNET_MODE
  Serial.println("Initializing Ethernet Shield...");
  Ethernet.begin(mac_eth, ip_eth);
  server.begin();
  Serial.print("Gateway IP: ");
  Serial.println(Ethernet.localIP());
  Serial.println("Gateway Server started via Ethernet. Listening for Nodes...");
#else
  Serial.println("\n[WARNING] Ethernet mode not active. Simulation fallbacks "
                 "will be used.");
#endif
}

void loop() {
  handleClientConnections();

  // Periodically display node status every 10 seconds if nodes are connected
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 10000) {
    if (connectedNodesCount > 0) {
      displayNodeStatus();
    }
    lastUpdate = millis();
  }

  delay(100);
}

void handleClientConnections() {
#ifdef ETHERNET_MODE
  EthernetClient client = server.available();
  if (client) {
    Serial.println("\n[INFO] New Node Connecting...");

    while (client.connected()) {
      if (client.available() >= sizeof(machine_data_t)) {
        machine_data_t newData;
        client.read((uint8_t *)&newData, sizeof(machine_data_t));

        // Update registry
        bool found = false;
        for (int i = 0; i < connectedNodesCount; i++) {
          if (strcmp(nodeRegistry[i].station_name, newData.station_name) == 0) {
            nodeRegistry[i] = newData;
            found = true;
            break;
          }
        }

        if (!found && connectedNodesCount < MAX_NODES) {
          nodeRegistry[connectedNodesCount] = newData;
          connectedNodesCount++;
          Serial.print("[INFO] New Node Registered via Ethernet: ");
          Serial.println(newData.station_name);
        } else if (!found) {
          Serial.println("[WARN] Max nodes reached. Cannot register new node.");
        }
      }
      break;
    }
    client.stop();
  }
#endif
}

void displayNodeStatus() {
  Serial.println(F("\n--- Connected Nodes Status ---"));
  Serial.println(F("Name       | Temp  | Hum   | Current | Vibr  | Uptime "));
  Serial.println(
      F("----------------------------------------------------------"));
  for (int i = 0; i < connectedNodesCount; i++) {
    Serial.print(nodeRegistry[i].station_name);
    Serial.print(F(" | "));
    Serial.print(nodeRegistry[i].temperature, 1);
    Serial.print(F("  | "));
    Serial.print(nodeRegistry[i].humidity, 1);
    Serial.print(F("  | "));
    Serial.print(nodeRegistry[i].current, 2);
    Serial.print(F("    | "));
    Serial.print(nodeRegistry[i].vibration ? F("YES") : F("NO "));
    Serial.print(F("   | "));
    Serial.println(nodeRegistry[i].uptime_seconds);
  }
}
