@echo off
cd build
erase *
rmdir /s CMakeFiles
cmake -G "Unix Makefiles" ..\.
make
