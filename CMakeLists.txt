cmake_minimum_required(VERSION 3.23)

project(logbook VERSION 2.0.0
            LANGUAGES CXX)

#include(FetchContent)
#FetchContent_Declare(
#  catch
#  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#  GIT_TAG        v2.13.6
#)
#FetchContent_MakeAvailable(catch)


# The compiled library code is here
#add_subdirectory(src/main)
# STATIC, SHARED, MODULE, INTERFACE
add_library(logbook STATIC
      ./src/main/logbook/Location.h
      ./src/main/logbook/Logger.cpp
      ./src/main/logbook/appender/MemBuffer.cpp
      ./src/main/logbook/appender/MemBuffer.h
      ./src/main/logbook/appender/OStream.h
      ./src/main/logbook/appender/OStream.cpp
      ./src/main/logbook/Writer.cpp
      ./src/main/logbook/Layout.h
      ./src/main/logbook/Stream.cpp
      ./src/main/logbook/StreamBuffer.h
      ./src/main/logbook/Appender.cpp
      ./src/main/logbook/Writer.h
      ./src/main/logbook/StreamBuffer.cpp
      ./src/main/logbook/layout/Default.h
      ./src/main/logbook/layout/Default.cpp
      ./src/main/logbook/Logbook.h
      ./src/main/logbook/Stream.h
      ./src/main/logbook/Appender.h
      ./src/main/logbook/Logbook.cpp
      ./src/main/logbook/Level.h
      ./src/main/logbook/Logger.h
)

target_include_directories(logbook PUBLIC ./src/main)
target_compile_features(logbook PUBLIC cxx_std_11)
set_property(TARGET logbook PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS logbook
        EXPORT logbook-export
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(EXPORT      logbook-export
        FILE        logbookTargets.cmake
        NAMESPACE   logbook::
        DESTINATION lib/cmake
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/logbookConfig.cmake
        DESTINATION "lib/cmake"
)

