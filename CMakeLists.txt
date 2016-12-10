cmake_minimum_required(VERSION 3.6)
project(PP)

#set(PROJECT_BINARY_DIR  Blatt_6/out)
#
#set(SOURCE_FILES    "Blatt_6/gauss5x5.c")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c99")

set(SOURCE_FILES    "Blatt_6/pocLoad.cpp")
set_source_files_properties(${SOURCE_FILES}  PROPERTIES COMPILE_FLAGS    "-mavx -march=native")
add_executable(poc ${SOURCE_FILES})

set(PROJECT_NAME gauss5x5_AVXonly)
set(SOURCE_FILES    "Blatt_6/${PROJECT_NAME}")
set_source_files_properties(${SOURCE_FILES}  PROPERTIES COMPILE_FLAGS    "-mavx -march=native")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(PROJECT_NAME gauss5x5orig)
set(SOURCE_FILES    "Blatt_6/${PROJECT_NAME}.c")
set_source_files_properties(${SOURCE_FILES}  PROPERTIES COMPILE_FLAGS    "")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(PROJECT_NAME andi)
set(SOURCE_FILES    "Blatt_6/${PROJECT_NAME}.c")
set_source_files_properties(${SOURCE_FILES}  PROPERTIES COMPILE_FLAGS    "-mavx -march=native ")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(PROJECT_NAME gauss5x5_AVX+OMP)
set(SOURCE_FILES    "Blatt_6/${PROJECT_NAME}.c")
set_source_files_properties(${SOURCE_FILES}  PROPERTIES COMPILE_FLAGS    "-mavx -march=native -DTHREAD_COUNT=8 -DRUN_COUNT=20 -O3")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})