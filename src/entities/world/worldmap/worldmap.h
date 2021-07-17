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

#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <QObject>
#include <QMap>
#include <QReadWriteLock>

#include <src/constants/constants.h>
#include <src/utils/types/object/object.h>
#include <src/utils/types/color/color.h>
#include <src/entities/world/worldmap/locations/locations.h>

#include <include/packet.pb.h>

class WorldMap : public QObject
{
    Q_OBJECT
public:
    WorldMap(Field *field, Constants *constants);
    ~WorldMap();

    // Objects getter
    Object getPlayer(Color teamColor, quint8 playerId);
    Object getBall();

    // Utilities
    QList<quint8> getAvailableIds(Color teamColor);
    QList<Object> getAvailableObjects(Color teamColor);

    // Locations getter
    Locations* getLocations();

private:
    // Objects internal vars
    QMap<Colors::Color, QMap<quint8, Object>*> _playerObjects;
    Object _ballObject;

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // Field locations
    Locations *_locations;

    // Mutexes for read/write control management
    QReadWriteLock _playerMutex;
    QReadWriteLock _ballMutex;

public slots:
    void updatePlayers(Color teamColor, QList<Object> playerObjects);
    void updateBall(Object ballObject);
    void updateGeometry(fira_message::Field geometryData);
};

#endif // WORLDMAP_H
