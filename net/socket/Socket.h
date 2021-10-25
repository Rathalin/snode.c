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

#ifndef NET_SOCKET_SOCKET_H
#define NET_SOCKET_SOCKET_H

#include "net/Descriptor.h"
#include "net/system/socket.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cerrno>
#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace io::socket {

    template <typename SocketAddressT>
    class Socket : virtual public io::Descriptor {
    public:
        using SocketAddress = SocketAddressT;

        Socket() = default;

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

    protected:
        virtual int create(int flags) = 0;

    public:
        void open(const std::function<void(int)>& onError, int flags = 0) {
            int fd = create(flags);

            if (fd >= 0) {
                attach(fd);
                onError(0);
            } else {
                onError(errno);
            }
        }

        void bind(const SocketAddress& bindAddress, const std::function<void(int)>& onError) {
            int ret = io::system::bind(getFd(), &bindAddress.getSockAddr(), sizeof(typename SocketAddress::SockAddr));

            if (ret < 0) {
                onError(errno);
            } else {
                this->bindAddress = bindAddress;
                onError(0);
            }
        }

        enum shutdown { WR = SHUT_WR, RD = SHUT_RD, RDWR = SHUT_RDWR };

        void shutdown(shutdown how) {
            io::system::shutdown(getFd(), how);
        }

        const SocketAddress& getBindAddress() const {
            return bindAddress;
        }

    protected:
        SocketAddress bindAddress{};
    };

} // namespace io::socket

#endif // NET_SOCKET_SOCKET_H
