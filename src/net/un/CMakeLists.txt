cmake_minimum_required(VERSION 3.18)

set(NET-UN_CPP Socket.cpp SocketAddress.cpp config/ConfigAddress.cpp)

set(NET-UN_H Socket.h SocketAddress.h config/ConfigAddress.h)

add_library(net-un SHARED ${NET-UN_CPP} ${NET-UN_H})
add_library(snodec::net-un ALIAS net-un)

target_link_libraries(net-un PUBLIC snodec::net)

target_include_directories(
    net-un
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<INSTALL_INTERFACE:include/snode.c>"
)

set_target_properties(net-un PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-net-un)

install(
    TARGETS net-un
    EXPORT snodec_net-un_Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/net/un
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "cmake" EXCLUDE
    PATTERN "stream" EXCLUDE
    PATTERN "dgram" EXCLUDE
)

install(
    EXPORT snodec_net-un_Targets
    FILE snodec_net-un_Targets.cmake
    NAMESPACE snodec::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
)

add_subdirectory(stream)
add_subdirectory(dgram)
