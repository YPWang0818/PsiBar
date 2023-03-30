cmake_minimum_required(VERSION 3.16)
project(PSIBAR VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

set(libsource 
    "src/expr.cpp"
    "src/expr.h"
    "src/util.h"
    "src/psibar.cpp"
    "src/psibar.h"
    "src/logger.h"
    "src/logger.cpp"
    "src/parser.h"
    "src/parser.cpp"
    )

set(source
    ${libsource}
    "src/main.h"
    "src/main.cpp"
)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)

find_package(Boost 1.36.0 REQUIRED COMPONENTS regex )

add_executable(psibar ${source})

target_include_directories(psibar PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(psibar ${Boost_LIBRARIES} )

if (UNIX)
   target_compile_options(psibar PRIVATE -ggdb )
endif (UNIX)

add_library(psibar_static ${libsource})
target_include_directories(psibar_static PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(psibar_static ${Boost_LIBRARIES} )

if (UNIX)
   target_compile_options(psibar_static PRIVATE -ggdb )
endif (UNIX)


add_subdirectory(tests)


