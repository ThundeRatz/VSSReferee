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

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <src/utils/exithandler/exithandler.h>
#include <src/core.h>

QCoreApplication *createApplication(int &argc, char *argv[]) {
    // Try to found in args an '--gui'
    bool foundArg = false;
    for (int i = 0; i < argc; ++i) {
        if (!qstrcmp(argv[i], "--gui")) {
            foundArg = true;
            break;
        }
    }

    // if not found, call core application
    if(!foundArg) {
        return new QCoreApplication(argc, argv);
    }
    // otherwise, call gui application
    else {
        return new QApplication(argc, argv);
    }
}

int main(int argc, char *argv[]) {
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    app->setApplicationVersion(APP_VERSION);

    // Setup command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    // Setup application options
    // Use gui
    QCommandLineOption useGuiOption("gui", "Enable GUI mode");
    parser.addOption(useGuiOption);

    // Use gui
    QCommandLineOption use5v5Field("5v5", "Use 5v5 field parameters");
    parser.addOption(use5v5Field);

    // Process parser in app
    parser.process(*app);

    // Initiating constants
    QScopedPointer<Constants> constants(new Constants(QString(PROJECT_PATH) + "/src/constants/constants.json"));

    // Setup ExitHandler
    ExitHandler::setApplication(app.data());
    ExitHandler::setup();

    // Start Core
    QScopedPointer<Core> refereeCore(new Core(constants.data()));
    refereeCore->start(parser.isSet(useGuiOption), parser.isSet(use5v5Field));

    // Wait for app exec
    bool exec = app->exec();

    // Stop Core
    refereeCore->stop();

    return exec;
}
