cmake_minimum_required(VERSION 3.16)

project(Homework12 VERSION 1.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(stats STATIC stats.cpp)
target_include_directories(stats PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(
    stats
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

function(configure_binary target_name)
    set_target_properties(
        ${target_name}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    target_link_libraries(${target_name} PRIVATE stats)
endfunction()

add_executable(mapper mapper.cpp)
configure_binary(mapper)

add_executable(reducer reducer.cpp)
configure_binary(reducer)

find_package(Threads REQUIRED)

add_executable(tests tests/test_stats.cpp)
configure_binary(tests)
target_link_libraries(tests PRIVATE Threads::Threads)
target_include_directories(tests PRIVATE tests)

include(GNUInstallDirs)

install(TARGETS mapper reducer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS stats
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES stats.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_PACKAGE_NAME "homework12")
set(CPACK_PACKAGE_VENDOR "OTUS")
set(CPACK_PACKAGE_CONTACT "savchenko.mn.dev@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_GENERATOR "DEB")

include(CPack)
