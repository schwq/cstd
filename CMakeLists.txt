cmake_minimum_required(VERSION 3.9)
project(cstd VERSION 1.0.0 DESCRIPTION "C standard library, recreating c++ concepts")
set(CMAKE_BUILD_TYPE Release)
set(C_STANDARD 17)

include(GNUInstallDirs)

set(header 
    include/benchmark.h 
    include/c_string.h 
    include/cmap.h 
    include/common.h 
    include/cstd.h 
    include/dynamic_list.h 
    include/lmath.h 
    include/matrix.h 
    include/memory.h 
    include/smart_ptr.h 
    include/exception.h 
)

set(source 
    source/benchmark.c 
    source/c_string.c 
    source/cmap.c 
    source/dynamic_list.c 
    source/lmath.c 
    source/matrix.c 
    source/memory.c 
    source/smart_ptr.c 
    source/exception.c
)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(include)

add_library(cstd SHARED ${source})

set_target_properties(cstd PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cstd PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(cstd PROPERTIES PUBLIC_HEADER include/cstd.h)

target_include_directories(cstd PRIVATE include)

target_link_libraries(cstd m)

add_definitions(-w -g -std=c17)

install(TARGETS cstd LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cstd)


