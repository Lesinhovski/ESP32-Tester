#include <Arduino.h>
#include <WiFi.h>

void core_0(void *pvParameters);
void core_1(void *pvParameters);

/**
 * @brief Verify if all the input pins are working correctly
 * 
 * @return true
 * @return false
 */
bool verifyInputPins();