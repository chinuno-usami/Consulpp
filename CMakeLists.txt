PROJECT(consulpp CXX C)
SET(CONSULPP_VERSION_MAJOR 0)
SET(CONSULPP_VERSION_MINOR 1)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -DDEBUG")

aux_source_directory(./src/ DIR_SRCS)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CPPREST_DIR})

link_directories(${PROJECT_SOURCE_DIR}/src)

if(BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})
else()
    add_library(${PROJECT_NAME} STATIC ${DIR_SRCS})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "Consulpp.h;ConsulService.h;Check.h")

if(BUILD_TEST)
    add_executable(test_client ${PROJECT_SOURCE_DIR}/test.cpp)
    target_link_libraries(
        test_client
        ${PROJECT_NAME}
        cpprest
        boost_system
        ssl
        crypto)
endif()

#install(TARGETS ${PROJECT_NAME}
#    LIBRARY DESTINATION lib/
#    PUBLIC_HEADER DESTINATION include/Consulpp/)

#include (InstallRequiredSystemLibraries)
#set (CPACK_RESOURCE_FILE_LICENSE
#    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
#set (CPACK_PACKAGE_VERSION_MAJOR "${CONSULPP_VERSION_MAJOR}")
#set (CPACK_PACKAGE_VERSION_MINOR "${CONSULPP_VERSION_MINOR}")
#include (CPack)
