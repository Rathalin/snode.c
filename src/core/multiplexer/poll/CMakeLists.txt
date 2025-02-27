cmake_minimum_required(VERSION 3.18)

set(MUX_POLL_CPP DescriptorEventPublisher.cpp EventMultiplexer.cpp)

set(MUX_POLL_H DescriptorEventPublisher.h EventMultiplexer.h)

add_library(mux-poll SHARED ${MUX_POLL_CPP} ${MUX_POLL_H})
add_library(snodec::mux-poll ALIAS mux-poll)

target_include_directories(
    mux-poll
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<INSTALL_INTERFACE:include/snode.c>"
)

set_target_properties(
    mux-poll PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-mux-poll
)

install(
    TARGETS mux-poll
    EXPORT snodec_mux-poll_Targets
    LIBRARY DESTINATION ${CMAKE_INISTALL_LIBDIR}
)

install(
    EXPORT snodec_mux-poll_Targets
    FILE snodec_mux-poll_Targets.cmake
    NAMESPACE snodec::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
)
