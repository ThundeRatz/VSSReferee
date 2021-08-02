# Qt libs to import
QT += core    \
      gui     \
      widgets \
      network \
      opengl

# Project configs
TEMPLATE = app
DESTDIR  = ../bin
TARGET   = VSSSReferee
VERSION  = 3.0.0

CONFIG += c++14 console
CONFIG -= app_bundle

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

# Project libs
LIBS *= -lprotobuf -lQt5Core -lGLU

# Compiling .proto files
system(echo "Compiling protobuf files" && cd proto && cmake . && make)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += PROJECT_PATH=\\\"$${PWD}\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        proto/command.pb.cc \
        proto/common.pb.cc \
        proto/packet.pb.cc \
        proto/replacement.pb.cc \
        proto/vssref_common.pb.cc \
        proto/vssref_packet.pb.cc \
        proto/vssref_placement.pb.cc \
        src/constants/constants.cpp \
        src/core.cpp \
        src/entities/entity.cpp \
        src/entities/referee/fouls/foul.cpp \
        src/entities/referee/gameinfo/gameinfo.cpp \
        src/entities/referee/referee.cpp \
        src/entities/replacer/replacer.cpp \
        src/entities/vision/filters/kalman/kalmanfilter.cpp \
        src/entities/vision/filters/kalman/kalmanstate/kalmanstate.cpp \
        src/entities/vision/filters/loss/lossfilter.cpp \
        src/entities/vision/filters/noise/noisefilter.cpp \
        src/entities/vision/vision.cpp \
        src/entities/world/world.cpp \
        src/entities/world/worldmap/locations/locations.cpp \
        src/entities/world/worldmap/worldmap.cpp \
        src/gui/gui.cpp \
        src/gui/refereeui/refereeui.cpp \
        src/utils/exithandler/exithandler.cpp \
        src/utils/text/text.cpp \
        src/utils/timer/timer.cpp \
        src/utils/types/angle/angle.cpp \
        src/utils/types/angularspeed/angularspeed.cpp \
        src/utils/types/color/color.cpp \
        src/utils/types/field/3v3/field_3v3.cpp \
        src/utils/types/field/5v5/field_5v5.cpp \
        src/utils/types/field/field.cpp \
        src/utils/types/fieldside/fieldside.cpp \
        src/utils/types/matrix/matrix.cpp \
        src/utils/types/object/object.cpp \
        src/utils/types/position/position.cpp \
        src/utils/types/velocity/velocity.cpp \
        src/utils/types/wall/wall.cpp \
        src/utils/utils.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    proto/command.pb.h \
    proto/common.pb.h \
    proto/packet.pb.h \
    proto/replacement.pb.h \
    proto/vssref_common.pb.h \
    proto/vssref_packet.pb.h \
    proto/vssref_placement.pb.h \
    src/core.h \
    src/entities/entity.h \
    src/entities/referee/fouls/foul.h \
    src/entities/referee/gameinfo/gameinfo.h \
    src/entities/referee/referee.h \
    src/entities/replacer/replacer.h \
    src/entities/vision/filters/kalman/kalmanfilter.h \
    src/entities/vision/filters/kalman/kalmanstate/kalmanstate.h \
    src/entities/vision/filters/loss/lossfilter.h \
    src/entities/vision/filters/noise/noisefilter.h \
    src/entities/vision/filters/visionfilters.h \
    src/entities/vision/vision.h \
    src/entities/world/world.h \
    src/entities/world/worldmap/locations/locations.h \
    src/entities/world/worldmap/worldmap.h \
    src/gui/gui.h \
    src/gui/refereeui/refereeui.h \
    src/utils/exithandler/exithandler.h \
    src/utils/text/text.h \
    src/utils/timer/timer.h \
    src/utils/types/angle/angle.h \
    src/utils/types/angularspeed/angularspeed.h \
    src/utils/types/color/color.h \
    src/utils/types/field/3v3/field_3v3.h \
    src/utils/types/field/5v5/field_5v5.h \
    src/utils/types/field/field.h \
    src/utils/types/fieldside/fieldside.h \
    src/utils/types/fieldside/side.h \
    src/utils/types/matrix/matrix.h \
    src/utils/types/object/object.h \
    src/utils/types/position/position.h \
    src/utils/types/velocity/velocity.h \
    src/utils/types/wall/wall.h \
    src/utils/utils.h

FORMS += \
    src/gui/gui.ui \
    src/gui/refereeui/refereeui.ui
