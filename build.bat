@echo off
setlocal

echo ======================================
echo   Warcraft Engine - Quick Build
echo ======================================
echo.

REM Add MSYS2 UCRT64 to PATH if not already present
set "UCRT64=C:\msys64\ucrt64\bin"
echo %PATH% | find /i "%UCRT64%" >nul || set "PATH=%UCRT64%;%PATH%"

REM Verify compiler
where gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] GCC not found. Install MSYS2 and run:
    echo   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake
    pause
    exit /b 1
)

REM Configure
echo [1/3] Configuring with CMake...
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_POLICY_VERSION_MINIMUM=3.5
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)
echo       Done
echo.

REM Build
echo [2/3] Building project...
cmake --build build
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b 1
)
echo       Done
echo.

REM Success
echo [3/3] Build completed successfully!
echo.
echo ======================================
echo   Executable: build\warcraft.exe
echo ======================================
echo.

choice /C YN /M "Run the game now"
if %ERRORLEVEL% EQU 1 (
    echo.
    echo Starting game...
    build\warcraft.exe
)
