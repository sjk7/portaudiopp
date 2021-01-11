# portaudiopp
Comprehensive Modern c++ wrapper for PortAudio. Tested on Linux and Windows. And if I can ever afford one, a MAC.
This library requires at least C++17 standard, and is built using CMake.

Owing to some "upstream" bug in Linux under Pulseaudio (probably the most universally used sound system: it's the default for Ubuntu-based distros)
I have had to modify portaudio in order to return some elapsed duration in
the audio callback. At least for this reason, please use the packaged portaudio instead of the system one. It will require static linking. 

This means you have to build and reference the portaudio included with this project, not your system one.

I developed this using VSCode on Linux Mint 19.


