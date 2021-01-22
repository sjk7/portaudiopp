QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += sdk_no_version_check
QMAKE_CXXFLAGS += -std=c++1z -Wall -Wpedantic -Wextra

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "../../tdd/"
INCLUDEPATH += "../../../portaudio/include"

SOURCES += \
    dialog.cpp \
    main.cpp

HEADERS += \
    ../../../portaudio/include/portaudio.h \
    ../../tdd/portaudioplusplus.h \
    dialog.h

FORMS += \
    dialog.ui

macx {
    LIBS += ../../../portaudio/build/libportaudio.dylib
}
unix : !macx {
LIBS += -lportaudio
}
win32{
    LIBS += ../../../portaudio/build/libportaudio.a
}

#LIBS += -lportaudio

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
