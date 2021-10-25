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

#include "net/socket/stream/SocketConnection.h"

#include "log/Logger.h"
#include "net/socket/stream/SocketContext.h"
#include "net/socket/stream/SocketContextFactory.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace io::socket::stream {

    SocketConnection::SocketConnection(const std::shared_ptr<SocketContextFactory>& socketContextFactory)
        : socketContext(socketContextFactory->create(this)) {
    }

    SocketContext* SocketConnection::getSocketContext() {
        return socketContext;
    }

    SocketContext* SocketConnection::switchSocketContext(SocketContextFactory* socketContextFactory) {
        SocketContext* newSocketContext = socketContextFactory->create(this);

        if (newSocketContext != nullptr) {
            socketContext->onDisconnected();
            socketContext = newSocketContext;
            socketContext->onConnected();
        } else {
            VLOG(0) << "Switch socket context unsuccessull: new socket context not created";
        }

        return newSocketContext;
    }

} // namespace net::socket::stream
