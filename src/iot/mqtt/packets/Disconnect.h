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

#ifndef IOT_MQTT_PACKETSNEW_DISCONNECT_H
#define IOT_MQTT_PACKETSNEW_DISCONNECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

#define MQTT_DISCONNECT 0x0E
#define MQTT_DISCONNECT_FLAGS 0x00

namespace iot::mqtt::packets {

    class Disconnect {
    public:
        Disconnect() = default;
    };

} // namespace iot::mqtt::packets

#endif // IOT_MQTT_PACKETSNEW_DISCONNECT_H
