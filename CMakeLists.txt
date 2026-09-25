cmake_minimum_required(VERSION 3.28)
project(Breakout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force les dépendances tierces (Freetype) à accepter les policies récentes.
# Nécessaire car CMake 4.x a supprimé la compat avec cmake_minimum_required < 3.5.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)

add_executable(breakout
    src/main.cpp
)

target_include_directories(breakout PRIVATE include)

target_link_libraries(breakout PRIVATE SFML::Graphics SFML::Window SFML::System)