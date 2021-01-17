#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "portaudio" for configuration "Debug"
set_property(TARGET portaudio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(portaudio PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/CoreAudio.framework;/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/AudioToolbox.framework;/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/AudioUnit.framework;/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/Carbon.framework;m;pthread"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libportaudio.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libportaudio.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS portaudio )
list(APPEND _IMPORT_CHECK_FILES_FOR_portaudio "${_IMPORT_PREFIX}/lib/libportaudio.dylib" )

# Import target "portaudio_static" for configuration "Debug"
set_property(TARGET portaudio_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(portaudio_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/CoreAudio.framework;/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/AudioToolbox.framework;/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/AudioUnit.framework;/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/System/Library/Frameworks/Carbon.framework;m;pthread"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libportaudio.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS portaudio_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_portaudio_static "${_IMPORT_PREFIX}/lib/libportaudio.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
