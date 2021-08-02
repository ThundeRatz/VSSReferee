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

#include "vision.h"

#include "vision.h"

#include <QNetworkDatagram>
#include <src/utils/text/text.h>

Vision::Vision(Constants *constants) : Entity(ENT_VISION) {
    // Taking constants
    _constants = constants;

    // Taking network data from constants
    _visionAddress = getConstants()->visionAddress();
    _visionPort = getConstants()->visionPort();

    // Allocating memory to control vectors
    _blueControl = (bool *) malloc(getConstants()->qtPlayers() * sizeof(bool));
    _yellowControl = (bool *) malloc(getConstants()->qtPlayers() * sizeof(bool));

    // Inserting teams for map
    _playerObjects.insert(VSSRef::Color::BLUE, new QMap<quint8, Object*>());
    _playerObjects.insert(VSSRef::Color::YELLOW, new QMap<quint8, Object*>());

    // Inserting players in map
    for(int i = VSSRef::Color::BLUE; i <= VSSRef::Color::YELLOW; i++) {
        for(int j = 0; j < getConstants()->qtPlayers(); j++) {
            QMap<quint8, Object*> *teamMap = _playerObjects.value(VSSRef::Color(i));
            teamMap->insert(j, new Object());
        }
    }
}

Vision::~Vision() {
    // Closing socket
    if(_visionClient->isOpen()) {
        _visionClient->close();
    }

    // Deleting vision client
    delete _visionClient;

    // Delete controls
    free(_blueControl);
    free(_yellowControl);

    // Get teams list
    QList<VSSRef::Color> teamList = _playerObjects.keys();

    // For each team
    for(QList<VSSRef::Color>::iterator it = teamList.begin(); it != teamList.end(); it++) {
        // Take team associated objects
        QMap<quint8, Object*> *teamObjects = _playerObjects.value((*it));
        QList<Object*> objects = teamObjects->values();

        // For each object, delete it
        for(QList<Object*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++) {
            delete (*it2);
        }

        // Clear team association
        teamObjects->clear();

        // Delete pointer
        delete teamObjects;
    }

    // Clear teams
    _playerObjects.clear();
}

QString Vision::name() {
    return "Vision";
}

void Vision::initialization() {
    // Bind and connect socket in network
    bindAndConnect();
    std::cout << Text::cyan("[VISION] ", true) + Text::bold("Started at address '" + _visionAddress.toStdString() + "' and port '" + std::to_string(_visionPort) + "'.") + '\n';
}

void Vision::loop() {
    while(_visionClient->hasPendingDatagrams()) {
        // Creating auxiliary vars
        fira_message::sim_to_ref::Environment wrapperData;
        QNetworkDatagram datagram;

        // Reading datagram and checking if it is valid
        datagram = _visionClient->receiveDatagram();
        if(!datagram.isValid()) {
            continue;
        }

        // Parsing datagram and checking if it worked properly
        if(wrapperData.ParseFromArray(datagram.data().data(), datagram.data().size()) == false) {
            std::cout << Text::cyan("[VISION] ", true) << Text::red("Wrapper packet parsing error.", true) + '\n';
            continue;
        }

        // Debug vision frame
        if(wrapperData.has_frame()) {
            fira_message::Frame visionFrame = wrapperData.frame();

            // Clear vision controls
            clearControls();

            // Update ball
            updateBall(visionFrame.ball());

            // Update blue team received players
            for(int i = 0; i < visionFrame.robots_blue_size(); i++) {
                // Mark as reached
                _blueControl[visionFrame.robots_blue(i).robot_id()] = true;

                // Take player and update
                fira_message::Robot robot = visionFrame.robots_blue(i);
                updatePlayer(VSSRef::Color::BLUE, robot.robot_id(), robot);
            }

            // Update yellow team received players
            for(int i = 0; i < visionFrame.robots_yellow_size(); i++) {
                // Mark as reached
                _yellowControl[visionFrame.robots_yellow(i).robot_id()] = true;

                // Take player and update
                fira_message::Robot robot = visionFrame.robots_yellow(i);
                updatePlayer(VSSRef::Color::YELLOW, robot.robot_id(), robot);
            }

            // Update not appeared objects for both teams yellow and blue
            updateNotAppeared(VSSRef::Color::YELLOW);
            updateNotAppeared(VSSRef::Color::BLUE);

            // Send players
            for(int i = VSSRef::Color::BLUE; i <= VSSRef::Color::YELLOW; i++) {
                // Generate list of objects
                QList<Object> playerObjects;
                for(int j = 0; j < getConstants()->qtPlayers(); j++) {
                    Object playerObject = *(_playerObjects.value(VSSRef::Color(i))->value(j));
                    playerObjects.append(playerObject);
                }

                // Send list
                emit sendPlayers(VSSRef::Color(i), playerObjects);
            }

            // Update not appeared ball
            if(_ballControl == false) {
                _ballObject.updateObject(0.0f, Position(false, 0.0, 0.0));
                emit sendBall(_ballObject);
            }
        }

        // Debug geometry data
        if(wrapperData.has_field()) {
            fira_message::Field geometryData = wrapperData.field();
            emit sendGeometryData(geometryData);
        }
    }
}

