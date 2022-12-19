# ESP32 Tester

How to use this code.

1 - Go to User Area at src/main.cpp Choose what tests you want to run and enter your Wi-Fi credentials(must be a 2.4GHz wifi).

1 = Run the test.
0 = Don't run the test.

User Area will look like this ->
 
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
bool testWifi = 1; 
 

 
/* ------------------------------------------------------ */
 

2 - Upload the code to your ESP32 and open the Serial Monitor.

3 - In the end of the script, will appear on your serial monitor the test results and its duration.
