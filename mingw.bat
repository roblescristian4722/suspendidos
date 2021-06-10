@echo off

mkdir build
g++ src/main.cpp src/cursor.cpp src/process.cpp src/controller.cpp src/processManager.cpp -static -static-libgcc -static-libstdc++ -B define -o build\process.exe
cd build
