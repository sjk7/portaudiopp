#ifndef PORTAUDIO_GLOBAL_H
#define PORTAUDIO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PORTAUDIO_LIBRARY)
#  define PORTAUDIO_EXPORT Q_DECL_EXPORT
#else
#  define PORTAUDIO_EXPORT Q_DECL_IMPORT
#endif

#endif // PORTAUDIO_GLOBAL_H
