#include <main.h>

/*
      Smart ESP32 Tester

      → What does this code tests:

        ∘ ESP WiFi;
        ∘ Cores and Tasks;
        ∘ Output pins;
        ∘ Input pins;
  
  v1.4.0


----------------------- User Area ----------------------- */

// Wifi
const char * ssid = "";
const char * password = "";

// Configuration
bool testWifi = 0; 
bool testInput = 0;

/* ------------------------------------------------------ */

byte inputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33,34,35,36,39};
byte outputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};

TaskHandle_t core0_Handle = NULL;
TaskHandle_t core1_Handle = NULL;

unsigned long resetMillis = 0, inputTestTime = 0;
bool testCompleted, taskTested = 0, core0 = 0, core1 = 0,
IO2 = 0, IO16 = 0, IO25 = 0, IO36 = 0,
IO4 = 0, IO17 = 0, IO26 = 0, IO39 = 0,
IO5 = 0, IO18 = 0, IO27 = 0,
IO12 = 0,IO19 = 0, IO32 = 0,
IO13 = 0,IO21 = 0, IO33 = 0,
IO14 = 0,IO22 = 0, IO34 = 0,
IO15 = 0,IO23 = 0, IO35 = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("\n------------ ESP32 Test Started! ------------");
    
    // WIFI TEST
    if(ssid == ""){
      Serial.println("Check your wifi ssid");
      testWifi = false;      
    } if(testWifi) {
      Serial.println("\n → Initializing wifi Test!\n\nConnecting...");
      WiFi.begin(ssid, password);

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

      if(WiFi.status() != WL_CONNECTED)
        Serial.println("\n ***** Could not connect to WiFi *****\n");
    }else
      Serial.println("\n* Wifi Test Skipped! *");

    // Output pins
    Serial.println("\n → Starting output pins test!");
    for(int i = 0; i < 19; i++) {
      pinMode(outputPins[i], OUTPUT);
      digitalWrite(outputPins[i], HIGH);
      delay(50);
      if(digitalRead(outputPins[i]))
        Serial.printf("\n → OUTPUT PIN %d: OK!", outputPins[i]);
      else
        Serial.printf("\n → OUTPUT PIN %d: *NOT OK*", outputPins[i]);
      digitalWrite(outputPins[i], LOW);
      delay(50);
    }

    // Create Tasks
    xTaskCreatePinnedToCore(core_0, "core_0", 4096, NULL, 1, &core0_Handle, 0);
    delay(100);
    xTaskCreatePinnedToCore(core_1, "core_1", 4096, NULL, 1, &core1_Handle, 1);
    delay(100);
    
    // Input pins
    if(testInput) {
      Serial.println("\n\n → Starting input pins test");
      
      for(int i = 0; i < 23; i++) {
        pinMode(inputPins[i], INPUT);
        delay(10);
      }
    
      Serial.println("");
      inputTestTime = millis();
    } else {
      Serial.println("Input Test Skipped!");
      testCompleted = true;
    }
}

