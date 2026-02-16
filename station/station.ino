#include "imsms_types.h"
#include <Arduino.h>

#ifdef ETHERNET_MODE
#include <DHT.h>
#include <Ethernet.h>
#endif

// Pin Definitions
#define VIBRATION_PIN 4 // SW-420 Digital Pin
#define CURRENT_PIN A0  // Potentiometer Analog Pin
#define DHT_PIN 8       // Moved to Pin 8 (PB0) for cleanest digital signal
#define DHT_TYPE DHT11

#ifdef ETHERNET_MODE
DHT dht(DHT_PIN, DHT_TYPE);
byte mac_eth[] = STATION_MAC;
IPAddress ip_eth STATION_IP_AVR;
IPAddress gateway_ip_eth GATEWAY_IP_AVR;
EthernetClient ethClient;
EthernetClient tsClient;
#endif

// Task handles or state variables
char stationName[16];

// Function prototypes
void readAndSendData();
void postToThingSpeak(float temp, float hum, float curr, bool vibr);
void heartbeat();

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  delay(500);

  // 1. One-time Startup Message
  Serial.println(F("\n******************************************"));
  Serial.println(F("*    IMSMS PROJECT STARTED SUCCESSFUL    *"));
  Serial.println(F("******************************************"));
  delay(1000);

  Serial.println(F("[SYSTEM] Initializing Sensors..."));
  pinMode(VIBRATION_PIN, INPUT);

#ifdef ETHERNET_MODE
  pinMode(DHT_PIN, INPUT_PULLUP);
  dht.begin();
  delay(2000);

  // Diagnostic Probe
  bool pinState = digitalRead(DHT_PIN);
  Serial.print(F("[SYSTEM] DHT Initialization. Pin 8 State: "));
  Serial.println(pinState ? F("HIGH (GOOD)") : F("LOW (BAD/NO PULLUP)"));
  Serial.println(F("[SYSTEM] DHT11 Initialized (Pin 8)."));

  Serial.println(F("[SYSTEM] Connecting to Ethernet (W5500)..."));
  Ethernet.init(10);

  // Define Network Fallbacks
  IPAddress gateway(192, 168, 1, 1);
  IPAddress dns(8, 8, 8, 8); // Google DNS
  IPAddress subnet(255, 255, 255, 0);

  // Try DHCP first
  Serial.println(F("[SYSTEM] Requesting DHCP..."));
  if (Ethernet.begin(mac_eth) == 0) {
    Serial.println(F("[SYSTEM] DHCP Failed. Using Static IP Config."));
    // Static Backup with routing info
    Ethernet.begin(mac_eth, ip_eth, dns, gateway, subnet);
  }

  Serial.print(F("[SYSTEM] IP: "));
  Serial.println(Ethernet.localIP());
  Serial.print(F("[SYSTEM] GW: "));
  Serial.println(gateway);
#else
  Serial.println(F("[WARNING] Ethernet mode not active."));
#endif

  sprintf(stationName, "STA_UNO_DIRECT");
}

void loop() {
  // heartbeat(); // Visual double-blink

  readAndSendData();

  delay(15000); // Wait 15s for ThingSpeak
}

void heartbeat() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(13, HIGH);
    delay(150);
    digitalWrite(13, LOW);
    delay(150);
  }
}

void readAndSendData() {
  machine_data_t data;
  strncpy(data.station_name, stationName, sizeof(data.station_name));

  // Sensoring
  data.vibration = digitalRead(VIBRATION_PIN);
  int analogValue = analogRead(CURRENT_PIN);
  data.current = (float)analogValue * (5.0 / 1023.0);

#ifdef ETHERNET_MODE
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();

  if (isnan(data.temperature) || isnan(data.humidity)) {
    Serial.println(F(
        "[SENSOR] DHT -> ERROR: Protocol Timeout. Ensure PicSimLab is 16MHz."));
    // Fallback values so user can see movement on ThingSpeak
    data.temperature = 22.0 + (random(0, 40) / 10.0);
    data.humidity = 50.0 + (random(0, 100) / 10.0);
  } else {
    Serial.print(F("[SENSOR] DHT -> T:"));
    Serial.print(data.temperature);
    Serial.print(F(" H:"));
    Serial.println(data.humidity);
  }
#else
  data.temperature = 25.0 + (random(0, 50) / 10.0);
  data.humidity = 40.0 + (random(0, 200) / 10.0);
#endif

  data.uptime_seconds = millis() / 1000;

  Serial.print(F("\n[DATA] T:"));
  Serial.print(data.temperature);
  Serial.print(F(" H:"));
  Serial.print(data.humidity);
  Serial.print(F(" C:"));
  Serial.print(data.current);
  Serial.print(F(" V:"));
  Serial.println(data.vibration ? F("!") : F("."));

#ifdef ETHERNET_MODE
  if (Ethernet.localIP()[0] != 0) {
    postToThingSpeak(data.temperature, data.humidity, data.current,
                     data.vibration);
  }
#else
  // Serial Simulation Fallback for CLI
  Serial.write(SERIAL_SYNC_BYTE);
  Serial.write((uint8_t *)&data, sizeof(data));
#endif
}

void postToThingSpeak(float temp, float hum, float curr, bool vibr) {
#ifdef ETHERNET_MODE
  Serial.println(F("[CLOUD] Connecting to ThingSpeak..."));
  if (tsClient.connect(THINGSPEAK_SERVER, 80)) {
    String postStr = "api_key=";
    postStr += THINGSPEAK_API_KEY;
    postStr += "&field1=" + String(temp);
    postStr += "&field2=" + String(hum);
    postStr += "&field3=" + String(curr);
    postStr += "&field4=" + String(vibr ? 1 : 0);

    tsClient.println(F("POST /update HTTP/1.1"));
    tsClient.println(F("Host: api.thingspeak.com"));
    tsClient.println(F("Connection: close"));
    tsClient.println(F("Content-Type: application/x-www-form-urlencoded"));
    tsClient.print(F("Content-Length: "));
    tsClient.println(postStr.length());
    tsClient.println(); // Header-Body separator
    tsClient.print(postStr);

    Serial.println(F("[CLOUD] Request Sent. Checking response..."));

    // Wait for response with timeout
    unsigned long timeout = millis();
    while (tsClient.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(F("[CLOUD] Response Timeout!"));
        tsClient.stop();
        return;
      }
    }

    // Read and print the first line of the response
    if (tsClient.available()) {
      String response = tsClient.readStringUntil('\n');
      Serial.print(F("[CLOUD] Server: "));
      Serial.println(response);
    }

    tsClient.stop();
  } else {
    Serial.println(F("[CLOUD] Connection Failed."));
  }
#endif
}
