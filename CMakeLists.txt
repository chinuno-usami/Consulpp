CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

PROJECT(consulpp CXX C)
SET(CONSULPP_VERSION_MAJOR 0)
SET(CONSULPP_VERSION_MINOR 2)

#check c++11 standard
if(UNIX)
    #linux
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -DDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O3")

include_directories(${CPPREST_DIR})
link_directories(${CPPREST_LIB})

if(BUILD_TEST)
    add_executable(test_client ${PROJECT_SOURCE_DIR}/test.cpp)
    target_link_libraries(
        test_client
        cpprest
        boost_system
        ssl
        crypto)
else()
    message("Single header, do not need build, enjoy.")
endif()
