# ESP32 Voice-Controlled Buggy with Micro-ROS and PlatformIO

This project enables voice-controlled buggy movement using an ESP32 microcontroller. The system listens for voice commands such as **"right"**, **"left"**, **"front"**, and **"back"**, sends corresponding HTTP requests to the ESP32, and provides both text and speech feedback. The ESP32 is configured with Micro-ROS firmware to handle GPIO pin control.

---

## Features

- **Voice Recognition**: Recognizes commands like **"right"**, **"left"**, **"front"**, and **"back"**, as well as exit commands such as **"quit"** or **"exit"**.
- **Signal Transmission**: Sends HTTP requests to the ESP32 to control GPIO pins for buggy movement.
- **Micro-ROS Firmware**: ESP32 firmware developed using Micro-ROS and PlatformIO for GPIO toggling.
- **Text-to-Speech Feedback**: Verbal acknowledgment of actions and errors.
- **Error Handling**: Graceful handling of network and voice recognition errors.

---

## Requirements

### Hardware
- ESP32 microcontroller with Wi-Fi.
- Buggy connected to GPIO pins for motor control.
- Microphone for speech input.

### Software
#### On Host PC:
- Python 3.x
- Required Python libraries:
  - `requests`
  - `speech_recognition`
  - `pyttsx3`

#### On ESP32:
- PlatformIO IDE.
- Micro-ROS library.

---

## Setup Instructions

### ESP32 Configuration (Micro-ROS and PlatformIO)

1. **Install PlatformIO**:
   - Install PlatformIO in VS Code or as a standalone IDE.
   - [PlatformIO Installation Guide](https://platformio.org/install).

2. **Create a Micro-ROS Project**:
   - Start a new PlatformIO project for the ESP32.
   - Add Micro-ROS dependencies to the `platformio.ini` file:
     ```ini
     [env:esp32]
     platform = espressif32
     board = esp32dev
     framework = arduino
     lib_deps =
         micro-ROS/micro_ros_arduino
     ```

3. **Write ESP32 Firmware**:
   - Program the ESP32 to host a web server that listens for HTTP requests to control GPIO pins based on voice commands.
   - Example ESP32 firmware:
     ```cpp
     #include <WiFi.h>
     #include <micro_ros_arduino.h>

     const char* ssid = "Your_SSID";
     const char* password = "Your_PASSWORD";

     WiFiServer server(80);

     void setup() {
         WiFi.begin(ssid, password);
         while (WiFi.status() != WL_CONNECTED) {
             delay(1000);
         }
         server.begin();
     }

     void loop() {
         WiFiClient client = server.available();
         if (client) {
             String request = client.readStringUntil('\r');
             if (request.indexOf("/GPIO=1") != -1) {  // Right
                 digitalWrite(1, HIGH);
             } else if (request.indexOf("/GPIO=2") != -1) {  // Left
                 digitalWrite(2, HIGH);
             } else if (request.indexOf("/GPIO=3") != -1) {  // Front
                 digitalWrite(3, HIGH);
             } else if (request.indexOf("/GPIO=4") != -1) {  // Back
                 digitalWrite(4, HIGH);
             }
             client.flush();
         }
     }
     ```

4. **Upload Firmware**:
   - Connect your ESP32 to the PC and upload the firmware using PlatformIO.

5. **Retrieve ESP32 IP Address**:
   - Find the IP address from the serial monitor or your router's admin page.

---

### Host PC Configuration

1. **Clone This Repository**:
   ```bash
   git clone https://github.com/Akhila-ch-40/Voice_command_Micro_ros.git
   cd Voice_command_Micro_ros
   ```

2. **Install Python Libraries**:
   ```bash
   pip install requests speechrecognition pyttsx3
   ```

3. **Run the Voice-Controlled Program**:
   - Update the ESP32 IP address in the Python script (`ESP32_IP` variable).
   - Run the program:
     ```bash
     python voice_control.py
     ```

---

## Usage

1. Run the Python program on your host PC.
2. Speak commands:
   - **"right"**: Sends signal `1` to the ESP32.
   - **"left"**: Sends signal `2` to the ESP32.
   - **"front"**: Sends signal `3` to the ESP32.
   - **"back"**: Sends signal `4` to the ESP32.
   - **"quit"** or **"exit"**: Stops the program.
3. Observe GPIO pin changes on the ESP32, which control the buggy's motors.

---

### Notes:
- Ensure the ESP32 is on the same network as the host PC.
- Verify correct GPIO connections to the buggy motors.
- Adjust the ESP32 firmware and Python script as necessary for custom signal mappings.
