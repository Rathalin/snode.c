/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
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

#ifndef FILEREADER_H
#define FILEREADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "File.h"
#include "ReadEventReceiver.h"
#include "streams/ReadStream.h"

namespace net::stream {
    class WriteStream;
}

class FileReader
    : public net::ReadEventReceiver
    , public net::stream::ReadStream
    , virtual public File {
protected:
    FileReader(int fd, net::stream::WriteStream& writeStream);

public:
    static FileReader* pipe(const std::string& path, net::stream::WriteStream& writeStream, const std::function<void(int err)>& onError);

    void readEvent() override;

private:
    void unobserved() override;
};

#endif // FILEREADER_H
