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

#include "worldmap.h"

#include <src/utils/text/text.h>

WorldMap::WorldMap(Field *field, Constants *constants) {
    // Taking constants
    _constants = constants;

    // Fill player objects
    for(int i = VSSRef::Color::BLUE; i <= VSSRef::Color::YELLOW; i++) {
        _playerObjects.insert(VSSRef::Color(i), new QMap<quint8, Object>());
        QMap<quint8, Object> *_teamObjects = _playerObjects.value(VSSRef::Color(i));
        for(int j = 0; j < getConstants()->qtPlayers(); j++) {
            Object object;
            object.setInvalid();
            _teamObjects->insert(j, object);
        }
    }

    // Initializing locations
    _locations = new Locations(field, getConstants());
}

WorldMap::~WorldMap() {
    // Get teams list
    QList<VSSRef::Color> teamList = _playerObjects.keys();

    // For each team
    for(QList<VSSRef::Color>::iterator it = teamList.begin(); it != teamList.end(); it++) {
        // Take team associated objects
        QMap<quint8, Object> *teamObjects = _playerObjects.value((*it));

        // Clear team association
        teamObjects->clear();

        // Delete pointer
        delete teamObjects;
    }

    // Clear teams
    _playerObjects.clear();

    // Delete locations
    delete _locations;
}

Constants* WorldMap::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at WorldMap") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

Object WorldMap::getPlayer(Color teamColor, quint8 playerId) {
    _playerMutex.lockForRead();

    // Check if contains teamColor
    if(!_playerObjects.contains(teamColor.getColor())) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("WorldMap::getPlayer(" + std::to_string(teamColor.getColor()) + ", quint8), teamColor " + std::to_string(teamColor.getColor()) + " does not exists.") + '\n';
        _playerMutex.unlock();
        return Object();
    }

    // Taking map for team
    QMap<quint8, Object> *_teamObjects = _playerObjects.value(teamColor.getColor());

    // Check if contains required player
    if(!_teamObjects->contains(playerId)) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("WorldMap::getPlayer(teamColor, " + std::to_string(playerId) + "), playerId " + std::to_string(playerId) + " does not exists.") + '\n';
        _playerMutex.unlock();
        return Object();
    }

    // Take object
    Object requiredObject = _teamObjects->value(playerId);

    _playerMutex.unlock();

    return requiredObject;
}

Object WorldMap::getBall() {
    _ballMutex.lockForRead();
    Object ballObject = _ballObject;
    _ballMutex.unlock();

    return ballObject;
}

QList<quint8> WorldMap::getAvailableIds(Color teamColor) {
    _playerMutex.lockForRead();
    QList<quint8> playersList;
    if(!_playerObjects.contains(teamColor.getColor())) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("WorldMap::getAvailablePlayers(" + std::to_string(teamColor.getColor()) + "), teamColor " + std::to_string(teamColor.getColor()) + " does not exists.") + '\n';
        _playerMutex.unlock();
        return playersList;
    }

    // Take team players
    QMap<quint8, Object> *teamPlayers = _playerObjects.value(teamColor.getColor());
    QMap<quint8, Object>::iterator it;

    for(it = teamPlayers->begin(); it != teamPlayers->end(); it++) {
        // Take player id and object
        quint8 playerId = it.key();
        Object playerObject = it.value();

        // If position is not invalid (player exists in field) add to the playersList
        if(!playerObject.getPosition().isInvalid()) {
            playersList.push_back(playerId);
        }
    }
    _playerMutex.unlock();

    return playersList;
}

QList<Object> WorldMap::getAvailableObjects(Color teamColor) {
    _playerMutex.lockForRead();

    QList<Object> objectsList;
    if(!_playerObjects.contains(teamColor.getColor())) {
        std::cout << Text::red("[ERROR] ", true) + Text::bold("WorldMap::getAvailableObjects(" + std::to_string(teamColor.getColor()) + "), teamColor " + std::to_string(teamColor.getColor()) + " does not exists.") + '\n';
        _playerMutex.unlock();
        return objectsList;
    }

    // Take team players
    QMap<quint8, Object> *teamPlayers = _playerObjects.value(teamColor.getColor());
    QMap<quint8, Object>::iterator it;

    for(it = teamPlayers->begin(); it != teamPlayers->end(); it++) {
        // Take player object
        Object playerObject = it.value();

        // If position is not invalid (player exists in field) add to the objectsList
        if(!playerObject.getPosition().isInvalid()) {
            objectsList.push_back(playerObject);
        }
    }
    _playerMutex.unlock();

    return objectsList;
}

void WorldMap::updatePlayers(Color teamColor, QList<Object> playerObjects) {
    _playerMutex.lockForWrite();

    // If !contains teamColor, create it
    if(!_playerObjects.contains(teamColor.getColor())) {
        _playerObjects.insert(teamColor.getColor(), new QMap<quint8, Object>());
    }

    // Taking map for the required team
    QMap<quint8, Object> *_teamObjects = _playerObjects.value(teamColor.getColor());

    // Iterate in received list (it is already sorted by ids)
    for(int i = 0; i < playerObjects.size(); i++) {
        // If !contains a player with playerId, create it
        if(!_teamObjects->contains(i)) {
            _teamObjects->insert(i, Object());
        }

        // Insert received object
        _teamObjects->insert(i, playerObjects.at(i));
    }

    _playerMutex.unlock();
}

void WorldMap::updateBall(Object ballObject) {
    _ballMutex.lockForWrite();

    _ballObject = ballObject;

    _ballMutex.unlock();
}

void WorldMap::updateGeometry(fira_message::Field geometryData) {
    _locations->updateGeometryData(geometryData);
}

Locations* WorldMap::getLocations() {
    return _locations;
}
