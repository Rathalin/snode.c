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

#ifndef IOT_MQTT_PACKETS_PUBREC_H
#define IOT_MQTT_PACKETS_PUBREC_H

#include "iot/mqtt/ControlPacket.h" // IWYU pragma: export
#include "iot/mqtt/types/UInt16.h"  // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt::packets {

    class Pubrec : public ControlPacket {
    public:
        Pubrec();
        explicit Pubrec(const uint16_t packetIdentifier);

    private:
        std::vector<char> serializeVP() const override;

    public:
        uint16_t getPacketIdentifier() const;

    protected:
        iot::mqtt::types::UInt16 packetIdentifier;
    };

} // namespace iot::mqtt::packets

#endif // IOT_MQTT_PACKETS_PUBREC_H
