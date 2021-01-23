QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += sdk_no_version_check
unix{
    QMAKE_CXXFLAGS += -std=c++1z -Wall -Wpedantic -Wextra
}
win32{
    QMAKE_CXXFLAGS +=  /std:c++17
}

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
#win32{
    # NOTE: Deploy the portaudio_x86.dll with the application!
#    LIBS += ../../../portaudio/msvc_build/msvc/Win32/Release/portaudio_x86.lib
    #C:\Users\DevNVME\Desktop\portaudiopp\portaudio\msvc_build\msvc\Win32\ReleaseMinDependency\portaudio.dll
#}

#LIBS += -lportaudio

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Dynamic linking in win32. Any linker errors?
# https://stackoverflow.com/questions/64233135/portaudio-test-application-unresolved-external-symbol-pa-getversioninfo
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../portaudio/msvc_build/msvc/Win32/ReleaseMinDependency/ -lportaudio_x86
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../portaudio/msvc_build/msvc/Win32/Debug/ -lportaudio_x86
#INCLUDEPATH += $$PWD/../../../portaudio/msvc_build/msvc/Win32/ReleaseMinDependency
#DEPENDPATH += $$PWD/../../../portaudio/msvc_build/msvc/Win32/ReleaseMinDependency
