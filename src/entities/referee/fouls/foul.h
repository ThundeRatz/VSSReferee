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

#ifndef FOUL_H
#define FOUL_H

#include <QObject>

#include <src/entities/world/worldmap/worldmap.h>
#include <src/utils/utils.h>

class Foul : public QObject
{
    Q_OBJECT
public:
    Foul(WorldMap *worldMap, Constants *constants);

    // Internal
    virtual QString name() = 0;
    virtual void configure() = 0;
    virtual void run() = 0;

    // Penalties info
    /// TODO: describe the new protobuf packet

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorldMap();

signals:
    void foulOccured();
};

#endif // FOUL_H
