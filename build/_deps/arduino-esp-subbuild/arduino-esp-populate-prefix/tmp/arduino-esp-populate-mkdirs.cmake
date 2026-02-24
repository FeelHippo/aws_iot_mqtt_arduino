# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/project/build/_deps/arduino-esp-src")
  file(MAKE_DIRECTORY "/project/build/_deps/arduino-esp-src")
endif()
file(MAKE_DIRECTORY
  "/project/build/_deps/arduino-esp-build"
  "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix"
  "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix/tmp"
  "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix/src/arduino-esp-populate-stamp"
  "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix/src"
  "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix/src/arduino-esp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix/src/arduino-esp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/project/build/_deps/arduino-esp-subbuild/arduino-esp-populate-prefix/src/arduino-esp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
