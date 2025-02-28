#include <WiFi.h>
#include "esp_wifi.h"

const char* funny_emoji_list[] = {
  "😺", "😸", "😹", "😻", "😼", "😽", "🙀", "😿", "😾",
  "👍", "👌", "🤞", "🤟", "🤘", "👋", "🖖",
  "👈", "👉", "👆", "🖕", "👇", "☝", "🫵",
  "🍎", "🍊", "🍋", "🍌", "🍉", "🍇", "🍓", "🍒", "🍑", "🍍",
  "🥸", "🤓", "😈", "👹", "👺", "🤡", "💩", "👻", "💀", "☠️"
};

const size_t emoji_count = 5; // max - 8
const long interval = 5 * 60 * 1000;  // in minutes
const char* ssid = "SmartHomeGateway";
const char* password = "0a2b3c4f";

void startSoftAP() {
  WiFi.softAPdisconnect(true);
  delay(2000);  // Allow time for disconnection

  char randomString[(emoji_count * 4) + 1]; // one emoji - 4 bytes
  generateRandomEmojiString(randomString, emoji_count);

  WiFi.softAP(randomString);
}

void generateRandomEmojiString(char* result, size_t length) {
  srand(millis());  // Seed the random number generator
  int emoji_count = sizeof(funny_emoji_list) / sizeof(funny_emoji_list[0]);
  int used_indices[emoji_count];

  for (int i = 0; i < emoji_count; ++i) {
    used_indices[i] = 0;  // Initialize all indices as unused
  }

  for (size_t i = 0; i < length; ++i) {
    int randomIndex;
    do {
      randomIndex = rand() % emoji_count;
    } while (used_indices[randomIndex]);  // Ensure the emoji hasn't been used

    used_indices[randomIndex] = 1;  // Mark the emoji as used
    result[i * 4] = funny_emoji_list[randomIndex][0];
    result[i * 4 + 1] = funny_emoji_list[randomIndex][1];
    result[i * 4 + 2] = funny_emoji_list[randomIndex][2];
    result[i * 4 + 3] = funny_emoji_list[randomIndex][3];
  }

  result[length * 4] = '\0';  // Null-terminate the string
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  // someone connected, changing wifi name :)
  startSoftAP();
}

void setup() {
  Serial.begin(115200);
  delay(100);
  setCpuFrequencyMhz(80);
  delay(500);

  // WiFi.mode(WIFI_AP_STA);
  // WiFi.begin(ssid, password);
  WiFi.mode(WIFI_AP);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STACONNECTED);
}

void loop() {
  startSoftAP();
  delay(interval);
}
