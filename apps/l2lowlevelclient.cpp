/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "core/SNodeC.h"
#include "io/socket/bluetooth/address/L2CapAddress.h" // for L2CapAddress
#include "io/socket/bluetooth/l2cap/Socket.h"         // for l2cap
#include "io/socket/bluetooth/l2cap/SocketClient.h"
#include "io/socket/stream/SocketClient.h" // for SocketClient<...
#include "io/socket/stream/SocketContext.h"
#include "io/socket/stream/SocketContextFactory.h"
#include "log/Logger.h"

namespace io::socket::stream {
    class SocketConnection;
} // namespace io::socket::stream

#include <cstddef>
#include <functional>  // for function
#include <string>      // for string, alloc...
#include <sys/types.h> // for ssize_t

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace io::socket::bluetooth::l2cap;

class SimpleSocketProtocol : public io::socket::stream::SocketContext {
public:
    explicit SimpleSocketProtocol(io::socket::stream::SocketConnection* socketConnection)
        : io::socket::stream::SocketContext(socketConnection) {
    }

    void onReceiveFromPeer() override {
        char junk[4096];

        ssize_t ret = readFromPeer(junk, 4096);

        if (ret > 0) {
            std::size_t junklen = static_cast<std::size_t>(ret);
            VLOG(0) << "Data to reflect: " << std::string(junk, junklen);
            sendToPeer(junk, junklen);
        }
    }

    void onWriteError(int errnum) override {
        VLOG(0) << "OnWriteError: " << errnum;
    }

    void onReadError(int errnum) override {
        VLOG(0) << "OnReadError: " << errnum;
    }
};

class SimpleSocketProtocolFactory : public io::socket::stream::SocketContextFactory {
public:
    io::socket::stream::SocketContext* create(io::socket::stream::SocketConnection* socketConnection) override {
        return new SimpleSocketProtocol(socketConnection);
    }
};

SocketClient<SimpleSocketProtocolFactory> getClient() {
    SocketClient<SimpleSocketProtocolFactory> client(
        [](const SocketClient<SimpleSocketProtocolFactory>::SocketAddress& localAddress,
           const SocketClient<SimpleSocketProtocolFactory>::SocketAddress& remoteAddress) -> void { // OnConnect
            VLOG(0) << "OnConnect";

            VLOG(0) << "\tServer: " + remoteAddress.toString();
            VLOG(0) << "\tClient: " + localAddress.toString();
        },
        [](SocketClient<SimpleSocketProtocolFactory>::SocketConnection* socketConnection) -> void { // onConnected
            VLOG(0) << "OnConnected";

            socketConnection->sendToPeer("Hello rfcomm connection!");
        },
        [](SocketClient<SimpleSocketProtocolFactory>::SocketConnection* socketConnection) -> void { // onDisconnect
            VLOG(0) << "OnDisconnect";

            VLOG(0) << "\tServer: " + socketConnection->getRemoteAddress().toString();
            VLOG(0) << "\tClient: " + socketConnection->getLocalAddress().toString();
        },
        {{}});

    return client;
}

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    {
        SocketClient<SimpleSocketProtocolFactory>::SocketAddress remoteAddress("A4:B1:C1:2C:82:37", 0x1023); // titan
        SocketClient<SimpleSocketProtocolFactory>::SocketAddress bindAddress("44:01:BB:A3:63:32");           // mpow

        SocketClient client = getClient();

        client.connect(remoteAddress, bindAddress, [](int err) -> void {
            if (err) {
                PLOG(ERROR) << "Connect: " << std::to_string(err);
            } else {
                VLOG(0) << "Connected";
            }
        });
    }

    return core::SNodeC::start();
}
