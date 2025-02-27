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

#include "iot/mqtt/client/SocketContext.h"

#include "iot/mqtt/client/packets/Connack.h"
#include "iot/mqtt/client/packets/Pingresp.h"
#include "iot/mqtt/client/packets/Puback.h"
#include "iot/mqtt/client/packets/Pubcomp.h"
#include "iot/mqtt/client/packets/Publish.h"
#include "iot/mqtt/client/packets/Pubrec.h"
#include "iot/mqtt/client/packets/Pubrel.h"
#include "iot/mqtt/client/packets/Suback.h"
#include "iot/mqtt/client/packets/Unsuback.h"
#include "iot/mqtt/packets/Connect.h"
#include "iot/mqtt/packets/Disconnect.h"
#include "iot/mqtt/packets/Pingreq.h"
#include "iot/mqtt/packets/Subscribe.h"
#include "iot/mqtt/packets/Unsubscribe.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "log/Logger.h"

#include <iomanip>

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt::client {

    SocketContext::SocketContext(core::socket::SocketConnection* socketConnection)
        : iot::mqtt::SocketContext(socketConnection) {
    }

    iot::mqtt::ControlPacketDeserializer* SocketContext::createControlPacketDeserializer(iot::mqtt::FixedHeader& fixedHeader) {
        iot::mqtt::ControlPacketDeserializer* currentPacket = nullptr;

        switch (fixedHeader.getPacketType()) {
            case MQTT_CONNACK:
                currentPacket = new iot::mqtt::client::packets::Connack(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_PUBLISH:
                currentPacket = new iot::mqtt::client::packets::Publish(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_PUBACK:
                currentPacket = new iot::mqtt::client::packets::Puback(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_PUBREC:
                currentPacket = new iot::mqtt::client::packets::Pubrec(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_PUBREL:
                currentPacket = new iot::mqtt::client::packets::Pubrel(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_PUBCOMP:
                currentPacket = new iot::mqtt::client::packets::Pubcomp(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_SUBACK:
                currentPacket = new iot::mqtt::client::packets::Suback(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_UNSUBACK:
                currentPacket = new iot::mqtt::client::packets::Unsuback(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            case MQTT_PINGRESP:
                currentPacket = new iot::mqtt::client::packets::Pingresp(fixedHeader.getRemainingLength(), fixedHeader.getFlags());
                break;
            default:
                currentPacket = nullptr;
                break;
        }

        return currentPacket;
    }

    void SocketContext::propagateEvent(iot::mqtt::ControlPacketDeserializer* controlPacketDeserializer) {
        dynamic_cast<iot::mqtt::client::ControlPacketDeserializer*>(controlPacketDeserializer)->propagateEvent(this);
    }

    void SocketContext::onConnack([[maybe_unused]] mqtt::packets::Connack& connack) {
    }

    void SocketContext::onPublish([[maybe_unused]] mqtt::packets::Publish& publish) {
    }

    void SocketContext::onPuback([[maybe_unused]] mqtt::packets::Puback& puback) {
    }

    void SocketContext::onPubrec([[maybe_unused]] mqtt::packets::Pubrec& pubrec) {
    }

    void SocketContext::onPubrel([[maybe_unused]] mqtt::packets::Pubrel& pubrel) {
    }

    void SocketContext::onPubcomp([[maybe_unused]] mqtt::packets::Pubcomp& pubcomp) {
    }

    void SocketContext::onSuback([[maybe_unused]] mqtt::packets::Suback& suback) {
    }

    void SocketContext::onUnsuback([[maybe_unused]] mqtt::packets::Unsuback& unsuback) {
    }

    void SocketContext::onPingresp([[maybe_unused]] mqtt::packets::Pingresp& pingresp) {
    }

    void SocketContext::_onConnack(iot::mqtt::packets::Connack& connack) {
        LOG(DEBUG) << "Received Connack:";
        LOG(DEBUG) << "=================";
        printStandardHeader(connack);
        LOG(DEBUG) << "Acknowledge Flag: " << static_cast<int>(connack.getAcknowledgeFlags());
        LOG(DEBUG) << "Return code: " << static_cast<int>(connack.getReturnCode());
        LOG(DEBUG) << "Session present: " << connack.getSessionPresent();

        if (connack.getReturnCode() != MQTT_CONNACK_ACCEPT) {
            shutdown(true);
        } else {
            onConnack(connack);
        }
    }

    void SocketContext::_onPublish(iot::mqtt::packets::Publish& publish) {
        LOG(DEBUG) << "Received PUBLISH:";
        LOG(DEBUG) << "=================";
        printStandardHeader(publish);
        LOG(DEBUG) << "DUP: " << publish.getDup();
        LOG(DEBUG) << "QoS: " << static_cast<uint16_t>(publish.getQoS());
        LOG(DEBUG) << "Retain: " << publish.getRetain();
        LOG(DEBUG) << "Topic: " << publish.getTopic();
        LOG(DEBUG) << "PacketIdentifier: " << publish.getPacketIdentifier();
        LOG(DEBUG) << "Message: " << publish.getMessage();

        if (publish.getQoS() > 2) {
            shutdown(true);
        } else if (publish.getPacketIdentifier() == 0 && publish.getQoS() > 0) {
            shutdown(true);
        } else {
            switch (publish.getQoS()) {
                case 1:
                    sendPuback(publish.getPacketIdentifier());
                    break;
                case 2:
                    sendPubrec(publish.getPacketIdentifier());
                    break;
            }

            onPublish(publish);
        }
    }

    void SocketContext::_onPuback(iot::mqtt::packets::Puback& puback) {
        LOG(DEBUG) << "Received PUBACK:";
        LOG(DEBUG) << "================";
        printStandardHeader(puback);
        LOG(DEBUG) << "PacketIdentifier: 0x" << std::hex << std::setfill('0') << std::setw(4) << puback.getPacketIdentifier();

        if (puback.getPacketIdentifier() == 0) {
            shutdown(true);
        } else {
            onPuback(puback);
        }
    }

    void SocketContext::_onPubrec(iot::mqtt::packets::Pubrec& pubrec) {
        LOG(DEBUG) << "Received PUBREC:";
        LOG(DEBUG) << "================";
        printStandardHeader(pubrec);
        LOG(DEBUG) << "PacketIdentifier: 0x" << std::hex << std::setfill('0') << std::setw(4) << pubrec.getPacketIdentifier();

        if (pubrec.getPacketIdentifier() == 0) {
            shutdown(true);
        } else {
            sendPubrel(pubrec.getPacketIdentifier());

            onPubrec(pubrec);
        }
    }

    void SocketContext::_onPubrel(iot::mqtt::packets::Pubrel& pubrel) {
        LOG(DEBUG) << "Received PUBREL:";
        LOG(DEBUG) << "================";
        printStandardHeader(pubrel);
        LOG(DEBUG) << "PacketIdentifier: 0x" << std::hex << std::setfill('0') << std::setw(4) << pubrel.getPacketIdentifier();

        if (pubrel.getPacketIdentifier() == 0) {
            shutdown(true);
        } else {
            sendPubcomp(pubrel.getPacketIdentifier());

            onPubrel(pubrel);
        }
    }

    void SocketContext::_onPubcomp(iot::mqtt::packets::Pubcomp& pubcomp) {
        LOG(DEBUG) << "Received PUBCOMP:";
        LOG(DEBUG) << "=================";
        printStandardHeader(pubcomp);
        LOG(DEBUG) << "PacketIdentifier: 0x" << std::hex << std::setfill('0') << std::setw(4) << pubcomp.getPacketIdentifier();

        if (pubcomp.getPacketIdentifier() == 0) {
            shutdown(true);
        } else {
            onPubcomp(pubcomp);
        }
    }

    void SocketContext::_onSuback(iot::mqtt::packets::Suback& suback) {
        LOG(DEBUG) << "Received SUBACK:";
        LOG(DEBUG) << "================";
        printStandardHeader(suback);
        LOG(DEBUG) << "PacketIdentifier: 0x" << std::hex << std::setfill('0') << std::setw(4) << suback.getPacketIdentifier();
        //        LOG(DEBUG) << "Return codes: " << suback.getReturnCodes();

        if (suback.getPacketIdentifier() == 0) {
            shutdown(true);
        } else {
            onSuback(suback);
        }
    }

    void SocketContext::_onUnsuback(iot::mqtt::packets::Unsuback& unsuback) {
        LOG(DEBUG) << "Received UNSUBACK:";
        LOG(DEBUG) << "==================";
        LOG(DEBUG) << "PacketIdentifier: 0x" << std::hex << std::setfill('0') << std::setw(4) << unsuback.getPacketIdentifier();

        if (unsuback.getPacketIdentifier() == 0) {
            shutdown(true);
        } else {
            onUnsuback(unsuback);
        }
    }

    void SocketContext::_onPingresp(iot::mqtt::packets::Pingresp& pingresp) {
        LOG(DEBUG) << "Received PINGRESP:";
        LOG(DEBUG) << "==================";

        onPingresp(pingresp);
    }

    void SocketContext::sendConnect(uint16_t keepAlive,
                                    const std::string& clientId,

                                    bool cleanSession,
                                    const std::string& willTopic,
                                    const std::string& willMessage,
                                    uint8_t willQoS,
                                    bool willRetain,
                                    const std::string& username,
                                    const std::string& password) { // Client
        LOG(DEBUG) << "Send CONNECT";
        LOG(DEBUG) << "============";

        send(iot::mqtt::packets::Connect(
            clientId, keepAlive, cleanSession, willTopic, willMessage, willQoS, willRetain, username, password));
    }

    void SocketContext::sendSubscribe(uint16_t packetIdentifier, std::list<iot::mqtt::Topic>& topics) { // Client
        LOG(DEBUG) << "Send SUBSCRIBE";
        LOG(DEBUG) << "==============";

        send(iot::mqtt::packets::Subscribe(packetIdentifier, topics));
    }

    void SocketContext::sendUnsubscribe(uint16_t packetIdentifier, std::list<std::string>& topics) { // Client
        LOG(DEBUG) << "Send UNSUBSCRIBE";
        LOG(DEBUG) << "================";

        send(iot::mqtt::packets::Unsubscribe(packetIdentifier, topics));
    }

    void SocketContext::sendPingreq() { // Client
        LOG(DEBUG) << "Send Pingreq";
        LOG(DEBUG) << "============";

        send(iot::mqtt::packets::Pingreq());
    }

    void SocketContext::sendDisconnect() {
        LOG(DEBUG) << "Send Disconnect";
        LOG(DEBUG) << "===============";

        send(iot::mqtt::packets::Disconnect());
    }

} // namespace iot::mqtt::client
