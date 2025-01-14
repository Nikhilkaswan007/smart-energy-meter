#include "SmartMeter.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
PZEM004Tv30 pzem(&Serial2, 16, 17);
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;  


void setup() {
  Serial.begin(115200);
  setupOLED();
  setupWiFi();
  updateOLED();
  setupThingSpeak();
  
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(BUTTON_PIN_RESET, INPUT);
  pinMode(BUTTON_PIN_SWITCH, INPUT);

  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);

  dht.begin();
}
void readline(){
  tempVoltage = pzem.voltage();
  tempCurrent = pzem.current();
  tempPower = pzem.power();
  tempEnergy_kWh = pzem.energy();
  tempFrequency = pzem.frequency();
}

void loop() {
  if(millis() - lastUpdateOled >= updateOledInterval){
    lastUpdateOled = millis();
    readline();
    // Check for a sudden spike in voltage
    if (tempVoltage > 300) { // Voltage jumps from 0 to a large value
            Serial.println("Voltage spike detected, delaying reading.");
            delay(100);
            readline();
    } else {
        voltage = tempVoltage; // Update voltage only if no spike
        current = tempCurrent; 
        power = tempPower; 
        energy_kWh = tempEnergy_kWh; 
        frequency = tempFrequency;
    }
    
    updateOLED();
    sendErrorCodeToThingSpeak();
  }

  if(millis() - lastSwitchInterval >= switchInterval){
    lastSwitchInterval = millis();
    int remoteSwitchValue = ThingSpeak.readFloatField(myChannelId1, FIELD_SWITCH);
    Serial.print("Remote switch value: ");
    Serial.println(remoteSwitchValue);
    if (remoteSwitchValue != switchValue) {
      switchValue = remoteSwitchValue;
      Serial.print("Switch value updated from remote to: ");
      Serial.println(switchValue);
      handleSwitchControl(switchValue);  // Apply the remote switch value change
    }
  }

  if (millis() - lastUpdateMillis >= updateInterval) {
    lastUpdateMillis = millis();
    
    Serial.println("Updating data...");
    sendEnergyDataToThingSpeak();
  }



  // Check for local switch control button press
  if (digitalRead(BUTTON_PIN_SWITCH) == LOW) {
    switchValue = (switchValue % 3) + 1;  // Cycle through switch values locally
    Serial.print("Local switch control: ");
    Serial.println(switchValue);

    handleSwitchControl(switchValue);
    
    // Update ThingSpeak with the new local switch value
    ThingSpeak.setField(FIELD_SWITCH, switchValue);
    ThingSpeak.writeFields(myChannelId1, myWriteApiKey1);
  }
}

void setupWiFi() {
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display "Connecting to WiFi..." message
  display.setCursor(0, 0); 
  display.print("Connecting to WiFi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    // Keep trying to connect
    Serial.println("WiFi not connected");
    
    // Update OLED with status
    display.clearDisplay();
    display.setCursor(0, 0); 
    display.print("WiFi not connected");
    display.display();

    delay(1000); // Retry after 1 second
  }

  // When connected, update OLED and serial
  Serial.println("WiFi connected!");
  display.clearDisplay();
  display.setCursor(0, 0);
  delay(2000);
  display.print("WiFi connected!");
  display.display(); 
  delay(2000);
}


void setupThingSpeak() {
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak setup complete.");
}

void setupOLED() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  Serial.println("OLED setup complete.");
}

void updateOLED() {
  // Get latest readings

  Serial.print("Voltage: "); Serial.println(voltage);
  Serial.print("Current: "); Serial.println(current);
  Serial.print("Power: "); Serial.println(power);
  Serial.print("Energy (kWh): "); Serial.println(energy_kWh);
  Serial.print("Frequency: "); Serial.println(frequency);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0); display.print("Voltage: "); display.print(voltage); display.print(" V");
  display.setCursor(0, 20); display.print("Current: "); display.print(current); display.print(" A");
  display.setCursor(0, 30); display.print("Power: "); display.print(power); display.print(" W");
  display.setCursor(0, 40); display.print("Frequency: "); display.print(frequency); display.print(" Hz");
  display.setCursor(0, 50); display.print("Energy: "); display.print(energy_kWh); display.print(" kWh");

    display.setCursor(SCREEN_WIDTH - (statusText.length() * 6), 0); // Right-align the status
    display.print(statusText);


  display.display();
}

