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

#ifndef EXPRESS_DISPATCHER_ROUTE_H
#define EXPRESS_DISPATCHER_ROUTE_H

#include "express/dispatcher/MountPoint.h"

namespace express {

    class Request;
    class Response;

    namespace dispatcher {

        class Dispatcher;
        class RouterDispatcher;

    } // namespace dispatcher

} // namespace express

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <memory>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace express::dispatcher {

    class Route {
    public:
        Route(RouterDispatcher* parentRouter,
              const std::string& method,
              const std::string& relativeMountPath,
              const std::shared_ptr<Dispatcher>& dispatcher);

        bool dispatch(const std::string& parentMountPath, Request& req, Response& res);

    protected:
        RouterDispatcher* parentRouter;
        MountPoint mountPoint;
        std::shared_ptr<Dispatcher> dispatcher;
    };

} // namespace express::dispatcher

#endif // EXPRESS_DISPATCHER_ROUTE_H
