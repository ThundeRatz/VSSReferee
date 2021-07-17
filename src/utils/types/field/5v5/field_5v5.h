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

#ifndef FIELD_5V5_H
#define FIELD_5V5_H

#include <src/utils/types/field/field.h>

class Field_5v5 : public Field
{
public:
    Field_5v5();
    QString name() const;

    // Field dimensions
    float length() const;
    float width() const;

    // Goal dimensions
    float goalWidth() const;
    float goalDepth() const;

    // Center dimensions
    float centerRadius() const;

    // Defense area dimensions
    float defenseRadius() const;
    float defenseStretch() const;

};

#endif // FIELD_5V5_H
