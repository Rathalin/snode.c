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

#include "net/un/dgram/PeerSocket.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::un::dgram {

    template <typename Config>
    PeerSocket<Config>::PeerSocket(const std::string& name)
        : Super(name) {
    }

    template <typename Config>
    void PeerSocket<Config>::connect(const std::string& sunPath, const std::function<void(const SocketAddress&, int)>& onError) {
        connect(SocketAddress(sunPath), onError);
    }

    template <typename Config>
    void PeerSocket<Config>::connect(const std::string& remoteSunPath,
                                     const std::string& localSunPath,
                                     const std::function<void(const SocketAddress&, int)>& onError) {
        connect(SocketAddress(remoteSunPath), SocketAddress(localSunPath), onError);
    }

} // namespace net::un::dgram
