cmake_minimum_required(VERSION 3.15)
project(ArkLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

if(NOT MSVC)
    add_compile_options(-Wall -Wextra -Wpedantic)
else()
    add_compile_options(/W4)
endif()

include_directories(src)

set(SOURCES
    src/CoreCompile.cpp
    src/FileHandler.cpp
    src/Lexer.cpp
    src/main.cpp
    src/ModuleManager.cpp
)

add_executable(arkc ${SOURCES})

set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(arkc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}"
    OUTPUT_NAME arkc
)

add_custom_command(TARGET arkc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:arkc>"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/$<TARGET_FILE_NAME:arkc>"
    COMMENT "Sincronizando binário com a pasta de testes..."
    VERBATIM
)