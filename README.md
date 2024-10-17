# ESP32 Hardware Testing Code

If you need to test multiple ESP32 boards before soldering them onto a PCB, this project is for you.

Follow these instructions:

   **Open the project and locate the "User Area" section in src/main.cpp. Select the tests you want to run and enter your Wi-Fi credentials (must be on a 2.4GHz network).**

   **Upload the code to your ESP32 and then open the Serial Monitor.**

   **At the end of the script, the test results and their duration will be displayed in the Serial Monitor.**

*All tests are automated, except for the input pins. You will need to perform those tests manually when prompted (you'll have one minute by default). The input-only pins are: 34, 35, 36, and 39.
