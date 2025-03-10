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

#include "iot/mqtt/types/UInt32.h"

#include "iot/mqtt/types/TypeBase.hpp" // IWYU pragma: keep

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <endian.h>

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace iot::mqtt::types {

    uint32_t UInt32::operator=(const uint32_t& newValue) {
        return *reinterpret_cast<uint32_t*>(value.data()) = htobe32(newValue);
    }

    UInt32::operator uint32_t() const {
        return be32toh(*reinterpret_cast<uint32_t*>(const_cast<char*>(value.data())));
    }

    template class TypeBase<uint32_t>;

} // namespace iot::mqtt::types
