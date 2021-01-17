QT -= gui

TEMPLATE = lib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../include

HEADERS += ../include/*.h \
    ../src/common/pa_allocation.h \
    ../src/common/pa_converters.h \
    ../src/common/pa_cpuload.h \
    ../src/common/pa_debugprint.h \
    ../src/common/pa_dither.h \
    ../src/common/pa_endianness.h \
    ../src/common/pa_gitrevision.h \
    ../src/common/pa_hostapi.h \
    ../src/common/pa_memorybarrier.h \
    ../src/common/pa_process.h \
    ../src/common/pa_ringbuffer.h \
    ../src/common/pa_stream.h \
    ../src/common/pa_trace.h \
    ../src/common/pa_types.h \
    ../src/common/pa_util.h \
    ../src/hostapi/coreaudio/pa_mac_core_blocking.h \
    ../src/hostapi/coreaudio/pa_mac_core_internal.h \
    ../src/hostapi/coreaudio/pa_mac_core_utilities.h \
    ../src/os/unix/pa_unix_util.h



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    ../src/common/pa_allocation.c \
    ../src/common/pa_converters.c \
    ../src/common/pa_cpuload.c \
    ../src/common/pa_debugprint.c \
    ../src/common/pa_dither.c \
    ../src/common/pa_front.c \
    ../src/common/pa_process.c \
    ../src/common/pa_ringbuffer.c \
    ../src/common/pa_stream.c \
    ../src/common/pa_trace.c \
    ../src/hostapi/coreaudio/pa_mac_core.c \
    ../src/hostapi/coreaudio/pa_mac_core_blocking.c \
    ../src/hostapi/coreaudio/pa_mac_core_old.c \
    ../src/hostapi/coreaudio/pa_mac_core_utilities.c \
    ../src/hostapi/oss/pa_unix_oss.c \
    ../src/hostapi/oss/recplay.c \
    ../src/os/unix/pa_unix_util.c
