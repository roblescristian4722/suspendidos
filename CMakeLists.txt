cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

# Nombre del proyecto y versión (la versión corresponde al número de programa
# a entregar para la materia)
project(procesamientoPorLotes VERSION 5.0)

# Incluye el directorio de headers
include_directories(define)

set(SOURCE src/main.cpp
            src/cursor.cpp
            src/process.cpp
            src/processManager.cpp)


# Añandir el ejecutable
add_executable(procesamientoPorLotes ${SOURCE})
