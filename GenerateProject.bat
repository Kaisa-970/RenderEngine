@echo off

set ROOT_DIR=%cd%
echo ROOT_DIR is %ROOT_DIR%
cmake -B build -DCMAKE_BUILD_TYPE=Debug

if %ERRORLEVEL% equ 0 (
    goto SUCCESS
) else (
    goto ERROR
)

:SUCCESS
    echo=
    echo Generate project to %ROOT_DIR%\build
    pause
    exit /b 0

:ERROR
    echo=
    echo Generate project failed!
    pause
    exit /b 1