@echo off
setlocal

echo ======================================
echo   Warcraft Engine - Quick Build
echo ======================================
echo.

REM Clean old build (DISABLED - use for full clean)
REM echo [1/4] Cleaning old build...
REM if exist build (
REM     rmdir /s /q build
REM     echo       Done
REM ) else (
REM     echo       No build folder found
REM )
REM echo.

REM Configure
echo [1/3] Configuring with CMake...
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
echo   Executable: build\Debug\warcraft.exe
echo ======================================
echo.

choice /C YN /M "Run the game now"
if %ERRORLEVEL% EQU 1 (
    echo.
    echo Starting game...
    build\warcraft.exe
)
