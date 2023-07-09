QT       += core gui uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game-object.cpp \
    game.cpp \
    main.cpp \
    menu_event.cpp \
    register.cpp \
    sprite-loader.cpp

HEADERS += \
    game-object.h \
    game.h \
    menu_event.h \
    register.h \
    sprite-loader.h

FORMS += \
    game.ui \
    menu.ui \
    register.ui \
    select-level.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
