cmake_minimum_required(VERSION 3.6)
project(Blatt_7)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES mmSerial.c)
add_executable(mmSerial ${SOURCE_FILES})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
set(SOURCE_FILES mmParallel.c)
set_source_files_properties(${SOURCE_FILES}  PROPERTIES COMPILE_FLAGS    "-fopenmp")
add_executable(mmParallel ${SOURCE_FILES})