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

#include "core/EventLoop.h" // for EventLoop

#include "core/DynamicLoader.h"
#include "core/EventDispatcher.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "core/system/signal.h"
#include "log/Logger.h" // for Logger
#include "utils/Config.h"

#include <cstdlib> // for exit
#include <cstring>
#include <string> // for string, to_string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define MAX_READ_INACTIVITY 60
#define MAX_WRITE_INACTIVITY 60
#define MAX_OUTOFBAND_INACTIVITY 60

/* Must be implemented in every variant of a multiplexer api */
core::EventDispatcher& EventDispatcher();

namespace core {

    bool EventLoop::initialized = false;
    bool EventLoop::running = false;
    bool EventLoop::stopped = true;
    int EventLoop::stopsig = 0;
    unsigned long EventLoop::tickCounter = 0;

    static std::string getTickCounterAsString(const el::LogMessage*) {
        std::string tick = std::to_string(EventLoop::getTickCounter());

        if (tick.length() < 10) {
            tick.insert(0, 10 - tick.length(), '0');
        }

        return tick;
    }

    EventLoop::EventLoop()
        : eventDispatcher(::EventDispatcher())

    {
    }

    EventLoop& EventLoop::instance() {
        static EventLoop eventLoop;

        return eventLoop;
    }

    unsigned long EventLoop::getTickCounter() {
        return tickCounter;
    }

    EventDispatcher& EventLoop::getEventDispatcher() {
        return eventDispatcher;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
    void EventLoop::init(int argc, char* argv[]) {
        utils::Config::instance().init(std::string(basename(argv[0])), argc, argv);

        logger::Logger::setCustomFormatSpec("%tick", core::getTickCounterAsString);

        EventLoop::initialized = true;
    }

    TickStatus EventLoop::_tick(const utils::Timeval& tickTimeOut, bool stopped) {
        tickCounter++;

        TickStatus tickStatus = eventDispatcher.dispatch(tickTimeOut, stopped);

        DynamicLoader::execDlCloseDeleyed();

        return tickStatus;
    }

    TickStatus EventLoop::tick(const utils::Timeval& timeOut) {
        if (!initialized) {
            PLOG(ERROR) << "snode.c not initialized. Use SNodeC::init(argc, argv) before SNodeC::tick().";
            exit(1);
        }

        sighandler_t oldSigPipeHandler = core::system::signal(SIGPIPE, SIG_IGN);

        TickStatus tickStatus = EventLoop::instance()._tick(timeOut, stopped);

        core::system::signal(SIGPIPE, oldSigPipeHandler);

        return tickStatus;
    }

    int EventLoop::start(const utils::Timeval& timeOut) {
        if (!initialized) {
            PLOG(ERROR) << "snode.c not initialized. Use SNodeC::init(argc, argv) before SNodeC::start().";
            exit(1);
        }

        utils::Config::instance().prepare();

        sighandler_t oldSigPipeHandler = core::system::signal(SIGPIPE, SIG_IGN);
        sighandler_t oldSigQuitHandler = core::system::signal(SIGQUIT, EventLoop::stoponsig);
        sighandler_t oldSigHubHandler = core::system::signal(SIGHUP, EventLoop::stoponsig);
        sighandler_t oldSigIntHandler = core::system::signal(SIGINT, EventLoop::stoponsig);
        sighandler_t oldSigTermHandler = core::system::signal(SIGTERM, EventLoop::stoponsig);
        sighandler_t oldSigAbrtHandler = core::system::signal(SIGABRT, EventLoop::stoponsig);

        if (!running) {
            running = true;

            stopped = false;

            core::TickStatus tickStatus = TickStatus::SUCCESS;
            while (tickStatus == TickStatus::SUCCESS && !stopped) {
                tickStatus = EventLoop::instance()._tick(timeOut, stopped);
            }

            switch (tickStatus) {
                case TickStatus::SUCCESS:
                    LOG(INFO) << "EventLoop terminated: Releasing resources";
                    break;
                case TickStatus::NO_OBSERVER:
                    LOG(INFO) << "EventLoop: No Observer - exiting";
                    break;
                case TickStatus::ERROR:
                    PLOG(ERROR) << "EventDispatcher::dispatch()";
                    break;
            }

            running = false;
        }

        core::system::signal(SIGPIPE, oldSigPipeHandler);
        core::system::signal(SIGQUIT, oldSigQuitHandler);
        core::system::signal(SIGHUP, oldSigHubHandler);
        core::system::signal(SIGINT, oldSigIntHandler);
        core::system::signal(SIGTERM, oldSigTermHandler);
        core::system::signal(SIGABRT, oldSigAbrtHandler);

        free();

        int returnReason = 0;
        if (stopsig != 0) {
            returnReason = -stopsig;
        }

        return returnReason;
    }

    void EventLoop::free() {
        core::TickStatus tickStatus;

        do {
            instance().eventDispatcher.stopDescriptorEvents();
            tickStatus = instance()._tick(2, true);
        } while (tickStatus == TickStatus::SUCCESS);

        do {
            instance().eventDispatcher.stopTimerEvents();
            tickStatus = instance()._tick(0, false);
        } while (tickStatus == TickStatus::SUCCESS);

        DynamicLoader::execDlCloseDeleyed();
        DynamicLoader::execDlCloseAll();

        utils::Config::instance().terminate();

        LOG(INFO) << "All resources released";
    }

    void EventLoop::stoponsig(int sig) {
        stopsig = sig;
        stopped = true;
    }

} // namespace core
