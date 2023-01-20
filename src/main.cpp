#include <main.h>

/*
      Smart ESP32 Tester

      → What does this code tests:

        ∘ Output pins;
        ∘ Input pins;
        ∘ Cores;
        ∘ Wi-Fi;
        ∘ EEPROM;
        ° Non Volatile Storage;
        ° SPIFFS.
  
  v2.3.8

----------------------- User Area ----------------------- */

// Wi-Fi
const char * ssid = "ssid";
const char * password = "password";

// Configuration
bool testOutput = 1;
bool testInput = 0;
bool testTask = 1;
bool testEEPROM = 1;
bool testSPIFFS = 1;
bool testNVS = 1;
bool testWiFi = 1; 

/* ------------------------------------------------------ */

Preferences preferences;

TaskHandle_t core0_Handle = NULL;
TaskHandle_t core1_Handle = NULL;

byte outputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
byte inputPins[] = {34,35,36,39};

unsigned long resetMillis = 0, inputTestTime = 0;
bool testFinished = 0, outputOK = 0, inputOK = 0, wifiOK = 0, spiffsOK = 0, nvsOK = 0, eepromOK = 0, core0 = 0, core1 = 0,
IO34 = 0, IO35 = 0, IO36 = 0, IO39 = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n-------- ESP32 Test Started! --------\n");

  // SPIFFS
  if (testSPIFFS) {
    Serial.println(" → SPIFFS test started!");
    if (!SPIFFS.begin(true))
      Serial.println("Mounting SPIFFS");

    File file = SPIFFS.open("/testSPIFFS.txt", FILE_WRITE);
    if (file.print(" SPIFFS OK!\n")) { 
      spiffsOK = true;
      Serial.println("File was written");
    } else {
      spiffsOK = false;
      Serial.println("File write failed");
    }
    file.close();

    if (spiffsOK) {
      file = SPIFFS.open("/testSPIFFS.txt");

      while (file.available()) {
        Serial.write(file.read());
        delay(1);
      }
      
      file.close();
    }
  } else
    Serial.println("SPIFFS test skipped.");

  // NVS
  if (testNVS) {
    Serial.println("\n → NVS test started!");
    preferences.begin("nvs-test", false);
    preferences.putInt("value", 1024);
    if (preferences.getInt("value") == 1024) {
      Serial.println(" NVS OK!");
      nvsOK = true;

      preferences.clear();
      preferences.end();
    } else
      Serial.println("** NVS NOT OK **");
  } else
    Serial.println("\nNVS test skipped.");

  // EEPROM
  if (testEEPROM) {
    Serial.println("\n →  EEPROM test started!");
    EEPROM.begin(EEPROM_SIZE);
    eepromOK = true;

    for (int i = 0; i < EEPROM_SIZE; i++) {
      EEPROM.writeBool(i, 1);
      delay(1);
    }
    EEPROM.commit();
    delay(100);

    for (int i = 0; i < EEPROM_SIZE; i++) {
      if (EEPROM.readBool(i) != 1)
        eepromOK = false;
      delay(1);
    }
    for (int i = 0; i < EEPROM_SIZE; i++) {
      EEPROM.writeBool(i, 0);
      delay(1);
    }
    EEPROM.commit();
    delay(100);
    
    eepromOK ? Serial.println(" EEPROM OK") : Serial.println(" EEPROM FAILED");
  } else 
    Serial.println("\nEEPROM test skipped.");
  
  // WIFI
  if (testWiFi) {
    Serial.print("\n → Wi-Fi test started!\nConnecting...");
    
    WiFi.begin(ssid, password);
    wifiOK = false;
    for (int tries = 1; tries < 10; tries++) {
      Serial.print(".");
      delay(1000);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" Connected!\n WIFI OK!");
        wifiOK = true;
        break;
      }
    }
    if (WiFi.status() != WL_CONNECTED) 
      Serial.println("Couldn't connect to wi-fi\n Verify if your wi-fi ssid and password is inserted correctly at \"User Area\"");
  } else 
    Serial.println("\nWi-Fi test skipped!");

  // TASKS
  if (testTask) {
    Serial.print("\n → Task test started!");
    xTaskCreatePinnedToCore(core_0, "core_0", 4096, NULL, 1, &core0_Handle, 0);
    delay(200);
    xTaskCreatePinnedToCore(core_1, "core_1", 4096, NULL, 1, &core1_Handle, 1);
    delay(200);
  } else
    Serial.println("\nTask test skipped!");
  
  // OUTPUT
  if (testOutput) {
    Serial.println("\n → Output pins test started!");
    outputOK = true;
    for (int i = 0; i < 19; i++) {
      pinMode(outputPins[i], OUTPUT);
      delay(1);
      digitalWrite(outputPins[i], HIGH);
      delay(10);
      if (digitalRead(outputPins[i]))
        Serial.printf(" OUTPUT PIN %d: OK!\n", outputPins[i]);
      else {
        Serial.printf(" OUTPUT PIN %d: *NOT OK*\n", outputPins[i]);
        outputOK = false;
      }
      digitalWrite(outputPins[i], LOW);
    }
  } else 
    Serial.println("\nOutput test skipped!");
  
  // INPUT
  if (testInput) {
    Serial.println("\n → Input pins test started!");
    
    for (int i = 0; i < 4; i++) {
      pinMode(inputPins[i], INPUT);
      delay(1);
    }

    inputTestTime = millis();
  } else
    Serial.println("\nInput test skipped!");
}

