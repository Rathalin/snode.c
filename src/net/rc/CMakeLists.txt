cmake_minimum_required(VERSION 3.18)

find_package(PkgConfig REQUIRED)

pkg_check_modules(BLUEZ bluez)

if(BLUEZ_FOUND)
    set(NET-RC_CPP Socket.cpp SocketAddress.cpp config/ConfigAddress.cpp)

    set(NET-RC_H Socket.h SocketAddress.h config/ConfigAddress.h)

    add_library(net-rc SHARED ${NET-RC_CPP} ${NET-RC_H})
    add_library(snodec::net-rc ALIAS net-rc)

    target_link_libraries(net-rc PUBLIC snodec::net ${BLUEZ_LIBRARIES})

    target_include_directories(
        net-rc
        PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
               "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
               "$<INSTALL_INTERFACE:include/snode.c>"
    )

    set_target_properties(
        net-rc PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-net-rc
    )

    install(
        TARGETS net-rc
        EXPORT snodec_net-rc_Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/net/rc
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "cmake" EXCLUDE
        PATTERN "stream" EXCLUDE
        PATTERN "dgram" EXCLUDE
    )

    install(
        EXPORT snodec_net-rc_Targets
        FILE snodec_net-rc_Targets.cmake
        NAMESPACE snodec::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
    )

    add_subdirectory(stream)

else(BLUEZ_FOUND)
    message(
        WARNING
            " libbluetooth-dev not found:\n"
            "    If you want to get bluetooth support you should install libbluetooth-dev."
    )
endif(BLUEZ_FOUND)
