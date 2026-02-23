# ESP32 + Bluepad32 Controller Receiver

ESP32 (D1 mini etc.) receives 8BitDo SN30 Pro / Micro via Bluetooth and outputs button, D-Pad, joystick values to serial.

## Features
- Bluetooth Classic HID support
- Buttons, D-Pad, triggers, joysticks
- Onboard LED lights when any input is detected

## Requirements
- ESP32 board (original ESP32 recommended)
- 8BitDo SN30 Pro or Micro Bluetooth controller
- Arduino IDE

## Setup Steps

1. **Add Boards Manager URLs**  
   File → Preferences → Additional Boards Manager URLs:  
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`  
   `https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json`

2. **Install Bluepad32**  
Tools → Board → Boards Manager → Search "esp32 bluepad32" → Install latest (4.1.0+)

3. **Select Board**  
Tools → Board → esp32_bluepad32 → WEMOS LOLIN32 (or ESP32 Dev Module)

4. **Upload**  
Connect ESP32 via USB → Select correct COM port → Sketch → Upload  
Open Serial Monitor (115200 baud)

5. **Pair Controller**  
- Android mode: Hold B + Start (or D switch on Micro)  
- Press PAIR button 3 sec → LED blinking  
- ESP32 will auto-connect

## Usage
- Any button/stick input → onboard LED ON  
- No input → LED OFF  
- Serial shows pressed/moved values

