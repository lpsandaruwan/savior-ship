cmake_minimum_required(VERSION 3.6)
project(savior)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES
        main.cpp
        src/PlayerShip.cpp src/PlayerShip.h
        src/TextureWrapper.cpp src/TextureWrapper.h
        src/rapidxml.hpp src/rapidxml_utils.hpp
        src/Object.cpp src/Object.h
        src/EnemyShip.cpp src/EnemyShip.h
        src/collisionCheck.h
        src/Asteroid.cpp src/Asteroid.h
        src/UFO.cpp src/UFO.h
        src/Shield.cpp src/Shield.h
        src/Weapon.cpp src/Weapon.h)
add_executable(savior ${SOURCE_FILES})

file(COPY "assets" DESTINATION "${CMAKE_BINARY_DIR}")

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})
