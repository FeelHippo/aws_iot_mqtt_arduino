#include <WiFi.h>
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoMqttClient.h>

// https://blog.brianbeach.com/posts/2021-09-25-aws-iot-mqtt/

// WiFi Info
const char WIFI_SSID[] = "Sunrise_3228896";
const char WIFI_PASSWORD[] = "kbno4mwf4Uaavwtp";

// AWS IoT Core Connection Information
const char AWS_IOT_ENDPOINT[] = "a2eqw4se8i5px2-ats.iot.eu-west-1.amazonaws.com";
const int AWS_IOT_PORT = 8883;
const char MQTT_TOPIC[]  = "smartThingTopic";

// Board Info
const char* TOPIC_LED_SET = "demo/led/set";
const char* TOPIC_LED_STATE = "demo/led/state";
const char* TOPIC_BUTTONS = "demo/buttons"; 
const int LED_PIN = 2;
const int BUTTON1_PIN = 12;
const int BUTTON2_PIN = 14;
const uint32_t DEBOUNCE_MS = 40;

// Time Info
const char* NTP_SERVER_1 = "pool.ntp.org";
const char* NTP_SERVER_2 = "time.nist.gov";
const long  GMT_OFFSET_SEC = 0;
const int   DAYLIGHT_OFFSET_SEC = 0;

// AWS Certs
const char AWS_ROOT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Certificato del device (Thing certificate)
const char DEVICE_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUWAq7ryczMLO4Nv3Gq70iwR+OH0IwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI2MDEwNTE2MDUw
NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOVuOrF0uVQ3lU6NnTCz
D2pKhNq8nweemqJ1pSJMSnS9laKLpoTqwHhm2MEYhh5/zCDel8eoM4hKRscLNf5I
psnHPxFuG9QGrUHqRuIJp7+D8quZ+5/ypFecTbIQ1weSxnaFzJnUvJqDLQgQhTqt
HpRb2bOfFQCqi58OIvVQ2XkscIkg5DvaHfxl9va3wtr/JCwVcvBVQKeGQGZ6R4os
wl9RLkRV9SJ2t9qA6J0BLqtkAT4NaG9FPxcprQbgY2I0uLYeOOkOZY+30SegV1Cs
ba5fBYlKvzHgddMa/Ia3mh2dhPjKU7QHvQC0pD0dyWqUvYYaMCzd1p3TrpBZvh/G
5MsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUlaapnmT1iujyRPD3wil+q3mmAtQwHQYD
VR0OBBYEFPr4yN8mHTyJyIrf7r02nolPfgQDMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB911VHSxDTfrNItFAs0XGPwoe/
FsgzThqHSTxGL8M43zlfDEliXsy3d0FTu+JDJKFJbgFSLa0hXFsIuVm64HkTVEQI
aWb3ZHsFl67Fa/OgRCahwRejkswyEn2m7eFPKNRNIaEwQk+Rvs7UYTzJwGzF4hbZ
pG8sCDImedRE6P1qXgAVVt+HmQnactFUOYeBe7yMVwkWorRLT1ta2ovt6Zdj9cyP
jMoSCHnN3hNLsGzaH0EGrYe22+M+80HCgO0kJnKPlq6VzV3ChrmyF0l1rfWGrSDq
3OlvM+hYvwUCeDCPQ9FIyD8F7Gd1RF6tVBi8Oel2FFUkb4t2GHVPd0/ZBMiE
-----END CERTIFICATE-----
)EOF";

