cmake_minimum_required(VERSION 3.16)
project(TESTS VERSION 0.1.0 LANGUAGES CXX C)

set(source 
    "tests.cpp")


add_executable(phibar_tests ${source})

if (UNIX)
   target_compile_options(phibar_tests PRIVATE -ggdb )
endif (UNIX)

target_include_directories(phibar_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(phibar_tests PRIVATE phibar_static)
