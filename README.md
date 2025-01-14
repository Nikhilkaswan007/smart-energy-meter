

```markdown
# Smart Energy Meter

The **Smart Energy Meter** is an IoT-based project designed to monitor and manage energy consumption efficiently.
It provides real-time data on voltage, current, power, and energy usage while allowing remote and local control of power lines.
The system integrates with ThingSpeak for cloud-based monitoring and visualization.

---

## Folder Structure

```plaintext
Smart-Energy-Meter/
├── Doc/                  # Documentation and guides
├── code/SmartMeter/      # Source code for the smart energy meter
├── LICENSE               # License for the project
├── README.md             # Project overview and instructions
```

---

## Features

- **Real-Time Monitoring**: Measures voltage, current, power, and energy consumption.
- **Error Detection**: Identifies issues like low voltage, overcurrent, or power failure.
- **Remote Control**: Allows switching between power lines via ThingSpeak.
- **Local Control**: Physical buttons for manual control.
- **OLED Display**: Displays real-time data for on-site monitoring.
- **WiFi-Enabled**: Sends data to the cloud for remote access.

---

## Requirements

### Hardware:
1. ESP32 microcontroller
2. PZEM004T energy monitoring module
3. DHT11 temperature and humidity sensor
4. Adafruit SSD1306 OLED display
5. Two relays for line switching
6. Push buttons for local control

### Software:
1. Arduino IDE
2. Required Libraries:
   - `WiFi.h`
   - `ThingSpeak.h`
   - `Wire.h`
   - `DHT.h`
   - `Adafruit_GFX.h`
   - `Adafruit_SSD1306.h`

---

## Setup Instructions

### Step 1: Clone the Repository
```bash
git clone https://github.com/your-username/smart-energy-meter.git
cd smart-energy-meter
```

### Step 2: Open the Code
- Navigate to `code/SmartMeter/` and open the `.ino` file in the Arduino IDE.

### Step 3: Install Libraries
Install the following libraries via Arduino IDE's Library Manager:
- ThingSpeak
- Adafruit SSD1306
- Adafruit GFX
- DHT Sensor

### Step 4: Configure WiFi and ThingSpeak
- Update your WiFi credentials in the code:
  ```cpp
  const char* WIFI_NAME = "YourWiFiSSID";
  const char* WIFI_PASSWORD = "YourWiFiPassword";
  ```
- Add your ThingSpeak Channel ID and API Keys:
  ```cpp
  const int myChannelId = YOUR_CHANNEL_ID;
  const char* myWriteApiKey = "YOUR_API_KEY";
  ```

### Step 5: Upload the Code
1. Connect your ESP32 to your computer.
2. Select the correct board and COM port in Arduino IDE.
3. Upload the code.

### Step 6: Connect the Hardware
- Connect the PZEM004T, DHT11, OLED, and relays as per the code's pin definitions.

### Step 7: Power On
- Turn on the system and monitor data on the OLED display.
- View real-time data on ThingSpeak.

---

## Usage

1. **Switch Control**:
   - Remote: Use ThingSpeak to change the switch field value.
   - Local: Use the button to toggle between Main Line, Backup Line, or Power Off.

2. **Monitor Data**:
   - Check the OLED for real-time readings.
   - View historical data and charts on ThingSpeak.

---

## Contributing

We welcome contributions! Please submit issues or pull requests to help improve the project.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
```

Replace `your-username` with your GitHub username in the clone instructions, and update `YOUR_CHANNEL_ID` and `YOUR_API_KEY` with the actual credentials.
