TEMPLATE = lib

CONFIG += staticlib flat
TARGET = qogrewidget

SOURCES += \
  QOgreWidget.cpp \
  QOgreWindow.cpp

    
HEADERS += \
  PreProcessor.h \
  QOgreWidget.h \
  QtOgre.h \
  QOgreApplication.h \
  QOgreWindow.h \
  WindowEventListener.h

