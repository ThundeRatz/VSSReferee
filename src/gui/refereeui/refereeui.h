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

#ifndef REFEREEUI_H
#define REFEREEUI_H

#include <QMainWindow>

namespace Ui {
class RefereeUI;
}

class RefereeUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit RefereeUI(QWidget *parent = nullptr);
    ~RefereeUI();

private:
    Ui::RefereeUI *ui;
};

#endif // REFEREEUI_H
