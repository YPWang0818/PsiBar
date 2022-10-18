cmake_minimum_required(VERSION 3.16)
project(PSIBAR VERSION 0.1.0 LANGUAGES CXX C)


set(libsource 
    "src/expr.cpp"
    "src/expr.h"
    "src/util.h"
    "src/psibar.cpp"
    "src/psibar.h"
    "src/logger.h"
    "src/logger.cpp"
    )

set(source
    ${libsource}
    "src/main.cpp"
)

add_executable(psibar ${source})

if (UNIX)
   target_compile_options(psibar PRIVATE -ggdb )
endif (UNIX)

add_library(psibar_static ${libsource})

if (UNIX)
   target_compile_options(psibar_static PRIVATE -ggdb )
endif (UNIX)


add_subdirectory(tests)