void loop() {
  if (testInput) {
    if (!IO34 && digitalRead(34)) {
      Serial.println(" INPUT PIN 34: OK!");
      IO34 = true;
    }if (!IO35 && digitalRead(35)) {
      Serial.println(" INPUT PIN 35: OK!");
      IO35 = true;
    }if (!IO36 && digitalRead(36)) { 
      Serial.println(" INPUT PIN 36: OK!");
      IO36 = true;
    }if (!IO39 && digitalRead(39)) {
      Serial.println(" INPUT PIN 39: OK!");
      IO39 = true;
    }if (IO34 && IO35 && IO36 && IO39)
      inputOK = true;
  }
  
  // Retry WiFi Connection
  if ((testWiFi && !wifiOK) && (!testInput || inputOK)) {
    Serial.println("\n Retrying to establish connection with the Wi-Fi");
    WiFi.begin(ssid, password);
    while (millis() - inputTestTime <= TIME_TO_GET_RESULTS) {
      Serial.print(".");
      delay(3000);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" Connected!\n WIFI OK!");
        wifiOK = true;
        break;
      }
    }
  }

  // Test Results
  testFinished = ((wifiOK || !testWiFi) && ((core0 && core1) || !testTask) && (inputOK || !testInput));
  if ((millis() - inputTestTime >= TIME_TO_GET_RESULTS || testFinished)) {
    Serial.println("\n\n----------- Test Results ------------");
    
    testOutput ? Serial.printf("\n Output Pins: %s.", outputOK ? "OK" : "One or more pins didn't work") 
      : Serial.print("\n Output Pins: Test skipped.");
    
    if (testInput) {
      Serial.printf("\n Input Pins: %s.", inputOK ? "OK" : "One or more pins didn't work");
      if (!inputOK) {
        Serial.print("\nInput Pins that aren't working: ");
        IO34 ? Serial.print("") : Serial.print("IO34, ");
        IO35 ? Serial.print("") : Serial.print("IO35, ");
        IO36 ? Serial.print("") : Serial.print("IO36, "); 
        IO39 ? Serial.print("") : Serial.print("IO39. ");
        Serial.println("");
      }
    } else
      Serial.print("\n Input Pins: Test skipped.");
    
    testEEPROM ? Serial.printf("\n EEPROM: %s.", eepromOK ? "OK" : "NOT OK") : Serial.print("\n EEPROM: Test skipped.");
    testSPIFFS ? Serial.printf("\n SPIFFS: %s.", spiffsOK ? "OK" : "NOT OK") : Serial.print("\n SPIFFS: Test skipped.");
    testNVS ? Serial.printf("\n NVS: %s.", nvsOK ? "OK" : "NOT OK") : Serial.print("\n NVS: Test skipped.");
    testWiFi ? Serial.printf("\n Wi-Fi: %s.", wifiOK ? "OK" : "NOT OK") : Serial.print("\n Wi-Fi: Test skipped.");

    if (testTask) {
      Serial.printf("\n Core 0: %s.", core0 ? "OK" : "NOT OK");
      Serial.printf("\n Core 1: %s.", core1 ? "OK" : "NOT OK");
    } else
      Serial.print("\n Tasks: Test skipped.");

    Serial.println("\n\n-------------------------------------");
    if (testFinished) {
      Serial.println("Test finished!");

      if (int(millis()/1000) == 1)
        Serial.println("Test duration: 1 second.");
      else
        Serial.printf("Test duration: %d seconds.\n", int(millis()/1000));

      vTaskDelete(NULL);
    }
    inputTestTime = millis();
  }
  delay(10);
}

void core_0(void * pvParameters) {
  while (1) {
    core0 = true;
    Serial.println("\n Core 0 OK!");
    vTaskDelete(NULL);
  }
}

void core_1(void * pvParameters) {
  while (1) {
    core1 = true;
    Serial.println(" Core 1 OK!");
    vTaskDelete(NULL);
  }
}
