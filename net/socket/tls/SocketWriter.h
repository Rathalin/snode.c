#ifndef TLS_SOCKETWRITER_H
#define TLS_SOCKETWRITER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>     // for size_t
#include <sys/types.h> // for ssize_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketWriter.h"
#include "socket/tls/Socket.h"

// IWYU pragma: no_forward_declare tls::Socket

namespace tls {

    class SocketWriter : public ::SocketWriter<tls::Socket> {
    protected:
        using ::SocketWriter<tls::Socket>::SocketWriter;

        ssize_t write(const char* junk, size_t junkSize) override;
    };

}; // namespace tls

#endif // TLS_SOCKETWRITER_H
