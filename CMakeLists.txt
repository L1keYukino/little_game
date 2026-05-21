cmake_minimum_required(VERSION 3.16)
project(farm_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC 默认用系统编码(GBK)解析源文件，中文注释会导致编译错误
# /utf-8 告诉编译器源文件是 UTF-8 编码
if(MSVC)
  add_compile_options(/utf-8)
endif()

# SFML 3.0 pre-built
set(SFML_DIR "D:/dev/SFML-3.0.0/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS Graphics Window System)

# 所有源文件（按目录分组）
set(SOURCES
    src/main.cpp
    src/core/Game.cpp
    src/core/Camera.cpp
    src/world/TileMap.cpp
    src/world/GameTime.cpp
    src/entities/Player.cpp
    src/entities/Crop.cpp
    src/entities/DroppedItem.cpp
    src/scenes/FarmScene.cpp
    src/systems/FarmingSystem.cpp
    src/ui/Inventory.cpp
)

add_executable(farm_game ${SOURCES})
target_link_libraries(farm_game PRIVATE SFML::Graphics)

# 编译后自动把 SFML DLL 复制到 exe 旁边（否则运行时会报找不到 dll）
if(WIN32)
  add_custom_command(TARGET farm_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SFML::Graphics>"
    "$<TARGET_FILE:SFML::Window>"
    "$<TARGET_FILE:SFML::System>"
    "$<TARGET_FILE_DIR:farm_game>"
  )
endif()
