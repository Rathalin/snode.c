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

#include "iot/mqtt/client/packets/Suback.h"

#include "iot/mqtt/client/SocketContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt::client::packets {

    Suback::Suback(uint32_t remainingLength, uint8_t flags)
        : iot::mqtt::client::ControlPacketDeserializer(remainingLength, flags, MQTT_SUBACK_FLAGS) {
        this->flags = flags;
    }

    std::size_t Suback::deserializeVP(iot::mqtt::SocketContext* socketContext) {
        std::size_t consumed = 0;

        switch (state) {
            case 0: // V-Header
                consumed += packetIdentifier.deserialize(socketContext);

                if (!packetIdentifier.isComplete()) {
                    break;
                }

                state++;
                [[fallthrough]];
            case 1: // Payload
                consumed += returnCode.deserialize(socketContext);

                if (!returnCode.isComplete()) {
                    break;
                } else {
                    returnCodes.push_back(returnCode);
                    returnCode.reset();

                    if (getConsumed() + consumed < this->getRemainingLength()) {
                        break;
                    }
                }

                complete = true;
                break;
        }

        return consumed;
    }

    void Suback::propagateEvent(iot::mqtt::client::SocketContext* socketContext) {
        socketContext->_onSuback(*this);
    }

} // namespace iot::mqtt::client::packets
