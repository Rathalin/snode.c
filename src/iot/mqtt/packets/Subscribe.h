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

#ifndef IOT_MQTT_PACKETSNEW_SUBSCRIBE_H
#define IOT_MQTT_PACKETSNEW_SUBSCRIBE_H

#include "iot/mqtt/Topic.h"        // IWYU pragma: export
#include "iot/mqtt/types/String.h" // IWYU pragma: export
#include "iot/mqtt/types/UInt16.h" // IWYU pragma: export
#include "iot/mqtt/types/UInt8.h"  // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <list> // IWYU pragma: export

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

#define MQTT_SUBSCRIBE 0x08
#define MQTT_SUBSCRIBE_FLAGS 0x02

namespace iot::mqtt::packets {

    class Subscribe {
    public:
        Subscribe() = default;

        uint16_t getPacketIdentifier() const;
        std::list<iot::mqtt::Topic>& getTopics();

    protected:
        iot::mqtt::types::UInt16 packetIdentifier;
        iot::mqtt::types::String topic;
        iot::mqtt::types::UInt8 qoS;

        std::list<iot::mqtt::Topic> topics;
    };

} // namespace iot::mqtt::packets

#endif // IOT_MQTT_PACKETSNEW_SUBSCRIBE_H
