cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17) 
project(tdd VERSION 0.1.0)
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include(CTest)
enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tdd main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/../include)

# ubuntu: sudo apt-get install portaudio19-dev

# For using system portaudio-dev 
#if (UNIX)
#    target_link_libraries(tdd portaudio)
#endif (UNIX)


include_directories(${CMAKE_SOURCE_DIR}/../../portaudio)
link_directories(${CMAKE_SOURCE_DIR}/../portaudio/build)

# This for static link to portaudio in Linux
if (UNIX)
    target_link_libraries(tdd ${CMAKE_SOURCE_DIR}/../../portaudio/build/libportaudio.a pthread jack asound)
endif (UNIX)




#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
