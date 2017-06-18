QT += core gui widgets serialport
CONFIG += c++11
TARGET = schem
TEMPLATE = app
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = ../exe/

INCLUDEPATH += \
    sources \
    iskra

SOURCES += \
    main.cpp\
    window.cpp \
    # iskra/iskra.cpp \
    sources/chip_widget.cpp \
    sources/duino.cpp \
    sources/duino_widget.cpp \
    sources/logger_widget.cpp \
    sources/globals.cpp \
    sources/pin_widget.cpp \
    sources/chip_selector_widget.cpp \
    sources/ugo.cpp \
    sources/ugo_widget.cpp

HEADERS += \
    window.h \
    iskra/protocol.h \
    iskra/iskra.ino \
    # iskra/iskra.h \
    sources/chip.h \
    sources/chip_widget.h \
    sources/duino.h \
    sources/pin.h \
    sources/duino_widget.h \
    sources/logger_widget.h \
    sources/globals.h \
    sources/pin_widget.h \
    sources/chip_selector_widget.h \
    sources/ugo.h \
    sources/ugo_widget.h

RESOURCES += \
    res.qrc
