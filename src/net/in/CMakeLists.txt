cmake_minimum_required(VERSION 3.18)

set(NET-IN_CPP Socket.cpp SocketAddress.cpp config/ConfigAddress.cpp)

set(NET-IN_H Socket.h SocketAddress.h config/ConfigAddress.h)

add_library(net-in SHARED ${NET-IN_CPP} ${NET-IN_H})
add_library(snodec::net-in ALIAS net-in)

target_link_libraries(net-in PUBLIC snodec::net)

target_include_directories(
    net-in
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<INSTALL_INTERFACE:include/snode.c>"
)

set_target_properties(net-in PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-net-in)

install(
    TARGETS net-in
    EXPORT snodec_net-in_Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/net/in
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "cmake" EXCLUDE
    PATTERN "stream" EXCLUDE
    PATTERN "dgram" EXCLUDE
)

install(
    EXPORT snodec_net-in_Targets
    FILE snodec_net-in_Targets.cmake
    NAMESPACE snodec::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
)

add_subdirectory(stream)
