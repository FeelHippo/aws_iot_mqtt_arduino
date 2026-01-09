### AWS IoT Core from Arduino

This project attempts to compile an Arduino sketch in Standard C++, for the ESP32 family of SoCs.

### Status Quo:

- Docker build: `sudo docker run -v $PWD:/project -w /project -e IDF_GIT_SAFE_DIR='/project' -u $UID -e HOME=/tmp espressif/idf:release-v5.5 idf.py build`
    - [IDF Docker Image](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/idf-docker-image.html)
- [Sketch](https://blog.brianbeach.com/posts/2021-09-25-aws-iot-mqtt/)
- [Sending Data over MQTT](https://docs.arduino.cc/tutorials/uno-wifi-rev2/uno-wifi-r2-mqtt-device-to-device/)
- [Current Blocker](https://arduino.stackexchange.com/questions/102099/standard-c-type-conversion-for-wificlientsecure-and-arduinomqttclient)
- [ESP IDF CI Action (TODO)](https://github.com/marketplace/actions/espressif-iot-development-framework-esp-idf)