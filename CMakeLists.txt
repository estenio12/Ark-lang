cmake_minimum_required(VERSION 3.15)
project(ArkLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

# Otimizações de compilação
if(MSVC)
    # Para Windows (Visual Studio)
    add_compile_options(/W4 /MP)
else()
    # Para GCC/Clang (Linux/Mac)
    add_compile_options(-Wall -Wextra -O3)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    src/CoreCompile.cpp
    src/FileHandler.cpp
    src/Lexer.cpp
    src/main.cpp
    src/ModuleManager.cpp
)

# Define o nome do executável e os fontes
add_executable(arkc ${SOURCES})

# Define o local de saída do executável para ./test/arkc
set_target_properties(arkc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test"
    OUTPUT_NAME arkc
)