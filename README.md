# Industrial Machine State Monitoring System (IMSMS)

A real-time monitoring system designed for industrial environments to track machine parameters (Temperature, Humidity, Current, and Vibration) and report them directly to the **ThingSpeak Cloud**. **Now with fully automated setup via Makefile!**

## 🚀 Accomplishments & Features

- **Exclusive Ethernet Support**: Migrated from Wi-Fi to a pure Ethernet implementation using the **W5100/W5500** shield for stable industrial/simulated connectivity.
- **Direct Cloud Integration**: Refactored the Station node to report telemetry directly to ThingSpeak, reducing dependency on a local gateway.
- **Autonomous Networking**: Integrated **DHCP** for automatic IP configuration with **Static IP/DNS** fallbacks for reliable internet communication.
- **System Observability**:
    - **Heartbeat LED**: Onboard Pin 13 double-blinks every 15 seconds to confirm CPU health.
    - **Startup Header**: Bold project initialization message in Serial Terminal.
    - **Verbose Debugging**: Step-by-step logging of network and sensor initialization.
- **Non-Blocking I/O & Latching**:
    - Replaced `delay()` with `millis()` for zero-lag sensor polling.
    - **Vibration Latch**: Remembers button presses even between cloud updates, ensuring no event is missed.

## 🛠 Challenges Faced & Solutions

| Issue | Root Cause | Solution |
| :--- | :--- | :--- |
| **DHT11 (Data)** | **2** | `DHT_PIN` |
| **Zero Cloud Entries** | Request sent but rejected by server (Malformed HTTP). | Refactored `postToThingSpeak` for strict **HTTP CRLF** compliance and verified server responses. |
| **Blank Serial Terminal** | Baud rate mismatch (115200 was too slow for some virtual terminals). | Reverted to **9600 baud** for maximum compatibility. |
| **Ethernet IP: 0.0.0.0** | SPI Chip Select (CS) pin was not explicitly initialized. | Added `Ethernet.init(10)` and verified SCS wiring on Pin 10. |
| **Cloud Connection Failed** | No Gateway/DNS info in Static IP; SIM was not bridged to internet. | Implemented **DHCP** and configured Bridge Mode in PicSimLab Spare Parts. |
| **Missing Libraries** | `Ethernet.h` not in the host environment. | Automated library installation using `arduino-cli`. |
| **ThingSpeak Blocking** | Updates were sent too fast (1s delay). | Enforced a strict **15-second delay** to match ThingSpeak's rate limit. |
| **Struct Mismatch** | `imsms_types.h` had wrong fields. | Updated `machine_data_t` to include `humidity` and `current`. |
| **Gateway Build Errors** | Used ESP32-specific `Serial.printf` on AVR. | Refactored code to standard `Serial.print` for cross-platform compatibility. |

## ⚙️ Prerequisites & Setup

To run this project, you need the **Arduino CLI** installed on your system.

### 1. Install Arduino CLI
-   **Download**: Get the latest binary from the [Arduino CLI Releases page](https://github.com/arduino/arduino-cli/releases).
-   **Extract**: Place the `arduino-cli.exe` (Windows) or `arduino-cli` (Linux/Mac) in a folder on your computer.

### 2. Configure Makefile
The [**Makefile**](file:///c:/Users/Manikanda%20Bharathi/Desktop/IMSMS/Makefile) is **cross-platform** (Windows, Linux, Mac).
-   If `arduino-cli` is in your system **PATH**, no changes are needed!
-   If not, update the `LOCAL_CLI_PATH` in the Makefile to match your installation.

## 🕹️ How to Run in PicSimLab

1.  **Preparation**: (Optional) Update the ThingSpeak API Key in [imsms_types.h](include/imsms_types.h) if you wish to use your own channel. Otherwise, telemetry will be sent to the default public channel listed in the [Monitoring](#monitoring) section.
2.  **Build**: Run `make uno-station` (or `make uno-gateway`).
3.  **Simulation Configuration**:
    - **Board**: Arduino Uno.
    - **Frequency**: **16 MHz** (Critical for DHT sensor timing).
    - **Shield**: **Eth W5500** (Spare Part).
    - **SCS Pin**: Connect to Pin 10.
    - **Current Sensor**: Connect a **Potentiometer** to **Analog Pin A0**.
    - **Vibration Sensor**: Connect a **Switch** or **Push Button** to **Digital Pin 4**.
    - **DHT11 (Data)**: Connect to **Digital Pin 8**.
    - **Network**: Bridge the Spare Part to your host internet adapter.
4.  **Loading the File**:
    - In PicSimLab: `File` -> `Load Hex`.
    - Navigate to `station/build/` and select `station.ino.hex`.
5.  **Terminal**: Open IO Virtual Terminal at **9600 baud**.
6.  **Save Workspace (Optional)**: After configuring all Spare Parts, go to `File` -> `Save Workspace` to create a `.pbi` file for quick loading next time.

## 📊 Monitoring
View live charts on your [ThingSpeak Channel](https://thingspeak.mathworks.com/channels/3266149/).
