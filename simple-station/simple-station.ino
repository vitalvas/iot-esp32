#include <WiFi.h>
#include "esp_wifi.h"

const char* emojiList[] = {
  "😺", "😸", "😹", "😻", "😼", "😽", "🙀", "😿", "😾",
  "👍", "👌", "🤞", "🤟", "🤘", "👋", "🖖",
  "👈", "👉", "👆", "🖕", "👇", "☝", "🫵",
  "🍎", "🍊", "🍋", "🍌", "🍉", "🍇", "🍓", "🍒", "🍑", "🍍",
  "🥸", "🤓", "😈", "👹", "👺", "🤡", "💩", "👻", "💀", "☠️"
};

const int emojiCount = 5; // max - 8
const long interval = 5 * 60 * 1000;  // in minutes
const char* ssid = "SmartHomeGateway";
const char* password = "0a2b3c4f";

void startSoftAP() {
  WiFi.softAPdisconnect(true);
  delay(2000);  // Allow time for disconnection

  char randomString[(emojiCount * 4) + 1]; // one emoji - 4 bytes
  generateRandomEmojiString(randomString, emojiCount);

  WiFi.softAP(randomString);
}

void generateRandomEmojiString(char* result, size_t length) {
  srand(millis());  // Seed the random number generator

  int emojiCountInList = sizeof(emojiList) / sizeof(emojiList[0]);
  int usedIndices[emojiCountInList];

  for (int i = 0; i < emojiCountInList; ++i) {
    usedIndices[i] = 0;  // Initialize all indices as unused
  }

  for (size_t i = 0; i < length; ++i) {
    int randomIndex;

    while (usedIndices[randomIndex]) { // Ensure the emoji hasn't been used
      randomIndex = rand() % emojiCountInList;
    }

    usedIndices[randomIndex] = 1;  // Mark the emoji as used
    result[i * 4] = emojiList[randomIndex][0];
    result[i * 4 + 1] = emojiList[randomIndex][1];
    result[i * 4 + 2] = emojiList[randomIndex][2];
    result[i * 4 + 3] = emojiList[randomIndex][3];
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
