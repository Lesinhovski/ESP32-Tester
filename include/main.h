#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <Preferences.h>
#include <SPIFFS.h>

#define TIME_TO_GET_RESULTS 60000
#define EEPROM_SIZE 512

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