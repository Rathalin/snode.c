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

#ifndef NET_CONFIG_CONFIGADDRESSREMOTE_H
#define NET_CONFIG_CONFIGADDRESSREMOTE_H

#include "net/config/ConfigAddress.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace CLI {
    class App;
    class Option;
} // namespace CLI

#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace net::config {

    template <typename SocketAddressT>
    class ConfigAddressRemote : public net::config::ConfigAddress<SocketAddressT> {
        using Super = net::config::ConfigAddress<SocketAddressT>;

        using SocketAddress = SocketAddressT;

    protected:
        explicit ConfigAddressRemote(CLI::App* baseSc);

    public:
        const SocketAddress& getRemoteAddress();
        void setRemoteAddress(const SocketAddress& localAddress);
    };

} // namespace net::config

#endif // NET_CONFIG_CONFIGADDRESSREMOTE_H
