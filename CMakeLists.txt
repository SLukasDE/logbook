cmake_minimum_required(VERSION 3.23)

project(
    logbook
    VERSION 0.4.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#target_include_directories(logbook PUBLIC ./src/main)
#target_compile_features(logbook PUBLIC cxx_std_11)
#set_property(TARGET logbook PROPERTY POSITION_INDEPENDENT_CODE ON)

if (DEFINED ${PROJECT_NAME}_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${${PROJECT_NAME}_SHARED_LIBS}")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(COMPILE_UNITTESTS "Weather to compile unittests" ON)
option(BUILD_SHARED_LIBS "Weather to compile shared libs" ON)

add_subdirectory(src/main)

if(NOT ALL_IN_ONE_ESL AND COMPILE_UNITTESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/test/main.cpp")
    add_subdirectory(src/test)
endif()

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME})

install(FILES "${PROJECT_NAME}Config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
