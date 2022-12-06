# ESP32 Tester

Output Test:
  Code writes 1 on every port and then read them, if they return 1, the port is working correctly.
  
Input Test:
  Code waits for an input signal (You'll need to test them manually, or depending on your ESP32 Dev Board you can just touch the pins).
 
Non Volatile Storage Test:
  Code writes 1024 to a key and read the value, if return 1024, NVS is working correctly.

EEPROM Test:
  Code writes 1 on every memory spaces and then read them, if they return 1, esp32 memory is working correctly.
 
Wi-Fi Test:
  The ESP32 will try to connect to wi-fi 5 times (3 seconds each).
  
Task Test:
  The code will create 2 tasks, one on core 0 and the other on core 1.
  
  
You can choose if you want to run all the tests or just some of them, you just need to change the variables on the "User Area" highlighted at main.cpp
  (0 = won't run) 
  (1 = will run).
  
You can also change EEPROM size and the time to get test results at main.h (default = 1 minute).
