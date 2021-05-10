#include <AboutHelper.h>
#include <ArduinoJson.h>
#include <Settings.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#elif ESP32
  #include <WiFi.h>
#endif

String AboutHelper::generateAboutString(bool abbreviated) {
  DynamicJsonDocument buffer(1024);

  generateAboutObject(buffer, abbreviated);

  String body;
  serializeJson(buffer, body);

  return body;
}
#ifdef ESP32
char *getResetReason(void)
{
    esp_reset_reason_t reset_reason = esp_reset_reason();

    static char str[55];

    switch (reset_reason)
    {
      case ESP_RST_UNKNOWN:    strcpy (str, "Reset reason can not be determined"); break;
      case ESP_RST_POWERON:    strcpy (str, "Reset due to power-on event"); break;
      case ESP_RST_EXT:        strcpy (str, "Reset by external pin (not applicable for ESP32)"); break;
      case ESP_RST_SW:         strcpy (str, "Software reset via esp_restart"); break;
      case ESP_RST_PANIC:      strcpy (str, "Software reset due to exception/panic"); break;
      case ESP_RST_INT_WDT:    strcpy (str, "Reset (software or hardware) due to interrupt watchdog"); break;
      case ESP_RST_TASK_WDT:   strcpy (str, "Reset due to task watchdog"); break;
      case ESP_RST_WDT:        strcpy (str, "Reset due to other watchdogs"); break;
      case ESP_RST_DEEPSLEEP:  strcpy (str, "Reset after exiting deep sleep mode"); break;
      case ESP_RST_BROWNOUT:   strcpy (str, "Brownout reset (software or hardware)"); break;
      case ESP_RST_SDIO:       strcpy (str, "Reset over SDIO"); break;
    }

    return str;
}
#endif
void AboutHelper::generateAboutObject(JsonDocument& obj, bool abbreviated) {
  obj["firmware"] = QUOTE(FIRMWARE_NAME);
  obj["version"] = QUOTE(MILIGHT_HUB_VERSION);
  obj["ip_address"] = WiFi.localIP().toString();
  #ifdef ESP8266
    obj["reset_reason"] = ESP.getResetReason();
  #elif ESP32
    obj["reset_reason"] = getResetReason();
  #endif

  if (! abbreviated) {
    obj["variant"] = QUOTE(FIRMWARE_VARIANT);
    obj["free_heap"] = ESP.getFreeHeap();
    #ifdef ESP8266
      obj["arduino_version"] = ESP.getCoreVersion();
    #elif ESP32
      obj["arduino_version"] = ESP.getSdkVersion();
    #endif
  }
}