# CMake version
cmake_minimum_required(VERSION 3.8)


SET(EXTERNALS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

include(WriteCompilerDetectionHeader OPTIONAL RESULT_VARIABLE WriterCompilerDetectionHeaderFound)

IF(!WIN32)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
ENDIF()

# Meta information about the project
set(META_PROJECT_NAME        "RQE_POC")

# Declare project
project(${META_PROJECT_NAME} C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Get project name
set(project ${META_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


# Find SQLAPI++
find_library(SQLAPI_LIBRARY sqlapi HINTS "${EXTERNALS_DIR}/SQLAPI/lib")
find_library(SQLAPI_LIBRARY_DEBUG sqlapid HINTS "${EXTERNALS_DIR}/SQLAPI/lib")
find_path(SQLAPI_INCLUDE 
        NAMES "SQLAPI.h" 
        PATHS "${EXTERNALS_DIR}/SQLAPI/include"
    )

if(NOT SQLAPI_LIBRARY)
    message(FATAL_ERROR "SQLAPI++ library not found")
endif()

if(NOT SQLAPI_INCLUDE)
    message(FATAL_ERROR "SQLAPI++ includes not found")
endif()

message(STATUS "SQLAPI++ Include: ${SQLAPI_INCLUDE}")
message(STATUS "SQLAPI++ Lib: ${SQLAPI_LIBRARY}")
message(STATUS "SQLAPI++ Debug Lib: ${SQLAPI_LIBRARY_DEBUG}")


# Find GemFire Native Client
set(GEMFIRE_NATIVE_LIB_DIR "${EXTERNALS_DIR}/geode-nativeclient-9.1/lib")
set(GEMFIRE_INCLUDE "${EXTERNALS_DIR}/geode-nativeclient-9.1/include")

message(STATUS "GemFire Include: ${GEMFIRE_INCLUDE}")
message(STATUS "GemFire Libs: ${GEMFIRE_NATIVE_LIB_DIR}")

set(COMMON_INCLUDES "${EXTERNALS_DIR}/include")


find_package(Boost 1.60.0 COMPONENTS system filesystem iostreams program_options date_time regex thread log chrono REQUIRED)


add_subdirectory(src)

# add_subdirectory(querytester)

