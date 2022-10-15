cmake_minimum_required(VERSION 3.16)
project(PHIBAR VERSION 0.1.0 LANGUAGES CXX C)


set(libsource 
    "src/expr.cpp"
    "src/expr.h"
    "src/util.h"
    "src/phibar.cpp"
    "src/phibar.h"
    )

set(source
    ${libsource}
    "src/main.cpp"
)

add_executable(phibar ${source})

if (UNIX)
   target_compile_options(phibar PRIVATE -ggdb )
endif (UNIX)

add_library(phibar_static ${libsource})

if (UNIX)
   target_compile_options(phibar_static PRIVATE -ggdb )
endif (UNIX)


add_subdirectory(tests)


