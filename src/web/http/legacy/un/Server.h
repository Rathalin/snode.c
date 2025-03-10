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

#ifndef WEB_HTTP_LEGACY_UN_SERVER_H
#define WEB_HTTP_LEGACY_UN_SERVER_H

#include "net/un/stream/legacy/SocketServer.h" // IWYU pragma: export
#include "web/http/server/Server.h"            // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::http::legacy::un {

    template <typename Request, typename Response>
    class Server : public web::http::server::Server<net::un::stream::legacy::SocketServer, Request, Response> {
        using Super = web::http::server::Server<net::un::stream::legacy::SocketServer, Request, Response>;
        using Super::Super;

    public:
        using SocketAddress = typename Super::SocketAddress;
        using SocketConnection = typename Super::SocketConnection;

        using web::http::server::Server<net::un::stream::legacy::SocketServer, Request, Response>::listen;

        void listen(const std::string& sunPath, const std::function<void(const SocketAddress&, int)>& onError) {
            listen(sunPath, LISTEN_BACKLOG, onError);
        }
    };

} // namespace web::http::legacy::un

#endif // WEB_HTTP_LEGACY_UN_SERVER_H
