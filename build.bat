@echo off
setlocal

echo ======================================
echo   Warcraft Engine - Clean Build
echo ======================================
echo.

REM Clean old build
echo [1/4] Cleaning old build...
if exist build (
    rmdir /s /q build
    echo       Done
) else (
    echo       No build folder found
)
echo.

REM Configure
echo [2/4] Configuring with CMake...
cmake -B build
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)
echo       Done
echo.

REM Build
echo [3/4] Building project...
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
echo [4/4] Build completed successfully!
echo.
echo ======================================
echo   Executable: build\Debug\warcraft.exe
echo ======================================
echo.

choice /C YN /M "Run the game now"
if %ERRORLEVEL% EQU 1 (
    echo.
    echo Starting game...
    build\warcraft.exe
)
