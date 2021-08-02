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

    // Setup gameInfo
    _gameInfo = new GameInfo(getConstants());

    // Init signal mapper
    _mapper = new QSignalMapper();
}

Referee::~Referee() {
    // Delete gameInfo
    delete _gameInfo;
}

QString Referee::name() {
    return "Referee";
}

void Referee::initialization() {
    // Connect to network
    connectToNetwork();

    // Init aux timer
    _timer = new QTimer();
    _timerStarted = false;

    // Debug network info
    std::cout << Text::blue("[REFEREE] ", true) + Text::bold("Module started at address '" + _refereeAddress.toStdString() + "' and port '" + std::to_string(_refereePort) + "'.") + '\n';
}

void Referee::loop() {
    // Update timestamp [internally gameInfo will setup next stage if needed]
    _gameInfo->updateTimeStamp();

    /// TODO: process
    // If game is on, run all fouls
    if(_gameInfo->isGameOn()) {
        runFouls();
    }
    else {

    }

    // Send packet to network
    sendPacketToNetwork();
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

void Referee::sendPacketToNetwork() {
    // Taking command from GameInfo
    VSSRef::ref_to_team::VSSRef_Packet packet = _gameInfo->packet();

    // Serializing packet to string
    std::string datagram;
    packet.SerializeToString(&datagram);

    // Send via socket
    if(_refereeSocket->write(datagram.c_str(), static_cast<quint64>(datagram.length())) == -1) {
        std::cout << Text::blue("[REFEREE] ", true) + Text::red("Failed to write to socket.", true) + '\n';
        return ;
    }

    // Debug sent foul
    std::cout << Text::blue("[REFEREE] ", true) + Text::yellow("[" + VSSRef::Stage_Name(packet.gamestage()) + ":" + std::to_string(packet.timestamp()) + "] ", true) + Text::bold("Sent command '" + VSSRef::Command_Name(packet.command()) + "' for team '" + VSSRef::Color_Name(packet.commandcolor()) + "' at quadrant '" + VSSRef::Quadrant_Name(packet.commandquadrant())) + "'\n";
}

void Referee::addFoul(Foul *foul, int priority) {
    // Check if priority already exists in hash
    if(!_fouls.contains(priority)) {
        // Create it
        _fouls.insert(priority, new QVector<Foul*>());
    }

    // Taking Foul vector
    QVector<Foul*> *foulVector = _fouls.value(priority);

    // Check if foul is already added
    if(!foulVector->contains(foul)) {
        // Connect in map
        connect(foul, SIGNAL(foulOccured()), _mapper, SLOT(map()), Qt::UniqueConnection);
        _mapper->setMapping(foul, foul);
        connect(_mapper, SIGNAL(mapped(QObject *)), this, SLOT(processFoul(QObject *)), Qt::UniqueConnection);

        // Call configure method
        foul->configure();

        // Add it
        foulVector->push_back(foul);
    }
}

void Referee::runFouls() {
    // For each check, call run()
    QList<int> priorityKeys = _fouls.keys();
    QList<int>::iterator it;
    for(it = priorityKeys.begin(); it != priorityKeys.end(); it++) {
        QVector<Foul*> *fouls = _fouls.value((*it));
        for(int i = 0; i < fouls->size(); i++) {
            Foul *atFoul = fouls->at(i);
            atFoul->run();
        }
    }
}

void Referee::resetFouls() {
    // For each check, call configure() (reset it)
    QList<int> priorityKeys = _fouls.keys();
    QList<int>::iterator it;
    for(it = priorityKeys.begin(); it != priorityKeys.end(); it++) {
        QVector<Foul*> *fouls = _fouls.value((*it));
        for(int i = 0; i < fouls->size(); i++) {
            Foul *atFoul = fouls->at(i);
            atFoul->configure();
        }
    }
}

void Referee::deleteFouls() {
    QList<int> priorityKeys = _fouls.keys();
    QList<int>::iterator it;

    for(it = priorityKeys.begin(); it != priorityKeys.end(); it++) {
        QVector<Foul*> *fouls = _fouls.take((*it));
        for(int i = 0; i < fouls->size(); i++) {
            Foul *atFoul = fouls->at(i);
            delete atFoul;
        }
    }
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
