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

#include "field_3v3.h"

Field_3v3::Field_3v3() {

}

QString Field_3v3::name() const {
    return "Field_3v3";
}

float Field_3v3::length() const {
    return 1.50f;
}

float Field_3v3::width() const {
    return 1.30f;
}

float Field_3v3::goalWidth() const {
    return 0.40f;
}

float Field_3v3::goalDepth() const {
    return 0.10f;
}

float Field_3v3::centerRadius() const {
    return 0.20f;
}

float Field_3v3::defenseRadius() const {
    return 0.15f;
}

float Field_3v3::defenseStretch() const {
    return 0.7f;
}
