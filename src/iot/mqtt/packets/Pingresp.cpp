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

#include "iot/mqtt/packets/Pingresp.h"

#include "iot/mqtt/SocketContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt::packets {

    Pingresp::Pingresp()
        : iot::mqtt::ControlPacket(MQTT_PINGRESP, MQTT_PINGRESP_FLAGS) {
    }

    Pingresp::Pingresp(uint32_t remainingLength, uint8_t flags)
        : iot::mqtt::ControlPacket(MQTT_PINGRESP, flags, remainingLength, MQTT_PINGRESP_FLAGS) {
    }

    std::vector<char> Pingresp::serializeVP() const {
        return std::vector<char>();
    }

    std::size_t Pingresp::deserializeVP([[maybe_unused]] SocketContext* socketContext) {
        complete = true;
        return 0;
    }

    void Pingresp::propagateEvent([[maybe_unused]] SocketContext* socketContext) {
        socketContext->_onPingresp(*this);
    }

} // namespace iot::mqtt::packets
