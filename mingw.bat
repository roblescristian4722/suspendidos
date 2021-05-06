@echo off

mkdir build
g++ src/*.cpp -static -static-libgcc -static-libstdc++ -B define -o build\process.exe
cd build
