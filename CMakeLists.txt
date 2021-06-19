cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(GCC_COMPILE_FLAGS -Wall
                      -static
                      -g)
# set(GCC_LINK_FLAGS)

# Nombre del proyecto y versión (la versión corresponde al número de programa
# a entregar para la materia)
project(suspendidos VERSION 8.0)

# Incluye el directorio de headers
include_directories(define)

set(SOURCE src/main.cpp
            src/cursor.cpp
            src/process.cpp
            src/processManager.cpp
            src/kbhitLinux.cpp
            src/controller.cpp)


# Añandir el ejecutable
add_definitions(${GCC_COMPILE_FLAGS})
add_executable(${PROJECT_NAME} ${SOURCE})
#target_link_libraries(${PROJECT_NAME} GCC_LINK_FLAGS)
