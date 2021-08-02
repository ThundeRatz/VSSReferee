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

#include "gameinfo.h"

#include <ctime>
#include <random>

GameInfo::GameInfo(Constants *constants) {
    // Setup constants
    _constants = constants;

    // Create mt random
    std::mt19937 mt(time(nullptr));

    // Setup initial values in command
    _packet.set_gamestage(VSSRef::Stage::TEST_HALF);
    _packet.set_command(VSSRef::Command::KICKOFF);
    _packet.set_commandcolor(VSSRef::Color::BLUE);
    _packet.set_commandquadrant(VSSRef::QUADRANT_NONE);
    _packet.set_timestamp(0.0f);

    // Setup initial kickoff color using mt random
    _kickoffColor = Color(VSSRef::Color(mt() % 2)); // Blue or Yellow
}

void GameInfo::setupNextStage() {
    // Swapping to next half and taking name
    _packet.set_gamestage(VSSRef::Stage(_packet.gamestage() + 1));
    QString nextHalfName = VSSRef::Stage_Name(_packet.gamestage()).c_str();

    /// TODO: test this later
    /// Basically: in every next stage, set an HALT command... wait for manual
    /// referee to start =)
    _packet.set_command(VSSRef::Command::HALT);
    _packet.set_commandcolor(VSSRef::Color::NONE);
    _packet.set_commandquadrant(VSSRef::Quadrant::QUADRANT_NONE);

    // Setup timestamp
    _lastTimeStamp = getStageTime();
    _packet.set_timestamp(0.0f);
}

void GameInfo::updateTimeStamp() {
    // Update _lastTimeStamp and set it to packet
    _lastTimeStamp = getStageTime() - getRemainingTime();
    _packet.set_timestamp(_lastTimeStamp);

    // If not remaining time, setup next half.
    if(getRemainingTime() <= 0) {
        setupNextStage();
    }
}

int GameInfo::getRemainingTime() {
    return _gameTimer.remainingTime();
}

int GameInfo::getStageTime() {
    // Take half name
    QString halfName = VSSRef::Stage_Name(_packet.gamestage()).c_str();

    /// TODO: check with Adam to define this time
    // If is an prepare half
    if(halfName.contains("PRE")) {
        return 5;
    }
    /// TODO: check with Adam how to define this time
    // If is penalty shootout
    else if(halfName.contains("PENALTY")) {
        return 10;
    }
    // Check if is overtime
    else if(halfName.contains("OVERTIME")){
        return getConstants()->overtimeHalfTime();
    }
    /// TODO: check how to turn this into inf later
    // Check if is TEST_HALF
    else if(halfName.contains("TEST")) {
        return -1;
    }
    // Else, it is FIRST / SECOND half
    else {
        return getConstants()->halfTime();
    }
}

VSSRef::ref_to_team::VSSRef_Packet GameInfo::packet() {
    return _packet;
}

void GameInfo::setPacket(VSSRef::ref_to_team::VSSRef_Packet packet) {
    _packet = packet;
    /// TODO: set timestamp here (?)
}

bool GameInfo::isGameOn() {
    return (_packet.command() == VSSRef::Command::GAME_ON);
}

void GameInfo::startGame() {
    _gameTimer.start(_lastTimeStamp);
}

void GameInfo::stopGame() {
    _lastTimeStamp = getRemainingTime();
    _gameTimer.stop();
}

Constants* GameInfo::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at GameInfo") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
