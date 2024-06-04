@echo off

git submodule update --init

if %ERRORLEVEL% equ 0 (
    goto SUCCESS
) else (
    goto ERROR
)

:SUCCESS
    echo=
    echo Set up success!
    pause
    exit /b 0

:ERROR
    echo=
    echo Set up failed!
    pause
    exit /b 1