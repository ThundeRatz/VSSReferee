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

#ifndef REFEREE_H
#define REFEREE_H

#include <QUdpSocket>
#include <src/entities/entity.h>
#include <src/entities/referee/gameinfo/gameinfo.h>
#include <src/entities/referee/fouls/foul.h>

class Referee : public Entity
{
public:
    Referee(WorldMap *worldMap, Constants *constants);
    QString name();

private:
    // Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorldMap();

    // Network
    QUdpSocket *_refereeSocket;
    QString _refereeAddress;
    quint16 _refereePort;
    void connectToNetwork();
    void disconnectFromNetwork();

    // GameInfo
    GameInfo _gameInfo;
};

#endif // REFEREE_H
