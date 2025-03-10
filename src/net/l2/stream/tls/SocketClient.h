/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
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

#ifndef NET_L2_STREAM_LEGACY_SOCKETCLIENT_H
#define NET_L2_STREAM_LEGACY_SOCKETCLIENT_H

#include "core/socket/stream/tls/SocketClient.h"         // IWYU pragma: export
#include "net/l2/stream/SocketClient.h"                  // IWYU pragma: export
#include "net/l2/stream/tls/config/ConfigSocketClient.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::l2::stream::tls {

    template <typename SocketContextFactoryT>
    using SocketClient =
        core::socket::stream::tls::SocketClient<net::l2::stream::SocketClient<net::l2::stream::tls::config::ConfigSocketClient>,
                                                SocketContextFactoryT>;

} // namespace net::l2::stream::tls

#endif // NET_L2_STREAM_LEGACY_SOCKETCLIENT_H
