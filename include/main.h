#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>

#define TIME_TO_GET_RESULTS 10000
#define EEPROM_SIZE 100

/**
 * @brief Task assigned to core 0.
 * 
 * @param pvParameters 
 */
void core_0(void *pvParameters);

/**
 * @brief Task assigned to core 1.
 * 
 * @param pvParameters 
 */
void core_1(void *pvParameters);

/**
 * @brief Verify if all the input pins are working correctly.
 * 
 * @return bool
 */
bool verifyInputPins();