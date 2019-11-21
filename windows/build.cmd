
:: Windows 10 build script for https://github.com/lpsandaruwan/savior-ship

:: REQUIRES:
:: MS Visual Studio, cmake, git, MSBuild, the Windows 10 SDK, Windows Universal C Runtime
:: MSVC (v142 VS2019 C++ : x64/x86 build tools v14.23 or newer,)
:: MSVC (v142 VS2019 C++ : x64/x86 Spectre-mitigated libs v14.23 or newer) and
:: C++ Core features.

:: Cmake is available from https://cmake.org/
:: Powershell comes preinstalled with Windows 10
:: Git, the Win 10 SDK and the Win universal runtime, etc can all be installed
:: via the individual components section of the MS Visual Studio installer.

:: WARNING: Please ensure that you change the MSVSDIR variable below
:: to reflect the installation path to MS Visual Studio on your system
:: before you run this script for the very first time. Please also
:: check that the file vcruntime140_1.dll exists and is correctly set
:: inside the VCRUN variable. The path may be different on your system.

:: The very first time this script is run, a working net connection
:: is required. After the very first build has successfully
:: completed, subsequent rebuilds no longer require a working
:: connection.

SETLOCAL

:: VS2019 - Variables
::
SET TOOLSET=v142
SET MSVSDIR="D:\Programs\MSVS2019"
SET GENERATOR=-G "Visual Studio 16 2019" -A x64
SET VCRUN=%MSVSDIR%"\VC\Redist\MSVC\14.23.27820\spectre\x64\Microsoft.VC142.CRT\vcruntime140_1.dll"
::
:: End - VS2019 - Variables

:: Internal Variables
::
SET SCRIPTDIR=%CD%
SET CONFIGURATION=Release
SET VCPKGDIR=%SCRIPTDIR%\..\vcpkg
SET BUILDDIR=%SCRIPTDIR%\..\build
::

CD %SCRIPTDIR%\..\

CALL %MSVSDIR%"\VC\Auxiliary\Build\vcvars64.bat"

:pullvcpkg

IF exist %VCPKGDIR% (  GOTO :cleanbuild )

MKDIR %VCPKGDIR% && CD %VCPKGDIR% && git init
git fetch https://github.com/Microsoft/vcpkg master
:: Last update to vcpkg HEAD: 11/10/2019
git merge fa7a3eaec35409d60dbc456d5229b54c192f11de
echo.set(VCPKG_BUILD_TYPE release)>> %VCPKGDIR%\triplets\x64-windows.cmake
CALL .\bootstrap-vcpkg.bat

vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows sdl2-mixer[libvorbis]:x64-windows

CD %SCRIPTDIR%\..\

:cleanbuild

IF exist %BUILDDIR% (  RMDIR /Q /S "%BUILDDIR%" )

MKDIR %BUILDDIR% && CD %BUILDDIR%

cmake .. %GENERATOR% ^
-DCMAKE_BUILD_TYPE="%CONFIGURATION%" ^
-DSDL2_DIR:PATH="%VCPKGDIR%/packages/sdl2_x64-windows/share/sdl2" ^
-DSDL2-IMAGE_DIR:PATH="%VCPKGDIR%/packages/sdl2-image_x64-windows/share/sdl2-image" ^
-DSDL2-TTF_DIR:PATH="%VCPKGDIR%/packages/sdl2-ttf_x64-windows/share/sdl2-ttf" ^
-DSDL2-MIXER_DIR:PATH="%VCPKGDIR%/packages/sdl2-mixer_x64-windows/share/sdl2-mixer"

msbuild /m ".\savior.sln" /p:configuration=%CONFIGURATION% /p:platform=x64 /p:PlatformToolset=%TOOLSET%

COPY %VCRUN% %BUILDDIR%\%CONFIGURATION%

ECHO #
ECHO    Build complete!
ECHO    Successfully built binaries can be found inside the folder \build\%CONFIGURATION%
ECHO #
