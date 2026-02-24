#include <WiFi.h>

const char WIFI_SSID[] = "Sunrise_3228896";
const char WIFI_PASSWORD[] = "kbno4mwf4Uaavwtp";
const int LED_PIN = 2;
const int BUTTON1_PIN = 12;
const int BUTTON2_PIN = 14;

struct ButtonState
{
  int pin;
  bool stableLevel;
  bool lastReadLevel;
  uint32_t lastChangeMs;
};

ButtonState b1{BUTTON1_PIN, true, true, 0};
ButtonState b2{BUTTON2_PIN, true, true, 0};

void handleButton(ButtonState &b, int id)
{
  bool on = digitalRead(b.pin);

  digitalWrite(LED_PIN, on ? LOW : HIGH);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
  }
}

void loop()
{
  handleButton(b1, 1);
  handleButton(b2, 2);
  delay(1000);
}