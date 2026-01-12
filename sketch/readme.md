#line 1 "/home/feelhippo/Documents/IoT/idf-docker-builder/sketch/readme.md"
### Instructions

- Install Arduino CLI
- Install ESP32 Core: `arduino-cli core install esp32:esp32`
- Install ArduinoMqttClient: `arduino-cli lib install ArduinoMqttClient`
- Compile Sketch: `arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso sketch/ -v --build-path ./output`