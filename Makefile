# Makefile for Industrial Machine Monitoring System (IMSMS)

# Configuration
ARDUINO_CLI = "C:\Users\Manikanda Bharathi\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
BOARD = arduino:avr:uno
# ESP32_BOARD = esp32:esp32:esp32
UNO_BOARD = arduino:avr:uno
PORT = COM3 # Change this to your actual port

# Sketch paths
GATEWAY_SKETCH = gateway/gateway.ino
STATION_SKETCH = station/station.ino

# Standard targets
.PHONY: all gateway station uno-gateway uno-station clean help

all: gateway station

gateway:
	@echo "--- Compiling Gateway ---"
	@mkdir -p gateway/build
	$(ARDUINO_CLI) compile --fqbn $(BOARD) --build-path gateway/build $(GATEWAY_SKETCH)

station:
	@echo "--- Compiling Station ---"
	@mkdir -p station/build
	$(ARDUINO_CLI) compile --fqbn $(BOARD) --build-path station/build $(STATION_SKETCH)

uno-gateway:
	@echo "--- Compiling Gateway for Uno (Simulation) ---"
	@mkdir -p gateway/build
	$(ARDUINO_CLI) compile --fqbn $(UNO_BOARD) --build-path gateway/build $(GATEWAY_SKETCH)

uno-station:
	@echo "--- Compiling Station for Uno (Simulation) ---"
	@mkdir -p station/build
	$(ARDUINO_CLI) compile --fqbn $(UNO_BOARD) --build-path station/build $(STATION_SKETCH)

upload-gateway:
	@echo "--- Uploading Gateway ---"
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(BOARD) --build-path gateway/build $(GATEWAY_SKETCH)

upload-station:
	@echo "--- Uploading Station ---"
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(BOARD) --build-path station/build $(STATION_SKETCH)

clean:
	@echo "--- Cleaning build artifacts ---"
	rm -rf gateway/build station/build

help:
	@echo "Available targets:"
	@echo "  make gateway         - Compile the Gateway sketch"
	@echo "  make station         - Compile the Station sketch"
	@echo "  make all             - Compile both sketches"
	@echo "  make upload-gateway  - Compile and upload Gateway (set PORT first)"
	@echo "  make upload-station  - Compile and upload Station (set PORT first)"
	@echo "  make uno-gateway     - Compile Gateway for Arduino Uno (Simulation)"
	@echo "  make uno-station     - Compile Station for Arduino Uno (Simulation)"
	@echo "  make clean           - Remove build directories"
	@echo "  make help            - Show this help message"
