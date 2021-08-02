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

#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QObject>
#include <QTimer>

#include <src/constants/constants.h>
#include <src/utils/types/color/color.h>
#include <proto/vssref_packet.pb.h>

class GameInfo : public QObject
{
    Q_OBJECT
public:
    GameInfo(Constants *constants);

    // Half, timestamp and half kickoff management
    void setupNextStage();
    void updateTimeStamp();
    int getRemainingTime();
    int getStageTime();

    // Command management
    VSSRef::ref_to_team::VSSRef_Packet packet();
    void setPacket(VSSRef::ref_to_team::VSSRef_Packet packet);
    bool isGameOn();

private:
    // Game management
    VSSRef::ref_to_team::VSSRef_Packet _packet;
    Color _kickoffColor;
    QTimer _gameTimer;
    int _lastTimeStamp;

    // Constants
    Constants *_constants;
    Constants* getConstants();

public slots:
    void startGame();
    void stopGame();
};

#endif // GAMEINFO_H
