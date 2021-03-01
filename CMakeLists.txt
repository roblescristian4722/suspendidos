cmake_minimum_required(VERSION 3.0)

# Nombre del proyecto y versión (la versión corresponde al número de programa
# a entregar para la materia)
project(procesamientoPorLotes VERSION 1.0)

# Incluye el directorio de headers
include_directories(define)

set(SOURCE src/main.cpp
            src/cursor.cpp
            src/lote.cpp
            src/procesadorLotes.cpp
            src/proceso.cpp)


# Añandir el ejecutable
add_executable(procesamientoPorLotes ${SOURCE})
