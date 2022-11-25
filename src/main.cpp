#include <main.h>

/*
      Smart ESP32 Tester

      → What does this code tests:

        ∘ Output pins;
        ∘ Input pins;
        ∘ Cores;
        ∘ Wi-Fi;
        ∘ EEPROM.
  
  v2.0.0

----------------------- User Area ----------------------- */

// WiFi
const char * ssid = "SmartLy-2.4GHz";
const char * password = "Sm4r7ly.iot";

// Configuration
bool testOutput = true;
bool testInput = true;
bool testTask = true;
bool testEEPROM = true;
bool testWifi = true
; 

/* ------------------------------------------------------ */

TaskHandle_t core0_Handle = NULL;
TaskHandle_t core1_Handle = NULL;

byte outputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
byte inputPins[] = {34,35,36,39};

unsigned long resetMillis = 0, inputTestTime = 0;
bool taskOK = 0, outputOK = 0, inputOK = 0, wifiOK = 0, eepromOK = 0, core0 = 0, core1 = 0,
IO34 = 0, IO35 = 0, IO36 = 0, IO39 = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n-------- ESP32 Test Started! --------");

  // EEPROM
  if(testEEPROM) {
    Serial.println("Starting EEPROM test.\n");
    EEPROM.begin(EEPROM_SIZE);
    eepromOK = true;

    for(int i = 0; i < 100; i++) {
      EEPROM.writeBool(i, 1);
      delay(1);
    }
    EEPROM.commit();
    delay(250);

    for(int i = 0; i < 100; i++) {
      if(EEPROM.readBool(i) != 1)
        eepromOK = false;
      delay(1);
    }

    for(int i = 0; i < 100; i++) {
      EEPROM.writeBool(i, 0);
      delay(1);
    }
    EEPROM.commit();
    delay(250);
    
    eepromOK ? Serial.println("\n --- EEPROM OK ---") : Serial.println("\n --- EEPROM FAILED ---");
  } else {
    Serial.println("EEPROM test skipped.");
    eepromOK = true;
  }
  
  // WIFI
  if(testWifi) {
    Serial.println("\n → Initializing wifi Test!\n\nConnecting...");
    
    WiFi.begin(ssid, password);
    wifiOK = true;
    for(int tries = 1; tries < 5; tries++) {
      Serial.printf("\n Tries: %d\n", tries);
      WiFi.disconnect();
      WiFi.begin();
      delay(3000);
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\n --- WIFI OK! ---");
        break;
      }
    }

    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Check if ssid and password are correct and try again.\n");
      wifiOK = false;
    }
  } else {
    Serial.println("WiFi Test skipped!");
    wifiOK = true;
  }

  // OUTPUT
  if(testOutput) {
    Serial.println("\n → Starting output pins test!");
    outputOK = true;
    for(int i = 0; i < 19; i++) {
      pinMode(outputPins[i], OUTPUT);
      delay(10);
      digitalWrite(outputPins[i], HIGH);
      delay(10);
      if(digitalRead(outputPins[i]))
        Serial.printf("\n → OUTPUT PIN %d: OK!", outputPins[i]);
      else {
        Serial.printf("\n → OUTPUT PIN %d: *NOT OK*", outputPins[i]);
        outputOK = false;
      }
      digitalWrite(outputPins[i], LOW);
      delay(1);
    }
  } else {
    Serial.println("Output test skipped!");
    outputOK = true;
  }

  // TASKS
  if(testTask) {
    xTaskCreatePinnedToCore(core_0, "core_0", 4096, NULL, 1, &core0_Handle, 0);
    delay(100);
    xTaskCreatePinnedToCore(core_1, "core_1", 4096, NULL, 1, &core1_Handle, 1);
    delay(100);
  } else {
    Serial.println("Task test skipped!");
    taskOK = true;
  }
  
  // INPUT
  if(testInput) {
    Serial.println("\n\n → Starting input pins test");
    
    for(int i = 0; i < 4; i++) {
      pinMode(inputPins[i], INPUT);
      delay(10);
    }
  
    Serial.println("");
    inputTestTime = millis();
  } else {
    Serial.println("Input test skipped!");
    inputOK = true;
  }
}

void loop() {
  if(testInput) {
    if (!IO34 && digitalRead(34)) {
      Serial.println("INPUT PIN 34: OK!");
      IO34 = true;
    }if (!IO35 && digitalRead(35)) {
      Serial.println("INPUT PIN 35: OK!");
      IO35 = true;
    }if (!IO36 && digitalRead(36)) { 
      Serial.println("INPUT PIN 36: OK!");
      IO36 = true;
    }if (!IO39 && digitalRead(39)) {
      Serial.println("INPUT PIN 39: OK!");
      IO39 = true;
    }

    if(IO34 && IO35 && IO36 && IO39)
      inputOK = true;
  }

  bool testFinished = (wifiOK && taskOK && outputOK && inputOK) ? true : false;
  if((millis() - inputTestTime >= TIME_TO_GET_RESULTS || testFinished)) {
    Serial.println("\n----------- Test Results ------------\n");
    
    testOutput ? Serial.printf("\n Output Pins: %s.\n", outputOK ? "OK" : "One or more pins didn't work") 
      : Serial.println("\n Output Pins: Test skipped.");
    
    if(testInput) {
      Serial.printf("\n Input Pins: %s.", inputOK ? "OK" : "One or more pins didn't work");
      if(!inputOK){
        Serial.print("\nInput Pins that aren't working: ");
        IO34 ? Serial.print("") : Serial.print("IO34, ");
        IO35 ? Serial.print("") : Serial.print("IO35, ");
        IO36 ? Serial.print("") : Serial.print("IO36, "); 
        IO39 ? Serial.print("") : Serial.print("IO39. ");
      }
      Serial.println("");
    } else
      Serial.println("\n Input Pins: Test skipped.\n");
    
    testEEPROM ? Serial.printf("\n EEPROM: %s.\n", eepromOK ? "OK" : "NOT OK") : Serial.println("\n EEPROM: Test skipped.");
    testWifi ? Serial.printf("\n WiFi: %s.\n", wifiOK ? "OK" : "NOT OK") : Serial.println("\n WiFi: Test skipped.");

    if(testTask) {
      Serial.printf("\n Core 0: %s.", core0 ? "OK" : "NOT OK");
      Serial.printf("\n Core 1: %s.\n", core1 ? "OK" : "NOT OK");
    } else
      Serial.println("\n Tasks: Test skipped.");

    Serial.println("\n-------------------------------------");

    if(testFinished) {
      Serial.println("Test finished!");
      vTaskDelete(NULL);
    }

    inputTestTime = millis();
  }

  delay(10);
}

void core_0(void * pvParameters) {
  while(1) {
    if(!core0) {
      core0 = true;
      Serial.println("\n\n --- Core 0 OK! ---\n");
    }

    if(taskOK) {
      vTaskDelete(core1_Handle);
      vTaskDelete(core0_Handle);
    }

    delay(100); 
  }
}

void core_1(void * pvParameters) {
  while(1) {
    if(!core1) {
      core1 = true;
      Serial.println("\n --- Core 1 OK! ---\n");
    }

    if(!taskOK && core0 && core1) {
      taskOK = true;
      Serial.println("Tasks OK!");
    }

    delay(100); 
  }
}