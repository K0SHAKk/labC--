@echo off
cd /d "%~dp0"
if exist windows\physics.exe (
    windows\physics.exe windows\data\elephant.txt
) else if exist physics.exe (
    physics.exe data\elephant.txt
) else if exist build\physics.exe (
    build\physics.exe data\elephant.txt
) else (
    build\Release\physics.exe data\elephant.txt
)
if errorlevel 1 pause
