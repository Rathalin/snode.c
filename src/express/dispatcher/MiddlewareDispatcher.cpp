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

#include "MiddlewareDispatcher.h"

#include "express/Request.h"
#include "express/dispatcher/MountPoint.h" // for MountPoint
#include "express/dispatcher/RouterDispatcher.h"
#include "express/dispatcher/regex_utils.h"

namespace express::dispatcher {

    class State;

}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace express::dispatcher {

    MiddlewareDispatcher::MiddlewareDispatcher(const std::function<void(Request&, Response&, State&)>& lambda)
        : lambda(lambda) {
    }

    bool MiddlewareDispatcher::dispatch(
        RouterDispatcher* parentRouter, const std::string& parentMountPath, const MountPoint& mountPoint, Request& req, Response& res) {
        bool catched = false;

        std::string absoluteMountPath = path_concat(parentMountPath, mountPoint.relativeMountPath);

        // TODO: Fix regex-match
        if ((req.path.rfind(absoluteMountPath, 0) == 0 && mountPoint.method == "use") ||
            ((absoluteMountPath == req.path || checkForUrlMatch(absoluteMountPath, req.url)) &&
             (req.method == mountPoint.method || mountPoint.method == "all"))) {
            catched = true;

            if (hasResult(absoluteMountPath)) {
                setParams(absoluteMountPath, req);
            }

            lambda(req, res, parentRouter->getState());
        }

        return catched;
    }

} // namespace express::dispatcher
