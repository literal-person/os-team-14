# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-src"
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-build"
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix"
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix/tmp"
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix/src"
  "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mark-mannion/Desktop/RP2040/firmware/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
