# Industrial Machine State Monitoring System (IMSMS)

A real-time monitoring system designed for industrial environments to track machine parameters (Temperature, Humidity, Current, and Vibration) and report them directly to the **ThingSpeak Cloud**.

## 🚀 Accomplishments & Features

- **Exclusive Ethernet Support**: Migrated from Wi-Fi to a pure Ethernet implementation using the **W5100/W5500** shield for stable industrial/simulated connectivity.
- **Direct Cloud Integration**: Refactored the Station node to report telemetry directly to ThingSpeak, reducing dependency on a local gateway.
- **Autonomous Networking**: Integrated **DHCP** for automatic IP configuration with **Static IP/DNS** fallbacks for reliable internet communication.
- **System Observability**:
    - **Heartbeat LED**: Onboard Pin 13 double-blinks every 15 seconds to confirm CPU health.
    - **Startup Header**: Bold project initialization message in Serial Terminal.
    - **Verbose Debugging**: Step-by-step logging of network and sensor initialization.

## 🛠 Challenges Faced & Solutions

| Issue | Root Cause | Solution |
| :--- | :--- | :--- |
| **Blank Serial Terminal** | Baud rate mismatch (115200 was too fast for some virtual terminals). | Reverted to **9600 baud** for maximum compatibility. |
| **Ethernet IP: 0.0.0.0** | SPI Chip Select (CS) pin was not explicitly initialized. | Added `Ethernet.init(10)` and verified SCS wiring on Pin 10. |
| **Cloud Connection Failed** | No Gateway/DNS info in Static IP; SIM was not bridged to internet. | Implemented **DHCP** and configured Bridge Mode in PicSimLab Spare Parts. |
| **Missing Libraries** | `Ethernet.h` not in the host environment. | Automated library installation using `arduino-cli`. |
| **ThingSpeak Blocking** | Updates were sent too fast (1s delay). | Enforced a strict **15-second delay** to match ThingSpeak's rate limit. |
| **Gateway Build Errors** | Used ESP32-specific `Serial.printf` on AVR. | Refactored code to standard `Serial.print` for cross-platform compatibility. |

## 🕹 How to Run in PicSimLab

1.  **Preparation**: Add your ThingSpeak API Key in `imsms_types.h`.
2.  **Build**: Run `make uno-station` (or `make uno-gateway`).
3.  **Simulation Configuration**:
    - **Board**: Arduino Uno.
    - **Shield**: **Eth W5500** (Spare Part).
    - **SCS Pin**: Connect to Pin 10.
    - **Network**: Bridge the Spare Part to your host internet adapter.
4.  **Terminal**: Open IO Virtual Terminal at **9600 baud**.

## 📊 Monitoring
View live charts on your [ThingSpeak Channel](https://thingspeak.com/channels/3266149).
