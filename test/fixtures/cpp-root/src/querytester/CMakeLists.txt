set(target querytester)

set(sources
    main.cpp
)

add_executable(${target} ${sources})
add_executable(${project}::${target} ALIAS ${target})

target_include_directories(${target}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SQLAPI_INCLUDE}
    ${Boost_INCLUDE_DIRS}
    ${GEMFIRE_INCLUDE}
    ${COMMON_INCLUDES}
)

target_link_libraries(${target}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_IOSTREAMS_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${GEMFIRE_NATIVE_LIB_DIR}/libcryptoImpl.so
    ${GEMFIRE_NATIVE_LIB_DIR}/libpivotal-gemfire.so
)

target_link_libraries(${target} debug ${SQLAPI_LIBRARY_DEBUG} optimized ${SQLAPI_LIBRARY})

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)

