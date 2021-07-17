/***
 * IEEE Very Small Size Soccer League
 * https://vsssleague.github.io/vss/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "referee.h"

Referee::Referee(WorldMap *worldMap, Constants *constants) : Entity(ENT_REFEREE) {
    // Take constants and worldmap
    _constants = constants;
    _worldMap = worldMap;

    // Set network data
    _refereeAddress = getConstants()->refereeAddress();
    _refereePort = getConstants()->refereePort();
}

QString Referee::name() {
    return "Referee";
}

void Referee::initialization() {
    // Connect to network
    connectToNetwork();

    // Debug network info
    std::cout << Text::blue("[REFEREE] ", true) + Text::bold("Module started at address '" + _refereeAddress.toStdString() + "' and port '" + std::to_string(_refereePort) + "'.") + '\n';
}

void Referee::loop() {

}

void Referee::finalization() {
    // Disconnect from network
    disconnectFromNetwork();

    // Debug finish
    std::cout << Text::blue("[REFEREE] ", true) + Text::bold("Module finished.") + '\n';
}

void Referee::connectToNetwork() {
    // Create socket pointer
    _refereeSocket = new QUdpSocket();

    // Close if already opened
    if(_refereeSocket->isOpen()) {
        _refereeSocket->close();
    }

    // Connect to network
    _refereeSocket->connectToHost(_refereeAddress, _refereePort, QIODevice::WriteOnly, QAbstractSocket::IPv4Protocol);
}

void Referee::disconnectFromNetwork() {
    // Close if already opened
    if(_refereeSocket->isOpen()) {
        _refereeSocket->close();
    }

    // Delete socket
    delete _refereeSocket;
}

Constants* Referee::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Referee") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

WorldMap* Referee::getWorldMap() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("WorldMap with nullptr value at Referee") + '\n';
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
