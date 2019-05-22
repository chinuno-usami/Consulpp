PROJECT(consulpp CXX C)
SET(CONSULPP_VERSION_MAJOR 0)
SET(CONSULPP_VERSION_MINOR 1)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -DDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O3")

if(BUILD_TEST)
    add_executable(test_client ${PROJECT_SOURCE_DIR}/test.cpp)
    target_link_libraries(
        test_client
        cpprest
        boost_system
        ssl
        crypto)
endif()