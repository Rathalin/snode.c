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

#include "iot/mqtt1/packets/Connect.h"

#include "iot/mqtt1/SocketContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt1::packets {

    Connect::Connect(const std::string& clientId)
        : iot::mqtt1::ControlPacket(MQTT_CONNECT, 0, 0) {
        this->clientId.setValue(clientId);
    }

    Connect::Connect(uint32_t remainingLength, uint8_t reserved)
        : iot::mqtt1::ControlPacket(MQTT_CONNECT, reserved, remainingLength) {
    }

    std::string Connect::getProtocol() const {
        return protocol.getValue();
    }

    uint8_t Connect::getLevel() const {
        return level.getValue();
    }

    uint8_t Connect::getFlags() const {
        return flags.getValue();
    }

    uint16_t Connect::getKeepAlive() const {
        return keepAlive.getValue();
    }

    std::string Connect::getClientId() const {
        return clientId.getValue();
    }

    bool Connect::getUsernameFlag() const {
        return usernameFlag;
    }

    bool Connect::getPasswordFlag() const {
        return passwordFlag;
    }

    bool Connect::getWillRetain() const {
        return willRetain;
    }

    uint8_t Connect::getWillQoS() const {
        return willQoS;
    }

    bool Connect::getWillFlag() const {
        return willFlag;
    }

    bool Connect::getCleanSession() const {
        return cleanSession;
    }

    std::string Connect::getWillTopic() const {
        return willTopic.getValue();
    }

    std::string Connect::getWillMessage() const {
        return willMessage.getValue();
    }

    std::string Connect::getUsername() const {
        return username.getValue();
    }

    std::string Connect::getPassword() const {
        return password.getValue();
    }

    std::vector<char> Connect::getPacket() const {
        std::vector<char> packet;

        std::vector<char> tmpVector = protocol.getValueAsVector();
        packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

        tmpVector = level.getValueAsVector();
        packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

        tmpVector = flags.getValueAsVector();
        packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

        tmpVector = keepAlive.getValueAsVector();
        packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

        tmpVector = clientId.getValueAsVector();
        packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

        if (willFlag) {
            tmpVector = willTopic.getValueAsVector();
            packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());

            tmpVector = willMessage.getValueAsVector();
            packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());
        }
        if (usernameFlag) {
            tmpVector = username.getValueAsVector();
            packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());
        }
        if (passwordFlag) {
            tmpVector = password.getValueAsVector();
            packet.insert(packet.end(), tmpVector.begin(), tmpVector.end());
        }

        return packet;
    }

    std::size_t Connect::construct(SocketContext* socketContext) {
        std::size_t consumed = 0;

        switch (state) {
            // V-Header
            case 0:
                consumed += protocol.construct(socketContext);

                if ((error = protocol.isError()) || !protocol.isComplete()) {
                    break;
                }

                if ((error = (protocol.getValue() != "MQTT"))) {
                    break;
                }
                state++;
                [[fallthrough]];
            case 1:
                consumed += level.construct(socketContext);

                if ((error = level.isError()) || !level.isComplete()) {
                    break;
                }
                state++;
                [[fallthrough]];
            case 2:
                consumed += flags.construct(socketContext);

                if ((error = flags.isError()) || !flags.isComplete()) {
                    break;
                }

                reserved = (flags.getValue() & 0x01) != 0;
                cleanSession = (flags.getValue() & 0x02) != 0;
                willFlag = (flags.getValue() & 0x04) != 0;
                willQoS = (flags.getValue() & 0x18) >> 3;
                willRetain = (flags.getValue() & 0x20) != 0;
                passwordFlag = (flags.getValue() & 0x40) != 0;
                usernameFlag = (flags.getValue() & 0x80) != 0;
                state++;
                [[fallthrough]];
            case 3:
                consumed += keepAlive.construct(socketContext);

                if ((error = keepAlive.isError()) || !keepAlive.isComplete()) {
                    break;
                }
                state++;
                [[fallthrough]];
            // Payload
            case 4:
                consumed += clientId.construct(socketContext);

                if ((error = clientId.isError()) || !clientId.isComplete()) {
                    break;
                }
                state++;
                [[fallthrough]];
            case 5:
                if (willFlag) {
                    consumed += willTopic.construct(socketContext);

                    if ((error = willTopic.isError()) || !willTopic.isComplete()) {
                        break;
                    }
                }
                state++;
                [[fallthrough]];
            case 6:
                if (willFlag) {
                    consumed += willMessage.construct(socketContext);

                    if ((error = willMessage.isError()) || !willMessage.isComplete()) {
                        break;
                    }
                }
                state++;
                [[fallthrough]];
            case 7:
                if (usernameFlag) {
                    consumed += username.construct(socketContext);

                    if ((error = username.isError()) || !username.isComplete()) {
                        break;
                    }
                }
                state++;
                [[fallthrough]];
            case 8:
                if (passwordFlag) {
                    consumed += password.construct(socketContext);

                    if ((error = password.isError()) || !password.isComplete()) {
                        break;
                    }
                }
                [[fallthrough]];
            default:
                complete = true;
                break;
        }

        return consumed;
    }

    void Connect::propagateEvent(SocketContext* socketContext) const {
        socketContext->_onConnect(*this);
    }

} // namespace iot::mqtt1::packets