// Private key del device
const char DEVICE_PRIVATE_KEY[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEA5W46sXS5VDeVTo2dMLMPakqE2ryfB56aonWlIkxKdL2Vooum
hOrAeGbYwRiGHn/MIN6Xx6gziEpGxws1/kimycc/EW4b1AatQepG4gmnv4Pyq5n7
n/KkV5xNshDXB5LGdoXMmdS8moMtCBCFOq0elFvZs58VAKqLnw4i9VDZeSxwiSDk
O9od/GX29rfC2v8kLBVy8FVAp4ZAZnpHiizCX1EuRFX1Ina32oDonQEuq2QBPg1o
b0U/FymtBuBjYjS4th446Q5lj7fRJ6BXUKxtrl8FiUq/MeB10xr8hreaHZ2E+MpT
tAe9ALSkPR3JapS9hhowLN3WndOukFm+H8bkywIDAQABAoIBAGvT14/pTOTQzsJB
ejwWG8uXxELNXWyGVSS/Z+Hrux6Mr5PaUGOttNGhfIOcFASniXd1Rh5W5glK52LK
2RBucrzogLKfbkLC9JDDG5dGdGllCRsvXnw43JY0ZqumkHkFuXKgxZ0Sx6VSu0pM
AWGnVXTiijmKLaPHg6E4UqO64Mq6fRLoY1YbSIVn6+1gYEJemTfjySQtx1NUoEFh
Wc9+ugzT57m6efpi8ITOQ34gNmr5wPe78+tHuPxFFr4K8DZHHFXmJgx0lMWeF+rb
nhieYP7sxBuaH4+oL2AhLT1sXUgJkfWOdijM9IC3QNCUxAbu47A6Au/pVYC5lWiP
p6A8kBECgYEA/OI29UijMVj/Bga7M80D/f3P9oL9iZunFOwUqTE9NImIlqqq5k0k
rGeNq84WEB1ZVDrKjMutwfAZpzLkYALOI6DKqqZx851TU8kogG+cJDtBwVbqII1M
Re3VEZblVMNMWQSHUmDL4q0gdHLhUbt7VHgk/GfJehzUqeBwPlsQU8kCgYEA6EIG
pbbLL8Ftdn28O0CwsMlFYtQH55NqLodCC0lUrK62QsspOCrHeB942XNJmVrAIZWq
3rQ500aLrhETdSM3DXUeqYqkFNc3Hx4xBKXup1/d92NOCLTr1Kq27u36wh7XgDj6
H+zz2TcqBUDm3Pgsh596BG/cPTxgWis8hTJY9fMCgYABuY1EaQVr7b53jQ4z0T8o
uVV0yfnuuVgPZXjSmppfRFzhIjLeNUJ9xosJ0/MEkx6r22bMRQtfWkLiiA9UtFAx
oAQh29bFIZqS5LlY/uniuWC+b9hS9oPBqlicsRnJVTetF8HQhwZ8+k9RXAM7anrx
3YvrCM4v/ghZTEWXt0lKOQKBgDMyCqEQj+0w594MjSdKdSgi1YaZUjy7g9ZJvkca
nO2LfWVvvuBAncNevJbCEOEa3esJQKbidzSZAkUWQoG5h3MR8/ZPNyfY4K+dShTK
NrbyLM1ROGw25Rz1nGlCT9nmTn5XENtkscOZo0uRKNJgagBjKNQ9jAB7hImbqjV6
CZ39AoGAEQ/ALcRqi0b2ok+92Vb5I/dtHHAGTbqxNMFOohgTe7XQ/84am/wfBV4M
RVGHVX4AiTeI5yHBD4G1q2syrMlKoWDr5QSIuJrrEzTWNXfjHkYPjPE2IJWhgVIf
LDRVBquGud1K3lDouofwQSajhMwMU7mPujy2NFIfkeslqSbUj0Y=
-----END RSA PRIVATE KEY-----
)EOF";

// https://docs.arduino.cc/tutorials/uno-wifi-rev2/uno-wifi-r2-mqtt-device-to-device/
WiFiClientSecure wifiClient = WiFiClientSecure();
// https://arduino.stackexchange.com/a/93805
MqttClient mqttClient(Client(wifiClient));

struct ButtonState {
  int pin;
  bool stableLevel;
  bool lastReadLevel;
  uint32_t lastChangeMs;
};

ButtonState b1{BUTTON1_PIN, true, true, 0};
ButtonState b2{BUTTON2_PIN, true, true, 0};

void publishJson(const char* topic, const String& json) {
  mqttClient.beginMessage(topic);
  mqttClient.print(json);
  mqttClient.endMessage();
}

void setLed(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);

  // Pubblica stato (opzionale)
  String js = String("{\"led\":") + (on ? "true" : "false") +
              ",\"ts\":" + String((uint32_t)millis()) + "}";
  publishJson(TOPIC_LED_STATE, js);
}

void onMqttMessage(int messageSize) {
  String t = mqttClient.messageTopic();
  if (t == TOPIC_LED_SET) {
    char s = (char)mqttClient.read();
    bool on = s == 'on' ? true : false;
    setLed(on);
  }
}

void handleButton(ButtonState& b, int id) {
  bool readLevel = digitalRead(b.pin);

  if (readLevel != b.lastReadLevel) {
    b.lastReadLevel = readLevel;
    b.lastChangeMs = millis();
  }

  if ((millis() - b.lastChangeMs) >= DEBOUNCE_MS && readLevel != b.stableLevel) {
    b.stableLevel = readLevel;

    bool pressed = (b.stableLevel == LOW);

    String js = String("{\"button\":") + id +
                ",\"pressed\":" + (pressed ? "true" : "false") +
                ",\"ts\":" + String((uint32_t)millis()) + "}";

    publishJson(TOPIC_BUTTONS, js);
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  wifiClient.setCACert(AWS_ROOT_CA);
  wifiClient.setCertificate(DEVICE_CERT);
  wifiClient.setPrivateKey(DEVICE_PRIVATE_KEY);

  if (!mqttClient.connect(AWS_IOT_ENDPOINT, AWS_IOT_PORT)) {
    while (1);
  }
  mqttClient.onMessage(onMqttMessage);
  mqttClient.subscribe(MQTT_TOPIC);
}

void loop()
{
  mqttClient.poll();
  handleButton(b1, 1);
  handleButton(b2, 2);
  delay(10);
}