# Makefile for Industrial Machine Monitoring System (IMSMS)

# Configuration
# If arduino-cli is in your PATH, it will be used. 
# Otherwise, it falls back to the hardcoded path.
ARDUINO_CLI ?= arduino-cli
LOCAL_CLI_PATH = "C:\Users\Manikanda Bharathi\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

# If the command in PATH fails, use the local path
ifeq ($(shell $(ARDUINO_CLI) version > /dev/null 2>&1 && echo ok),)
    ARDUINO_CLI = $(LOCAL_CLI_PATH)
endif

BOARD = arduino:avr:uno
UNO_BOARD = arduino:avr:uno
PORT = COM3 

# Sketch paths
GATEWAY_SKETCH = gateway/gateway.ino
STATION_SKETCH = station/station.ino

# Libraries and Cores
CORE = arduino:avr
LIBS = "Ethernet" "DHT sensor library" "Adafruit Unified Sensor"

# Standard targets
.PHONY: all gateway station uno-gateway uno-station clean help setup

all: setup gateway station

setup:
	@if ! $(ARDUINO_CLI) version > /dev/null 2>&1; then \
		echo "ERROR: arduino-cli not found in PATH or at specified LOCAL_CLI_PATH."; \
		echo "Please install it or update the path in the Makefile."; \
		exit 1; \
	fi
	@echo "--- Initializing Arduino Environment ---"
	@$(ARDUINO_CLI) core update-index
	@$(ARDUINO_CLI) core install $(CORE)
	@$(ARDUINO_CLI) lib install $(LIBS)
	@echo "--- Setup Complete ---"

gateway: 
	@echo "--- Compiling Gateway ---"
	@mkdir -p gateway/build
	$(ARDUINO_CLI) compile --fqbn $(BOARD) --build-property "compiler.cpp.extra_flags=\"-I$(CURDIR)/include\"" --build-path gateway/build $(GATEWAY_SKETCH)

station: 
	@echo "--- Compiling Station ---"
	@mkdir -p station/build
	$(ARDUINO_CLI) compile --fqbn $(BOARD) --build-property "compiler.cpp.extra_flags=\"-I$(CURDIR)/include\"" --build-path station/build $(STATION_SKETCH)

uno-gateway: 
	@echo "--- Compiling Gateway for Uno (Simulation) ---"
	@mkdir -p gateway/build
	$(ARDUINO_CLI) compile --fqbn $(UNO_BOARD) --build-property "compiler.cpp.extra_flags=\"-I$(CURDIR)/include\"" --build-path gateway/build $(GATEWAY_SKETCH)

uno-station: 
	@echo "--- Compiling Station for Uno (Simulation) ---"
	@mkdir -p station/build
	$(ARDUINO_CLI) compile --fqbn $(UNO_BOARD) --build-property "compiler.cpp.extra_flags=\"-I$(CURDIR)/include\"" --build-path station/build $(STATION_SKETCH)

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
