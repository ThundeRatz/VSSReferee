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

#ifndef OBJECT_H
#define OBJECT_H

#include <src/utils/types/position/position.h>
#include <src/utils/types/velocity/velocity.h>
#include <src/utils/types/angle/angle.h>
#include <src/utils/types/angularspeed/angularspeed.h>
#include <src/entities/vision/filters/visionfilters.h>

class Object
{
public:
    Object();
    ~Object();

    // Getters
    Position getPosition();
    Velocity getVelocity();
    Angle getOrientation();
    AngularSpeed getAngularSpeed();

    // Filtering
    bool isObjectSafe();
    bool isObjectLoss();

    // Update
    void updateObject(float confidence, Position pos, Angle orientation = Angle(false, 0.0));
    void setInvalid();

private:
    // Object params
    Position _position;
    Velocity _velocity;
    Angle _orientation;
    AngularSpeed _angularSpeed;
    float _confidence;

    // Timers for angularSpeed
    Timer _aSpeedTimer;

    // Object filters
    LossFilter _lossFilter;
    NoiseFilter _noiseFilter;
    KalmanFilter _kalmanFilter;
};

#endif // OBJECT_H