int findErrorCode() {
  if (voltage < MIN_VOLTAGE && current > MAX_CURRENT) {
    Serial.println("High short circuit detected.");
    return 9;  // High short circuit
  }
  if (voltage < MIN_VOLTAGE) {
    Serial.println("Low voltage detected.");
    return 2;  // Low voltage
  }
  if (current > MAX_CURRENT) {
    Serial.println("Overcurrent detected.");
    return 3;  // Overcurrent
  }
  if (isnan(voltage) || voltage ==0) {
    Serial.println("No power detected.");
    return 1;  // No power
  }
  return 0;  // No error
}

void handleSwitchControl(int value) {
  if (errorCode == 9) {
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, HIGH);
    Serial.println("Error 9: High short circuit, power off.");
      // Check for error reset button press
  if (digitalRead(BUTTON_PIN_RESET) == LOW && errorCode == 9) {
    errorCode = 0;  // Reset error code 9
    Serial.println("Error code 9 reset by button.");
  }
  }

    switch (value) {
        case 1:
            Serial.println("Activating Relay 1 (Main Line)");
            digitalWrite(relay2Pin, LOW);
            delay(1000);
            digitalWrite(relay1Pin, HIGH);
            digitalWrite(relay2Pin, HIGH);
            statusText = "M.L"; // Main Line
            break;
        case 2:
            Serial.println("Activating Relay 2 (Backup Line)");
            digitalWrite(relay2Pin, LOW);
            delay(1000);
            digitalWrite(relay1Pin, LOW);
            digitalWrite(relay2Pin, HIGH);
            statusText = "B.L"; // Backup Line
            break;
        case 3:
            Serial.println("Power off");
            digitalWrite(relay2Pin, LOW);
            statusText = "Off";
            break;
        default:
            Serial.println("Invalid switch value.");
            statusText = "INV";
            break;
    }

}

void sendEnergyDataToThingSpeak() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  Serial.print("Temperature: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);

  ThingSpeak.setField(FIELD_TEMP, temperature);
  ThingSpeak.setField(FIELD_HUMIDITY, humidity);

  Serial.print("Sending Voltage: "); Serial.println(voltage);
  Serial.print("Sending Current: "); Serial.println(current);
  Serial.print("Sending Power: "); Serial.println(power);
  Serial.print("Sending Energy: "); Serial.println(energy_kWh);
  //Serial.print("Sending Frequency: "); Serial.println(frequency);

  ThingSpeak.setField(FIELD_VOLTAGE, voltage);
  ThingSpeak.setField(FIELD_CURRENT, current);
  ThingSpeak.setField(FIELD_POWER, power);
  ThingSpeak.setField(FIELD_ENERGY, energy_kWh);
  //ThingSpeak.setField(FIELD_FREQUENCY, frequency);

  ThingSpeak.writeFields(myChannelId, myWriteApiKey);
}


static unsigned long lastRetryTime = 0;
  static int retryCount = 0;
  static bool isRetrying = false;
  const int maxRetries = 10;
  const unsigned long retryInterval = 1000; // 2 seconds
  static int writeResult = 0;


void sendErrorCodeToThingSpeak() {

  errorCode = findErrorCode();
  
  Serial.print("errorCode ");
  Serial.println(errorCode);

  if (errorCode != lastErrorCode) {
    // Start a new update process
    lastErrorCode = errorCode;
    Serial.print("Sending Error Code: ");
    Serial.println(errorCode);

    ThingSpeak.setField(FIELD_ERROR_CODE, errorCode);
    writeResult = ThingSpeak.writeFields(myChannelId1, myWriteApiKey1);

    if (writeResult != 200) {
      // If initial update fails, start retry process
      Serial.print("ThingSpeak error code update failed (code ");
      Serial.print(writeResult);
      Serial.println("). Starting retries...");
      isRetrying = true;
      lastRetryTime = millis();
      retryCount = 0;
    } else {
      Serial.println("ThingSpeak error code update successful.");
    }
  }

  if (isRetrying) {
    // Handle retries using millis
    if (millis() - lastRetryTime >= retryInterval) {
      lastRetryTime = millis();
      retryCount++;

      Serial.print("Retrying ThingSpeak error code update: Attempt ");
      Serial.println(retryCount);
      ThingSpeak.setField(FIELD_ERROR_CODE, errorCode);

      writeResult = ThingSpeak.writeFields(myChannelId1, myWriteApiKey1);
      if (writeResult == 200) {
        Serial.println("ThingSpeak error code update successful on retry.");
        isRetrying = false;
      } else if (retryCount >= maxRetries) {
        Serial.println("ThingSpeak error code update failed after maximum retries.");
        isRetrying = false;
      }
    }
  }
}
