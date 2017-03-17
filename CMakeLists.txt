cmake_minimum_required(VERSION 3.3)
project(savior)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

SET(SOURCE_FILES
        main.cpp
        src/PlayerShip.cpp include/PlayerShip.h
        src/TextureWrapper.cpp include/TextureWrapper.h
        include/rapidxml.hpp include/rapidxml_utils.hpp
        src/Object.cpp include/Object.h
        src/EnemyShip.cpp include/EnemyShip.h
        include/collisionCheck.h
        src/Asteroid.cpp include/Asteroid.h
        src/UFO.cpp include/UFO.h
        src/Shield.cpp include/Shield.h
        src/Weapon.cpp include/Weapon.h)

ADD_EXECUTABLE(savior ${SOURCE_FILES} savior.sh SaviorShip.desktop)
FILE(COPY "${PROJECT_SOURCE_DIR}/assets" "${PROJECT_SOURCE_DIR}/SaviorShip.desktop" "${PROJECT_SOURCE_DIR}/savior.sh" DESTINATION "${CMAKE_BINARY_DIR}")

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION savior_ship)
INSTALL(FILES "${PROJECT_SOURCE_DIR}/SaviorShip.desktop" DESTINATION /usr/share/applications)
INSTALL(FILES "${PROJECT_SOURCE_DIR}/savior.sh" DESTINATION savior_ship)
INSTALL(TARGETS savior DESTINATION savior_ship)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${SDL2TTF_LIBRARIES})


SET(VERSION "0.1.0")
SET(CPACK_PACKAGE_VERSION ${VERSION})
SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME "savior_ship")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An opensource SDL2 game")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md")
SET(CPACK_PACKAGE_RELEASE 1)
SET(CPACK_PACKAGE_CONTACT "Lahiru Pathirage")
SET(CPACK_PACKAGE_VENDOR "lpsandaruwan")
SET(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
