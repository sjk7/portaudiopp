# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.19.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.19.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/stevekerr/Documents/portaudiopp/portaudio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/stevekerr/Documents/portaudiopp/portaudio/build

# Include any dependencies generated for this target.
include CMakeFiles/portaudio.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/portaudio.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/portaudio.dir/flags.make

CMakeFiles/portaudio.dir/src/common/pa_allocation.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_allocation.c.o: ../src/common/pa_allocation.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/portaudio.dir/src/common/pa_allocation.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_allocation.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_allocation.c

CMakeFiles/portaudio.dir/src/common/pa_allocation.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_allocation.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_allocation.c > CMakeFiles/portaudio.dir/src/common/pa_allocation.c.i

CMakeFiles/portaudio.dir/src/common/pa_allocation.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_allocation.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_allocation.c -o CMakeFiles/portaudio.dir/src/common/pa_allocation.c.s

CMakeFiles/portaudio.dir/src/common/pa_converters.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_converters.c.o: ../src/common/pa_converters.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/portaudio.dir/src/common/pa_converters.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_converters.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_converters.c

CMakeFiles/portaudio.dir/src/common/pa_converters.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_converters.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_converters.c > CMakeFiles/portaudio.dir/src/common/pa_converters.c.i

CMakeFiles/portaudio.dir/src/common/pa_converters.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_converters.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_converters.c -o CMakeFiles/portaudio.dir/src/common/pa_converters.c.s

CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.o: ../src/common/pa_cpuload.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_cpuload.c

CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_cpuload.c > CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.i

CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_cpuload.c -o CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.s

CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.o: ../src/common/pa_debugprint.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_debugprint.c

CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_debugprint.c > CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.i

CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_debugprint.c -o CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.s

CMakeFiles/portaudio.dir/src/common/pa_dither.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_dither.c.o: ../src/common/pa_dither.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/portaudio.dir/src/common/pa_dither.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_dither.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_dither.c

CMakeFiles/portaudio.dir/src/common/pa_dither.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_dither.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_dither.c > CMakeFiles/portaudio.dir/src/common/pa_dither.c.i

CMakeFiles/portaudio.dir/src/common/pa_dither.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_dither.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_dither.c -o CMakeFiles/portaudio.dir/src/common/pa_dither.c.s

CMakeFiles/portaudio.dir/src/common/pa_front.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_front.c.o: ../src/common/pa_front.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/portaudio.dir/src/common/pa_front.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_front.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_front.c

CMakeFiles/portaudio.dir/src/common/pa_front.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_front.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_front.c > CMakeFiles/portaudio.dir/src/common/pa_front.c.i

CMakeFiles/portaudio.dir/src/common/pa_front.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_front.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_front.c -o CMakeFiles/portaudio.dir/src/common/pa_front.c.s

CMakeFiles/portaudio.dir/src/common/pa_process.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_process.c.o: ../src/common/pa_process.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/portaudio.dir/src/common/pa_process.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_process.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_process.c

CMakeFiles/portaudio.dir/src/common/pa_process.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_process.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_process.c > CMakeFiles/portaudio.dir/src/common/pa_process.c.i

CMakeFiles/portaudio.dir/src/common/pa_process.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_process.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_process.c -o CMakeFiles/portaudio.dir/src/common/pa_process.c.s

CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.o: ../src/common/pa_ringbuffer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_ringbuffer.c

CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_ringbuffer.c > CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.i

CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_ringbuffer.c -o CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.s

CMakeFiles/portaudio.dir/src/common/pa_stream.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_stream.c.o: ../src/common/pa_stream.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/portaudio.dir/src/common/pa_stream.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_stream.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_stream.c

CMakeFiles/portaudio.dir/src/common/pa_stream.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_stream.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_stream.c > CMakeFiles/portaudio.dir/src/common/pa_stream.c.i

CMakeFiles/portaudio.dir/src/common/pa_stream.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_stream.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_stream.c -o CMakeFiles/portaudio.dir/src/common/pa_stream.c.s

