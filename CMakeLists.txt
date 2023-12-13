# Building with CMake version 3.15 or higher.
cmake_minimum_required(VERSION 3.15)

message(STATUS " ... STARTING ...")

set(PROJECT_NAME
    "Advent"
)

project(${PROJECT_NAME}
  LANGUAGES C CXX
  VERSION "01.00"
)

find_package (Threads)

set(PROJECT_SOURCE_FILES)
set(PROJECT_HEADER_FILES)

include(${CMAKE_CURRENT_LIST_DIR}/src/CMakeLists.txt)

set(LOCAL_INCLUDE_DIRS
      ${CMAKE_CURRENT_LIST_DIR}/include
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADER_FILES}
    ${PROJECT_SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME}
      PROPERTIES
        CXX_EXTENSIONS OFF
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${LOCAL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)

target_compile_options(${PROJECT_NAME}
    PUBLIC -Wall
    $<$<CONFIG:Debug>:
      -DVERBOSE_LEVEL=2 -O0 -g3
    >
)

install(TARGETS ${PROJECT_NAME}
      DESTINATION bin
)