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

#ifndef NET_IN6_CONFIGLOCAL_H
#define NET_IN6_CONFIGLOCAL_H

#include "net/ConfigLocal.h"
#include "net/in6/SocketAddress.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstdint> // for uint16_t
#include <string>  // for string

namespace CLI {
    class App;
    class Option;
} // namespace CLI

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::in6 {

    class ConfigLocal : public net::ConfigLocal<SocketAddress> {
    public:
        explicit ConfigLocal(CLI::App* baseSc);

    protected:
        void required() const;

        CLI::App* bindSc = nullptr;
        CLI::Option* bindHostOpt = nullptr;
        CLI::Option* bindPortOpt = nullptr;

    private:
        SocketAddress getAddress() const override;

        std::string bindHost = "";
        uint16_t bindPort = 0;
    };

} // namespace net::in6

#endif // NET_IN6_CONFIGLOCAL_H
