/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
 *               2021, 2022 Daniel Flockert
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

#include "database/mariadb/commands/MariaDBRollbackCommand.h"

#include "database/mariadb/MariaDBConnection.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace database::mariadb::commands {

    MariaDBRollbackCommand::MariaDBRollbackCommand(MariaDBConnection* mariaDBConnection,
                                                   const std::function<void()>& onRollback,
                                                   const std::function<void(const std::string&, unsigned int)>& onError)
        : MariaDBCommand(mariaDBConnection, "Rollback", onError)
        , onRollback(onRollback) {
    }

    int MariaDBRollbackCommand::commandStart() {
        return mysql_rollback_start(&ret, mysql);
    }

    int MariaDBRollbackCommand::commandContinue(int status) {
        return mysql_rollback_cont(&ret, mysql, status);
    }

    void MariaDBRollbackCommand::commandCompleted() {
        onRollback();
        mariaDBConnection->commandCompleted();
    }

    void MariaDBRollbackCommand::commandError(const std::string& errorString, unsigned int errorNumber) {
        onError(errorString, errorNumber);
    }

    std::string MariaDBRollbackCommand::commandInfo() {
        return commandName();
    }

    bool MariaDBRollbackCommand::error() {
        return ret != 0;
    }

} // namespace database::mariadb::commands
