cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17) 
project(tdd VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(tdd main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/../include)

# ubuntu: sudo apt-get install portaudio19-dev

if (UNIX)
    target_link_libraries(tdd portaudio)
endif (UNIX)

if (WIN32)
    # note: in Linux, we link to and use the system libportaudio
    # For windows, however, we want to use the headers in the ../portaudio
    # directory, and link to the output in ../portaudio/build (statically? Not sure.)
    include_directories(${CMAKE_SOURCE_DIR}/../../portaudio)
    link_directories(${CMAKE_SOURCE_DIR}/../portaudio/build)
endif (WIN32)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