void Vision::finalization() {
    std::cout << Text::cyan("[VISION] " , true) + Text::bold("Client finished.") + '\n';
}

void Vision::updateNotAppeared(Color color) {
    // Run for every player
    for(int i = 0; i < getConstants()->qtPlayers(); i++) {
        // Take control (true if appeared)
        bool appeared = color.isBlue() ? _blueControl[i] : _yellowControl[i];
        // If not appeared
        if(!appeared) {
            // Take team map
            QMap<quint8, Object*> *teamMap = _playerObjects.value(color.getColor());

            // Take object
            Object* playerObject = teamMap->value(i);

            // Update object by an invalid value (not appeared by default)
            playerObject->updateObject(0.0f, Position(false, 0.0f, 0.0f));
        }
    }
}

void Vision::clearControls() {
    for(int i = 0; i < getConstants()->qtPlayers(); i++) {
        _blueControl[i] = false;
        _yellowControl[i] = false;
    }
    _ballControl = false;
}

void Vision::updatePlayer(VSSRef::Color teamColor, quint8 playerId, fira_message::Robot player) {
    // Check if team is registered in map
    if(!_playerObjects.contains(teamColor)) {
        _playerObjects.insert(teamColor, new QMap<quint8, Object*>());
    }

    // Take team map
    QMap<quint8, Object*> *teamMap = _playerObjects.value(teamColor);

    // Check if teamMap has the required player
    if(!teamMap->contains(playerId)) {
        teamMap->insert(playerId, new Object());
    }

    // Take object
    Object* playerObject = teamMap->value(playerId);

    // Update
    playerObject->updateObject(1.0f, Position(true, player.x(), player.y()), Angle(true, player.orientation()));
}

void Vision::updateBall(fira_message::Ball ball) {
    // Update
    _ballObject.updateObject(1.0f, Position(true, ball.x(), ball.y()));

    // Emit updated object
    emit sendBall(_ballObject);
}

void Vision::bindAndConnect() {
    // Creating socket
    _visionClient = new QUdpSocket();

    // Binding in defined network
    if(_visionClient->bind(QHostAddress(_visionAddress), _visionPort, QUdpSocket::ShareAddress) == false) {
        std::cout << Text::cyan("[VISION] " , true) << Text::red("Error while binding socket.", true) + '\n';
        return ;
    }

    // Joining multicast group
    if(_visionClient->joinMulticastGroup(QHostAddress(_visionAddress)) == false) {
        std::cout << Text::cyan("[VISION] ", true) << Text::red("Error while joining multicast.", true) + '\n';
        return ;
    }
}

Constants* Vision::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Vision") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
