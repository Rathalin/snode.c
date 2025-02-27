cmake_minimum_required(VERSION 3.18)

find_package(OpenSSL)

if(NOT OpenSSL_FOUND)
    message(
        FATAL_ERROR
            " libssl-dev not found:\n"
            "   libssl-dev is mandatory for SNode.C.\n" "   Thus, install it!"
    )
else(NOT OpenSSL_FOUND)

    set(CORE-SOCKET_STREAM-TLS_CPP ssl_utils.cpp TLSHandshake.cpp
                                   TLSShutdown.cpp system/ssl.cpp
    )

    set(CORE-SOCKET_STREAM-TLS_H
        SocketAcceptor.h
        SocketClient.h
        SocketConnection.h
        SocketConnector.h
        SocketReader.h
        SocketServer.h
        SocketWriter.h
        TLSHandshake.h
        TLSShutdown.h
        ssl_utils.h
        system/ssl.h
    )

    add_library(
        core-socket-stream-tls SHARED ${CORE-SOCKET_STREAM-TLS_CPP}
                                      ${CORE-SOCKET_STREAM-TLS_H}
    )
    add_library(snodec::core-socket-stream-tls ALIAS core-socket-stream-tls)

    target_include_directories(
        core-socket-stream-tls
        PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
               "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
               "$<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIRS}>"
               "$<INSTALL_INTERFACE:include/snode.c>"
    )

    target_link_libraries(
        core-socket-stream-tls PUBLIC core-socket-stream ${OPENSSL_LIBRARIES}
    )

    set_target_properties(
        core-socket-stream-tls
        PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-core-socket-stream-tls
    )

    install(
        TARGETS core-socket-stream-tls
        EXPORT snodec_core-socket-stream-tls_Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/core/socket/stream/tls
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "cmake" EXCLUDE
    )

    install(
        EXPORT snodec_core-socket-stream-tls_Targets
        FILE snodec_core-socket-stream-tls_Targets.cmake
        NAMESPACE snodec::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
    )

endif(NOT OpenSSL_FOUND)