void loop() {
  if(testInput) {
    if(!testCompleted && millis() - inputTestTime >= 40000) {
      Serial.println("");

      IO2  ? Serial.print(""): Serial.println("IO2  NOT OK!");
      IO4  ? Serial.print(""): Serial.println("IO4  NOT OK!");
      IO5  ? Serial.print(""): Serial.println("IO5  NOT OK!");
      IO12 ? Serial.print(""): Serial.println("IO12 NOT OK!");
      IO13 ? Serial.print(""): Serial.println("IO13 NOT OK!");
      IO14 ? Serial.print(""): Serial.println("IO14 NOT OK!");
      IO15 ? Serial.print(""): Serial.println("IO15 NOT OK!");
      IO16 ? Serial.print(""): Serial.println("IO16 NOT OK!");
      IO17 ? Serial.print(""): Serial.println("IO17 NOT OK!");
      IO18 ? Serial.print(""): Serial.println("IO18 NOT OK!");
      IO19 ? Serial.print(""): Serial.println("IO19 NOT OK!");
      IO21 ? Serial.print(""): Serial.println("IO21 NOT OK!");
      IO22 ? Serial.print(""): Serial.println("IO22 NOT OK!");
      IO23 ? Serial.print(""): Serial.println("IO23 NOT OK!");
      IO25 ? Serial.print(""): Serial.println("IO25 NOT OK!");
      IO26 ? Serial.print(""): Serial.println("IO26 NOT OK!");
      IO27 ? Serial.print(""): Serial.println("IO27 NOT OK!");
      IO32 ? Serial.print(""): Serial.println("IO32 NOT OK!");
      IO33 ? Serial.print(""): Serial.println("IO33 NOT OK!");
      IO34 ? Serial.print(""): Serial.println("IO34 NOT OK!");
      IO35 ? Serial.print(""): Serial.println("IO35 NOT OK!");
      IO36 ? Serial.print(""): Serial.println("IO36 NOT OK!"); 
      IO39 ? Serial.print(""): Serial.println("IO39 NOT OK!");

      inputTestTime = millis();
    }

    if (!IO2  && digitalRead(2)) {    Serial.println("INPUT PIN  2: OK!");     IO2 = 1;  }
    if (!IO4  && digitalRead(4)) {    Serial.println("INPUT PIN  4: OK!");     IO4 = 1;  }
    if (!IO5  && digitalRead(5)) {    Serial.println("INPUT PIN  5: OK!");     IO5 = 1;  }
    if (!IO12 && digitalRead(12)){    Serial.println("INPUT PIN 12: OK!");    IO12 = 1;  }
    if (!IO13 && digitalRead(13)){    Serial.println("INPUT PIN 13: OK!");    IO13 = 1;  }
    if (!IO14 && digitalRead(14)){    Serial.println("INPUT PIN 14: OK!");    IO14 = 1;  }
    if (!IO15 && digitalRead(15)){    Serial.println("INPUT PIN 15: OK!");    IO15 = 1;  }
    if (!IO16 && digitalRead(16)){    Serial.println("INPUT PIN 16: OK!");    IO16 = 1;  }
    if (!IO17 && digitalRead(17)){    Serial.println("INPUT PIN 17: OK!");    IO17 = 1;  }
    if (!IO18 && digitalRead(18)){    Serial.println("INPUT PIN 18: OK!");    IO18 = 1;  }
    if (!IO19 && digitalRead(19)){    Serial.println("INPUT PIN 19: OK!");    IO19 = 1;  }
    if (!IO21 && digitalRead(21)){    Serial.println("INPUT PIN 21: OK!");    IO21 = 1;  }
    if (!IO22 && digitalRead(22)){    Serial.println("INPUT PIN 22: OK!");    IO22 = 1;  }
    if (!IO23 && digitalRead(23)){    Serial.println("INPUT PIN 23: OK!");    IO23 = 1;  }
    if (!IO25 && digitalRead(25)){    Serial.println("INPUT PIN 25: OK!");    IO25 = 1;  }
    if (!IO26 && digitalRead(26)){    Serial.println("INPUT PIN 26: OK!");    IO26 = 1;  }
    if (!IO27 && digitalRead(27)){    Serial.println("INPUT PIN 27: OK!");    IO27 = 1;  }
    if (!IO32 && digitalRead(32)){    Serial.println("INPUT PIN 32: OK!");    IO32 = 1;  }
    if (!IO33 && digitalRead(33)){    Serial.println("INPUT PIN 33: OK!");    IO33 = 1;  }
    if (!IO34 && digitalRead(34)){    Serial.println("INPUT PIN 34: OK!");    IO34 = 1;  }
    if (!IO35 && digitalRead(35)){    Serial.println("INPUT PIN 35: OK!");    IO35 = 1;  }
    if (!IO36 && digitalRead(36)){    Serial.println("INPUT PIN 36: OK!");    IO36 = 1;  }
    if (!IO39 && digitalRead(39)){    Serial.println("INPUT PIN 39: OK!");    IO39 = 1;  }

    if(!testCompleted)
      testCompleted = verifyInputPins();
    else{
      Serial.println("Input Test Finished!");
      vTaskDelete(NULL);
    }
  }else
    vTaskDelete(NULL);

  delay(10);
}

void core_0(void * pvParameters) {
  while(1) {
    if(!core0) {
      core0 = 1;
      Serial.println("\n\n --- Core 0 OK! ---\n");
    }

    if(taskTested && testCompleted) {
      Serial.println("\n\n\n----- Test Completed! -----\n\n\n");
      vTaskDelete(core1_Handle);
      vTaskDelete(core0_Handle);
    }

    delay(100); 
  }
}

void core_1(void * pvParameters) {
  while(1) {
    if(!core1) {
      core1 = 1;
      Serial.println("\n --- Core 1 OK! ---\n");
    }

    if(!taskTested && core0 && core1) {
      taskTested = 1;
      Serial.println("Tasks OK!");
    }

    delay(100); 
  }
}

bool verifyInputPins(){
  if(IO2&&IO4&&IO5&&IO12&&IO13&&IO14&&IO15&&IO16&&IO17&&IO18&&IO19&&IO21&&IO22&&IO23&&IO25&&IO26&&IO27&&IO32&&IO33&&IO34&&IO35&&IO36&&IO39){
    Serial.println("\n --------------- Input pins are OK! ---------------");
    return 1;
  }else
    return 0;
}