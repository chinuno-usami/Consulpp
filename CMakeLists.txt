PROJECT(consulpp CXX C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O3 -mcmodel=small -lc -Wno-unused-result")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -DDEBUG")

set(DIR_SRCS
${PROJECT_SOURCE_DIR}/Check.cpp
${PROJECT_SOURCE_DIR}/Consulpp.cpp
${PROJECT_SOURCE_DIR}/ConsulService.cpp
)


include_directories(
	${PROJECT_SOURCE_DIR}/
)

link_directories(
	${PROJECT_SOURCE_DIR}/
)

add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "Consulpp.h;ConsulService.h;Check.h")
add_executable(test_client ${PROJECT_SOURCE_DIR}/test.cpp)
target_link_libraries(
    test_client
    ${PROJECT_NAME}
    cpprest
    boost_system
    ssl
    crypto
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib/
    PUBLIC_HEADER DESTINATION include/Consulpp/
)

