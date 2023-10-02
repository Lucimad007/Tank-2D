QT       += core gui uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blocks-event-filter.cpp \
    construction.cpp \
    game-object.cpp \
    game.cpp \
    main.cpp \
    menu-event.cpp \
    multi-player.cpp \
    register.cpp \
    sprite-loader.cpp \
    user.cpp

HEADERS += \
    blocks-event-filter.h \
    construction.h \
    game-object.h \
    game.h \
    menu-event.h \
    multi-player.h \
    register.h \
    sprite-loader.h \
    user.h

FORMS += \
    construct.ui \
    construction.ui \
    custom-level.ui \
    game.ui \
    menu.ui \
    multi-player.ui \
    register.ui \
    score-board.ui \
    select-level.ui \
    winner.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Arts.qrc \
    levels.qrc
