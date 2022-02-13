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

#ifndef CORE_TIMEREVENTRECEIVER_H
#define CORE_TIMEREVENTRECEIVER_H

#include "EventReceiver.h"

namespace core::timer {
    class Timer;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/Timeval.h" // IWYU pragma: export

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core {

    class TimerEventReceiver : public EventReceiver {
    public:
        TimerEventReceiver(const TimerEventReceiver&) = delete;
        TimerEventReceiver& operator=(const TimerEventReceiver&) = delete;

        TimerEventReceiver(const utils::Timeval& delay);

        virtual ~TimerEventReceiver();

        utils::Timeval getTimeout() const;
        void updateTimeout();

        void cancel();

        virtual void unobservedEvent() = 0;

        void setTimer(core::timer::Timer* timer);
        core::timer::Timer* getTimer() {
            return timer;
        }

    private:
        core::timer::Timer* timer = nullptr;
        utils::Timeval absoluteTimeout;
        utils::Timeval delay;
    };

} // namespace core

#endif // CORE_TIMEREVENTRECEIVER_H
