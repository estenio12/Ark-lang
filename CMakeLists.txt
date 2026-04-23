cmake_minimum_required(VERSION 3.15)
project(ArkLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Essencial para o clangd e ferramentas de análise
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

if(NOT MSVC)
    add_compile_options(-Wall -Wextra -O3)
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

set_target_properties(arkc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test"
    OUTPUT_NAME arkc
)