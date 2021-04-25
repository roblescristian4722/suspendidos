@echo off

mkdir build
g++ src/*.cpp -B define -o build\process.exe
cd build
