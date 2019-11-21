cmake_minimum_required (VERSION 3.14)

project (savior VERSION 0.1.2
  DESCRIPTION "An opensource SDL2 game. ${GIT_REVISION}"
  HOMEPAGE_URL "https://github.com/lpsandaruwan/savior-ship"
)

if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
  find_package (Git QUIET)
  if (GIT_FOUND)
    execute_process (
      COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_SOURCE_DIR} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_REVISION
    )
    string (REGEX REPLACE "\n" "" GIT_REVISION "${GIT_REVISION}")
  else ()
    set (GIT_REVISION "")
    message (STATUS "Git revision not found.")
  endif ()
endif ()

if (MSVC AND (MSVC_VERSION GREATER 1900))
    if (${use_control_flow_guard})
        message ("Setting control flow guard") 
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /guard:cf")
        set (CMAKE_EXE_LINKER_FLAGS  "/guard:cf /DYNAMICBASE")
    endif ()
    if (${use_qspectre})
        message ("Setting spectre mitigation")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Qspectre")
    endif ()
    if (${use_qpar})
        message ("Setting parallel code generation")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Qpar")
    endif ()
    if (${use_ot})
        message ("Generating fast code")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Ot")
    endif ()
    if (${use_o2})
        message ("Maximum optimisation for speed")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
    endif ()
    if (${use_mp})
        message ("Multi processor compilation")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    endif ()
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (MACOSX TRUE)
endif ()

if (MACOSX)
  set (CMAKE_FIND_LIBRARY_PREFIXES "lib")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib;.so;.a")
elseif (LINUX OR UNIX AND NOT APPLE)
  set (CMAKE_FIND_LIBRARY_PREFIXES "lib")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
elseif (MSVC OR WIN32)
  set (CMAKE_FIND_LIBRARY_PREFIXES "")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
endif ()

set (SRC_FILES
          main.cpp
          src/PlayerShip.cpp
          include/PlayerShip.h
          src/TextureWrapper.cpp
          include/TextureWrapper.h
          include/rapidxml.hpp
          include/rapidxml_utils.hpp
          src/Object.cpp
          include/Object.h
          src/EnemyShip.cpp
          include/EnemyShip.h
          include/collisionCheck.h
          src/Asteroid.cpp
          include/Asteroid.h
          src/UFO.cpp
          include/UFO.h
          src/Shield.cpp
          include/Shield.h
          src/Weapon.cpp
          include/Weapon.h
)

if (MSVC OR WIN32)

  set (SRC_FILES ${SRC_FILES} include/usleep.h)
  set (VCPKG_P_DIR ${CMAKE_SOURCE_DIR}/vcpkg/packages)
  set (SDL2_INCLUDE_DIRS ${VCPKG_P_DIR}/sdl2_x64-windows/include/SDL2)
  set (SDL2IMAGE_INCLUDE_DIRS ${VCPKG_P_DIR}/sdl2-image_x64-windows/include/SDL2)
  set (SDL2MIXER_INCLUDE_DIRS ${VCPKG_P_DIR}/sdl2-mixer_x64-windows/include/SDL2)
  set (SDL2TTF_INCLUDE_DIRS ${VCPKG_P_DIR}/sdl2-ttf_x64-windows/include/SDL2)
  set (SDL2MAIN_LIBRARIES ${VCPKG_P_DIR}/sdl2_x64-windows/lib/manual-link/SDL2main.lib)
  set (SDL2_LIBRARIES ${VCPKG_P_DIR}/sdl2_x64-windows/lib/SDL2.lib)
  set (SDL2IMAGE_LIBRARIES ${VCPKG_P_DIR}/sdl2-image_x64-windows/lib/SDL2_image.lib)
  set (SDL2MIXER_LIBRARIES ${VCPKG_P_DIR}/sdl2-mixer_x64-windows/lib/SDL2_mixer.lib)
  set (SDL2TTF_LIBRARIES ${VCPKG_P_DIR}/sdl2-ttf_x64-windows/lib/SDL2_ttf.lib)

  find_package (SDL2 REQUIRED sdl2)
  find_package (SDL2-IMAGE REQUIRED SDL2_image>=2.0.0)
  find_package (SDL2-TTF REQUIRED SDL2_ttf>=2.0.0)
  find_package (SDL2-MIXER REQUIRED SDL2_mixer>=2.0.0)

  set (WIN_DLL_FILES
          ${VCPKG_P_DIR}/bzip2_x64-windows/bin/bz2.dll
          ${VCPKG_P_DIR}/freetype_x64-windows/bin/freetype.dll
          ${VCPKG_P_DIR}/libogg_x64-windows/bin/ogg.dll
          ${VCPKG_P_DIR}/libpng_x64-windows/bin/libpng16.dll
          ${VCPKG_P_DIR}/libvorbis_x64-windows/bin/vorbis.dll
          ${VCPKG_P_DIR}/libvorbis_x64-windows/bin/vorbisenc.dll
          ${VCPKG_P_DIR}/libvorbis_x64-windows/bin/vorbisfile.dll
          ${VCPKG_P_DIR}/sdl2_x64-windows/bin/SDL2.dll
          ${VCPKG_P_DIR}/sdl2-image_x64-windows/bin/SDL2_image.dll
          ${VCPKG_P_DIR}/sdl2-mixer_x64-windows/bin/SDL2_mixer.dll
          ${VCPKG_P_DIR}/sdl2-ttf_x64-windows/bin/SDL2_ttf.dll
          ${VCPKG_P_DIR}/zlib_x64-windows/bin/zlib1.dll
  )

  if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    file (COPY ${WIN_DLL_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Release)
    file (COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/Release)
  else()
    file (COPY ${WIN_DLL_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Debug)
    set (COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/Debug)
  endif ()

  include_directories (${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
  add_executable (savior WIN32 ${SRC_FILES})
  install (TARGETS savior DESTINATION ./build)
  target_link_libraries (${PROJECT_NAME} ${SDL2MAIN_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${SDL2TTF_LIBRARIES})

elseif (LINUX OR UNIX AND NOT APPLE)

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
  include_directories (${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
  add_executable (savior ${SRC_FILES} savior.sh SaviorShip.desktop)
  set (COPY "${PROJECT_SOURCE_DIR}/assets" "${PROJECT_SOURCE_DIR}/SaviorShip.desktop" "${PROJECT_SOURCE_DIR}/savior.sh" DESTINATION "${CMAKE_BINARY_DIR}")
  install (DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION savior_ship)
  install (FILES "${PROJECT_SOURCE_DIR}/SaviorShip.desktop" DESTINATION /usr/share/applications)
  install (FILES "${PROJECT_SOURCE_DIR}/savior.sh" DESTINATION savior_ship)
  install (TARGETS savior DESTINATION savior_ship)

  include (FindPkgConfig)

  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
  PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
  PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
  PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

  target_link_libraries (${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${SDL2TTF_LIBRARIES})

  if (BUILD_DEB_PACKAGE)
    set (VERSION "0.1.2")
    set (CPACK_PACKAGE_VERSION ${VERSION})
    set (CPACK_GENERATOR "DEB")
    set (CPACK_PACKAGE_NAME "savior_ship")
    set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "An opensource SDL2 game")
    set (CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md")
    set (CPACK_PACKAGE_RELEASE 1)
    set (CPACK_PACKAGE_CONTACT "Lahiru Pathirage")
    set (CPACK_PACKAGE_VENDOR "lpsandaruwan")
    set (CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
    set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    include (CPack)
  endif ()

endif ()
