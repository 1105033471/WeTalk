@echo off

set "batch_dir=%~dp0"

if not exist %batch_dir%/build (
    md build
)
cd /d build

cmake .. -G "MinGW Makefiles" && make -j16
