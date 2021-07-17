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

#include "core.h"

Core::Core(Constants *constants) {
    // Setting up constants
    _constants = constants;

    // Creating world
    _world = new World(getConstants());
}

void Core::start(bool useGui, bool is5v5) {
    // Creating World Map (set here the map that u desire)
    Field *field3v3 = new Field_3v3();
    Field *field5v5 = new Field_5v5();
    _worldMap = new WorldMap(is5v5 ? field3v3 : field5v5, getConstants());

    // Setup Utils
    Utils::setConstants(getConstants());

    // Creating and adding vision to world
    _vision = new Vision(getConstants());
    _world->addEntity(_vision, 0);

    // Creating and adding referee to world
    _referee = new Referee(getWorldMap(), getConstants());
    _world->addEntity(_referee, 1);

    // Vision-WorldMap connection
    qRegisterMetaType<Color>("Color");
    qRegisterMetaType<Object>("Object");
    qRegisterMetaType<fira_message::Field>("fira_message::Field");
    qRegisterMetaType<Position>("Position");
    QObject::connect(_vision, SIGNAL(sendPlayers(Color, QList<Object>)), _worldMap, SLOT(updatePlayers(Color, QList<Object>)), Qt::DirectConnection);
    QObject::connect(_vision, SIGNAL(sendBall(Object)), _worldMap, SLOT(updateBall(Object)), Qt::DirectConnection);
    QObject::connect(_vision, SIGNAL(sendGeometryData(fira_message::Field)), _worldMap, SLOT(updateGeometry(fira_message::Field)), Qt::DirectConnection);

    // If use gui, create gui pointer
    if(useGui) {
        _gui = new GUI();
        _gui->show();
    }
    // Otherwise, cast as nullptr
    else {
        _gui = nullptr;
    }

    // Starting world
    _world->start();

    // Disabling world thread (loop() won't run anymore)
    _world->disableLoop();
}

void Core::stop() {
    // Stopping and waiting world
    _world->stopEntity();
    _world->wait();

    // Deleting world (it also delete all other entities added to it)
    delete _world;

    // Delete worldmap
    delete _worldMap;

    // If gui pointer is not null, close and delete it
    if(_gui != nullptr) {
        _gui->close();
        delete _gui;
    }
}

Constants* Core::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Core") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

WorldMap* Core::getWorldMap() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("WorldMap with nullptr value at Core") + '\n';
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
