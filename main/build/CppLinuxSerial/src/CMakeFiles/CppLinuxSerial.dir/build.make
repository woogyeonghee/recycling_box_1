# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jetson/recycle_Box

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jetson/recycle_Box/build

# Include any dependencies generated for this target.
include CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/depend.make

# Include the progress variables for this target.
include CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/progress.make

# Include the compile flags for this target's objects.
include CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/flags.make

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/flags.make
CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o: ../CppLinuxSerial/src/SerialPort.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jetson/recycle_Box/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o"
	cd /home/jetson/recycle_Box/build/CppLinuxSerial/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o -c /home/jetson/recycle_Box/CppLinuxSerial/src/SerialPort.cpp

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.i"
	cd /home/jetson/recycle_Box/build/CppLinuxSerial/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jetson/recycle_Box/CppLinuxSerial/src/SerialPort.cpp > CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.i

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.s"
	cd /home/jetson/recycle_Box/build/CppLinuxSerial/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jetson/recycle_Box/CppLinuxSerial/src/SerialPort.cpp -o CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.s

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.requires:

.PHONY : CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.requires

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.provides: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.requires
	$(MAKE) -f CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/build.make CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.provides.build
.PHONY : CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.provides

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.provides.build: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o


# Object files for target CppLinuxSerial
CppLinuxSerial_OBJECTS = \
"CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o"

# External object files for target CppLinuxSerial
CppLinuxSerial_EXTERNAL_OBJECTS =

CppLinuxSerial/src/libCppLinuxSerial.a: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o
CppLinuxSerial/src/libCppLinuxSerial.a: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/build.make
CppLinuxSerial/src/libCppLinuxSerial.a: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jetson/recycle_Box/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libCppLinuxSerial.a"
	cd /home/jetson/recycle_Box/build/CppLinuxSerial/src && $(CMAKE_COMMAND) -P CMakeFiles/CppLinuxSerial.dir/cmake_clean_target.cmake
	cd /home/jetson/recycle_Box/build/CppLinuxSerial/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CppLinuxSerial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/build: CppLinuxSerial/src/libCppLinuxSerial.a

.PHONY : CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/build

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/requires: CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/SerialPort.cpp.o.requires

.PHONY : CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/requires

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/clean:
	cd /home/jetson/recycle_Box/build/CppLinuxSerial/src && $(CMAKE_COMMAND) -P CMakeFiles/CppLinuxSerial.dir/cmake_clean.cmake
.PHONY : CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/clean

CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/depend:
	cd /home/jetson/recycle_Box/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jetson/recycle_Box /home/jetson/recycle_Box/CppLinuxSerial/src /home/jetson/recycle_Box/build /home/jetson/recycle_Box/build/CppLinuxSerial/src /home/jetson/recycle_Box/build/CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CppLinuxSerial/src/CMakeFiles/CppLinuxSerial.dir/depend

