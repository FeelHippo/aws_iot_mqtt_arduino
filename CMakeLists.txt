cmake_minimum_required(VERSION 3.10.2)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(smart-thing-lib)

include(FetchContent)
FetchContent_Declare(
        avr-libc
        GIT_REPOSITORY https://github.com/avrdudes/avr-libc.git
        GIT_TAG avr-libc-2_3_0-release

)
FetchContent_Declare(
        mbedtls_components
        GIT_REPOSITORY https://github.com/FeelHippo/esp-idf.git
)
FetchContent_Declare(
        arduino-mqtt
        GIT_REPOSITORY https://github.com/arduino-libraries/ArduinoMqttClient.git
)
FetchContent_Declare(
        arduino-cloud
        GIT_REPOSITORY https://github.com/arduino-libraries/ArduinoCloud.git
)
FetchContent_Declare(
        arduino-core
        GIT_REPOSITORY https://github.com/arduino/ArduinoCore-avr.git
)
FetchContent_Declare(
        arduino-esp
        GIT_REPOSITORY https://github.com/FeelHippo/arduino-esp32.git
)
FetchContent_Declare(
        arduino-wifi
        GIT_REPOSITORY https://github.com/arduino-libraries/WiFi.git
)
FetchContent_MakeAvailable(avr-libc mbedtls_components arduino-mqtt arduino-cloud arduino-core arduino-esp arduino-wifi)
add_library(avr-libc INTERFACE)
configure_file(${avr-libc_SOURCE_DIR}/include/avr/version.h.in ${avr-libc_SOURCE_DIR}/include/avr/version.h)
configure_file(${avr-libc_SOURCE_DIR}/include/util/delay.h.in ${avr-libc_SOURCE_DIR}/include/util/delay.h)
target_include_directories(avr-libc
        SYSTEM INTERFACE
        ${avr-libc_SOURCE_DIR}/include
        ${avr-libc_SOURCE_DIR}/include/avr
)

add_library(mbedtls_components INTERFACE)
target_include_directories(mbedtls
        SYSTEM INTERFACE
        ${mbedtls_components_SOURCE_DIR}/components/mbedtls/port/include
)

add_library(arduino-mqtt INTERFACE)
target_include_directories(arduino-mqtt
        SYSTEM INTERFACE
        ${arduino-mqtt_SOURCE_DIR}/src
)

add_library(arduino-cloud INTERFACE)
target_include_directories(arduino-cloud
        SYSTEM INTERFACE
        ${arduino-cloud_SOURCE_DIR}/src/lib
)

add_library(arduino-core INTERFACE)
target_include_directories(arduino-core
        SYSTEM INTERFACE
        ${arduino-core_SOURCE_DIR}/cores/arduino
        ${arduino-core_SOURCE_DIR}/variants/standard
)

add_library(arduino-esp INTERFACE)
target_include_directories(arduino-esp
        SYSTEM INTERFACE
        ${arduino-esp_SOURCE_DIR}/tools/sdk/include
        ${arduino-esp_SOURCE_DIR}/tools/sdk/lwip2/include/
        ${arduino-esp_SOURCE_DIR}/cores/esp8266
        ${arduino-esp_SOURCE_DIR}/libraries/NetworkClientSecure/src
)

add_library(arduino-wifi INTERFACE)
target_include_directories(arduino-wifi
        SYSTEM INTERFACE
        ${arduino-wifi_SOURCE_DIR}/src
)

add_library(smart-thing-lib STATIC ./smart-thing-mqtt.cpp)
set_target_properties(smart-thing-lib PROPERTIES CXX_STANDARD 23)

target_link_libraries(smart-thing-lib
        PRIVATE
        avr-libc
        mbedtls
        arduino-mqtt
        arduino-cloud
        arduino-core
        arduino-esp
        arduino-wifi
        -llog
)

## Install ESP-IDF: https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started-cmake/index.html#step-1-set-up-toolchain
## To Run: sudo docker run -v $PWD:/project -w /project -e IDF_GIT_SAFE_DIR='/project' -u $UID -e HOME=/tmp espressif/idf:release-v5.5 idf.py build

# If it doesn't work, use my FORK in the udo docker run -v $PWD:/project -w ... command
# see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-docker-image.html
