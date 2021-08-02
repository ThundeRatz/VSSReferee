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

#include "color.h"

Color::Color(VSSRef::Color color) {
    _color = color;
}

VSSRef::Color Color::getColor() const {
    return _color;
}

Color Color::oppositeColor() const {
    return Color(_color == VSSRef::Color::BLUE ? VSSRef::Color::YELLOW : VSSRef::Color::BLUE);
}

bool Color::isBlue() const {
    return (_color == VSSRef::Color::BLUE);
}

bool Color::isYellow() const {
    return (_color == VSSRef::Color::YELLOW);
}

bool Color::isNone() const {
    return (_color == VSSRef::Color::NONE);
}

