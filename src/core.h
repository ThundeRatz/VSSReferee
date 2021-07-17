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

#ifndef CORE_H
#define CORE_H

#include <src/entities/referee/referee.h>
#include <src/entities/vision/vision.h>
#include <src/entities/world/world.h>
#include <src/utils/types/field/3v3/field_3v3.h>
#include <src/utils/types/field/5v5/field_5v5.h>
#include <src/gui/gui.h>

class Core
{
public:
    Core(Constants *constants);

    // Internal management
    void start(bool useGui, bool is5v5);
    void stop();

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorldMap();

    // Entities
    Vision *_vision;
    Referee *_referee;

    // GUI
    GUI *_gui;

    // World
    World *_world;
};

#endif // CORE_H
