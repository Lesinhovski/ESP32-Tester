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
  
  v2.3.3

----------------------- User Area ----------------------- */

// Wi-Fi
const char * ssid = "Wi-Fi Name";
const char * password = "Password";

// Configuration
bool testOutput = 1;
bool testInput = 0;
bool testTask = 1;
bool testEEPROM = 1;
bool testSPIFFS = 1;
bool testNVS = 1;
bool testWifi = 0; 

/* ------------------------------------------------------ */

Preferences preferences;

TaskHandle_t core0_Handle = NULL;
TaskHandle_t core1_Handle = NULL;

byte outputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
byte inputPins[] = {34,35,36,39};

unsigned long resetMillis = 0, inputTestTime = 0;
bool testFinished = 0, taskOK = 0, outputOK = 0, inputOK = 0, wifiOK = 0, spiffsOK = 0, nvsOK = 0, eepromOK = 0, core0 = 0, core1 = 0,
IO34 = 0, IO35 = 0, IO36 = 0, IO39 = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n-------- ESP32 Test Started! --------");

  // SPIFFS
  if (testSPIFFS) {
    Serial.println(" → SPIFFS test starting!");
    if (!SPIFFS.begin(true))
      Serial.println("Mounting SPIFFS");

    File file = SPIFFS.open("/testSPIFFS.txt", FILE_WRITE);
    if (file.print("SPIFFS OK!\n")) { 
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
    Serial.println("\n → NVS test starting!");
    preferences.begin("nvs-test", false);
    delay(10);

    preferences.putInt("value", 1024);
    delay(10);

    if (preferences.getInt("value") == 1024) {
      Serial.println("NVS OK!");
      nvsOK = true;

      preferences.clear();
      preferences.end();
    } else
      Serial.println("** NVS Not OK **");
  } else
    Serial.println("NVS test skipped.");

  // EEPROM
  if (testEEPROM) {
    Serial.println("\n →  EEPROM test starting!");
    EEPROM.begin(EEPROM_SIZE);
    eepromOK = true;

    for (int i = 0; i < 100; i++) {
      EEPROM.writeBool(i, 1);
      delay(1);
    }
    EEPROM.commit();
    delay(100);

    for (int i = 0; i < 100; i++) {
      if (EEPROM.readBool(i) != 1)
        eepromOK = false;
      delay(1);
    }

    for (int i = 0; i < 100; i++) {
      EEPROM.writeBool(i, 0);
      delay(1);
    }
    EEPROM.commit();
    delay(100);
    
    eepromOK ? Serial.println("EEPROM OK\n") : Serial.println("EEPROM FAILED\n");
  } else 
    Serial.println("EEPROM test skipped.");
  
  // WIFI
  if (testWifi) {
    Serial.println("\n → Wi-Fi test starting!\n\nConnecting...");
    
    WiFi.begin(ssid, password);
    wifiOK = false;
    for (int tries = 1; tries < 5; tries++) {
      Serial.printf("\n Tries: %d\n", tries);
      WiFi.disconnect();
      WiFi.begin();
      delay(3000);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n --- WIFI OK! ---");
        wifiOK = true;
        break;
      }
    }
  } else 
    Serial.println("Wi-Fi test skipped!");

  // OUTPUT
  if (testOutput) {
    Serial.println("\n → Starting output pins test!");
    outputOK = true;
    for (int i = 0; i < 19; i++) {
      pinMode(outputPins[i], OUTPUT);
      delay(10);
      digitalWrite(outputPins[i], HIGH);
      delay(10);
      if (digitalRead(outputPins[i]))
        Serial.printf("\n OUTPUT PIN %d: OK!", outputPins[i]);
      else {
        Serial.printf("\n OUTPUT PIN %d: *NOT OK*", outputPins[i]);
        outputOK = false;
      }
      digitalWrite(outputPins[i], LOW);
    }
  } else 
    Serial.println("Output test skipped!");

  // TASKS
  if (testTask) {
    xTaskCreatePinnedToCore(core_0, "core_0", 4096, NULL, 1, &core0_Handle, 0);
    delay(100);
    xTaskCreatePinnedToCore(core_1, "core_1", 4096, NULL, 1, &core1_Handle, 1);
    delay(100);
  } else
    Serial.println("Task test skipped!");
  
  // INPUT
  if (testInput) {
    Serial.println("\n\n → Starting input pins test");
    
    for (int i = 0; i < 4; i++) {
      pinMode(inputPins[i], INPUT);
      delay(10);
    }
  
    Serial.println("");
    inputTestTime = millis();
  } else
    Serial.println("Input test skipped!");
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
    }

    if (IO34 && IO35 && IO36 && IO39)
      inputOK = true;
  }
  
  if ((testWifi && !wifiOK) && (!testInput || inputOK)) {
    Serial.println("Retrying to establish connection with the Wi-Fi");
    WiFi.begin(ssid, password);
    while (millis() - inputTestTime <= TIME_TO_GET_RESULTS) {
      Serial.print(".");
      delay(3000);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("--- WIFI OK! ---");
        wifiOK = true;
        break;
      }
    }
  }

  // Results
  testFinished = ((wifiOK || !testWifi) && (taskOK || !testTask) && (inputOK || !testInput)) ? true : false;
  if ((millis() - inputTestTime >= TIME_TO_GET_RESULTS || testFinished)) {
    Serial.println("\n----------- Test Results ------------");
    
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
    testWifi ? Serial.printf("\n WiFi: %s.", wifiOK ? "OK" : "NOT OK") : Serial.print("\n WiFi: Test skipped.");

    if (testTask) {
      Serial.printf("\n Core 0: %s.", core0 ? "OK" : "NOT OK");
      Serial.printf("\n Core 1: %s.\n", core1 ? "OK" : "NOT OK");
    } else
      Serial.print("\n Tasks: Test skipped.");

    Serial.println("\n-------------------------------------");
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
    if (!core0) {
      core0 = true;
      Serial.println("\n\n --- Core 0 OK! ---\n");
    }

    if (taskOK) {
      vTaskDelete(core1_Handle);
      vTaskDelete(core0_Handle);
    }

    delay(100); 
  }
}

void core_1(void * pvParameters) {
  while (1) {
    if (!core1) {
      core1 = true;
      Serial.println("\n --- Core 1 OK! ---\n");
    }

    if (!taskOK && core0 && core1) {
      taskOK = true;
      Serial.println("Tasks OK!");
    }

    delay(100); 
  }
}