CMakeFiles/portaudio.dir/src/common/pa_trace.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/common/pa_trace.c.o: ../src/common/pa_trace.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/portaudio.dir/src/common/pa_trace.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/common/pa_trace.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_trace.c

CMakeFiles/portaudio.dir/src/common/pa_trace.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/common/pa_trace.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_trace.c > CMakeFiles/portaudio.dir/src/common/pa_trace.c.i

CMakeFiles/portaudio.dir/src/common/pa_trace.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/common/pa_trace.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/common/pa_trace.c -o CMakeFiles/portaudio.dir/src/common/pa_trace.c.s

CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.o: ../src/hostapi/skeleton/pa_hostapi_skeleton.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/skeleton/pa_hostapi_skeleton.c

CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/skeleton/pa_hostapi_skeleton.c > CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.i

CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/skeleton/pa_hostapi_skeleton.c -o CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.s

CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.o: ../src/os/unix/pa_unix_hostapis.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/os/unix/pa_unix_hostapis.c

CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/os/unix/pa_unix_hostapis.c > CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.i

CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/os/unix/pa_unix_hostapis.c -o CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.s

CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.o: ../src/os/unix/pa_unix_util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/os/unix/pa_unix_util.c

CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/os/unix/pa_unix_util.c > CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.i

CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/os/unix/pa_unix_util.c -o CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.s

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.o: ../src/hostapi/coreaudio/pa_mac_core.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core.c

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core.c > CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.i

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core.c -o CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.s

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.o: ../src/hostapi/coreaudio/pa_mac_core_blocking.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core_blocking.c

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core_blocking.c > CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.i

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core_blocking.c -o CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.s

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.o: CMakeFiles/portaudio.dir/flags.make
CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.o: ../src/hostapi/coreaudio/pa_mac_core_utilities.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.o -c /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core_utilities.c

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core_utilities.c > CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.i

CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/stevekerr/Documents/portaudiopp/portaudio/src/hostapi/coreaudio/pa_mac_core_utilities.c -o CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.s

# Object files for target portaudio
portaudio_OBJECTS = \
"CMakeFiles/portaudio.dir/src/common/pa_allocation.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_converters.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_dither.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_front.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_process.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_stream.c.o" \
"CMakeFiles/portaudio.dir/src/common/pa_trace.c.o" \
"CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.o" \
"CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.o" \
"CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.o" \
"CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.o" \
"CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.o" \
"CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.o"

# External object files for target portaudio
portaudio_EXTERNAL_OBJECTS =

libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_allocation.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_converters.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_cpuload.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_debugprint.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_dither.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_front.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_process.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_ringbuffer.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_stream.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/common/pa_trace.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/hostapi/skeleton/pa_hostapi_skeleton.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/os/unix/pa_unix_hostapis.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/os/unix/pa_unix_util.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_blocking.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/src/hostapi/coreaudio/pa_mac_core_utilities.c.o
libportaudio.dylib: CMakeFiles/portaudio.dir/build.make
libportaudio.dylib: CMakeFiles/portaudio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking C shared library libportaudio.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/portaudio.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/portaudio.dir/build: libportaudio.dylib

.PHONY : CMakeFiles/portaudio.dir/build

CMakeFiles/portaudio.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/portaudio.dir/cmake_clean.cmake
.PHONY : CMakeFiles/portaudio.dir/clean

CMakeFiles/portaudio.dir/depend:
	cd /Users/stevekerr/Documents/portaudiopp/portaudio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/stevekerr/Documents/portaudiopp/portaudio /Users/stevekerr/Documents/portaudiopp/portaudio /Users/stevekerr/Documents/portaudiopp/portaudio/build /Users/stevekerr/Documents/portaudiopp/portaudio/build /Users/stevekerr/Documents/portaudiopp/portaudio/build/CMakeFiles/portaudio.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/portaudio.dir/depend

