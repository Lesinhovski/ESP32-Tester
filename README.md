# ESP32 Hardware Testing Code

If you need to test multiple ESP32s, before soldering it on a board, this project is for you.

Follow the instructions:

**1 - Open the project and search for "User Area" at src/main.cpp, select which tests that you want to run and enter your Wi-Fi credentials (must be a 2.4GHz wi-fi).**

**2 - Now, you just need to upload the code to your ESP32 then open the Serial Monitor.**

**3 - At the end of the script, will be shown on your serial monitor the test results and its duration.**


  *All the tests are automated except the input pins one, you'll need to do it manually when it begins (You'll have one minute by default).
    Input Only Pins: 34, 35, 36 and 39.
