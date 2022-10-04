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

#include "iot/mqtt1/packets/Unsubscribe.h"

#include "iot/mqtt1/SocketContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt1::packets {

    Unsubscribe::Unsubscribe(uint16_t packetIdentifier, std::list<std::string>& topics)
        : iot::mqtt1::ControlPacket(MQTT_SUBSCRIBE, 0x02, 0) {
        this->packetIdentifier.setValue(packetIdentifier);
        this->topics = topics;
    }

    Unsubscribe::Unsubscribe(uint32_t remainingLength, uint8_t reserved)
        : iot::mqtt1::ControlPacket(MQTT_SUBSCRIBE, reserved, remainingLength) {
        error = reserved != 0x02;
    }

    uint16_t Unsubscribe::getPacketIdentifier() const {
        return packetIdentifier.getValue();
    }

    const std::list<std::string>& Unsubscribe::getTopics() const {
        return topics;
    }

    std::vector<char> Unsubscribe::serializeVP() const {
        std::vector<char> packet;

        std::vector<char> tmpVector = packetIdentifier.serialize();
        packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

        for (const std::string& topic : topics) {
            packet.insert(packet.end(), topic.begin(), topic.end());
        }

        return packet;
    }

    std::size_t Unsubscribe::deserializeVP(SocketContext* socketContext) {
        std::size_t consumed = 0;

        switch (state) {
            case 0:
                consumed += packetIdentifier.deserialize(socketContext);

                if ((error = packetIdentifier.isError()) || !packetIdentifier.isComplete()) {
                    break;
                }
                state++;
                [[fallthrough]];
            case 1:
                consumed += topic.deserialize(socketContext);

                if (!(error = topic.isError()) && topic.isComplete()) {
                    topics.push_back(topic.getValue());
                    topic.reset();

                    if (getConsumed() + consumed < this->getRemainingLength()) {
                        state = 1;
                    } else {
                        complete = true;
                    }
                }

                break;
        }

        return consumed;
    }

    void Unsubscribe::propagateEvent([[maybe_unused]] SocketContext* socketContext) const {
        socketContext->_onUnsubscribe(*this);
    }

} // namespace iot::mqtt1::packets
