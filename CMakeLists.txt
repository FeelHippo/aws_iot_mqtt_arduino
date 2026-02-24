cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
list(APPEND EXTRA_COMPONENT_DIRS build/_deps)

project(smart-thing-lib)

include(FetchContent)
FetchContent_Declare(
        arduino-wifi
        GIT_REPOSITORY https://github.com/arduino-libraries/WiFi.git
)
FetchContent_MakeAvailable(arduino-wifi)

add_library(arduino-wifi INTERFACE)
target_include_directories(arduino-wifi
        SYSTEM INTERFACE
        ${arduino-wifi_SOURCE_DIR}/src
)

add_library(smart-thing-lib STATIC ./smart-thing-mqtt.cpp)
set_target_properties(smart-thing-lib PROPERTIES CXX_STANDARD 17)

target_link_libraries(smart-thing-lib
        PRIVATE
        arduino-wifi
        -llog
)

## To Run:
## sudo systemctl start docker
## sudo docker run -v $PWD:/project -w /project -e IDF_GIT_SAFE_DIR='*' -u $UID -e HOME=/tmp espressif/idf:release-v5.5 idf.py build
