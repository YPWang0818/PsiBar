cmake_minimum_required(VERSION 3.16)
project(TESTS VERSION 0.1.0 LANGUAGES CXX C)

set(source 
    "tests.cpp")


add_executable(psibar_tests ${source})

if (UNIX)
   target_compile_options(psibar_tests PRIVATE -ggdb )
endif (UNIX)

target_include_directories(psibar_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(psibar_tests PRIVATE psibar_static)
