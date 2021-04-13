@echo off

mkdir build
g++ src\* -B define -o build\process.exe
cd build
