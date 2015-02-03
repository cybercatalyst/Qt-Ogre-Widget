QT += widgets opengl

unix {
    QT += x11extras
}

TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ogrewidget.cpp
    
HEADERS += \
    ogrewidget.h


