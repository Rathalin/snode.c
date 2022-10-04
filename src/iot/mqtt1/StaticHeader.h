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

#include "iot/mqtt1/types/UInt8.h"
#include "iot/mqtt1/types/UIntV.h"

namespace iot::mqtt1 {
    class SocketContext; // IWYU pragma: keep
}

#ifndef IOT_MQTT1_STATICHEADER_H
#define IOT_MQTT1_STATICHEADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstddef>
#include <cstdint>
#include <vector>

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt1 {

    class StaticHeader {
    public:
        StaticHeader();
        StaticHeader(uint8_t packetType, uint8_t reserved, uint32_t remainingLength = 0);

        ~StaticHeader();

        std::size_t deserialize(iot::mqtt1::SocketContext* socketContext);
        std::vector<char> serialize();

        uint8_t getPacketType() const;
        uint8_t getFlags() const;

        void setRemainingLength(uint32_t remainingLength);
        uint32_t getRemainingLength() const;

        bool isComplete() const;
        bool isError() const;

        void reset();

    private:
        types::UInt8 typeFlags;
        types::UIntV remainingLength;

        bool complete = false;
        bool error = false;

        int state = 0;
    };

} // namespace iot::mqtt1

#endif // IOT_MQTT1_STATICHEADER_H
