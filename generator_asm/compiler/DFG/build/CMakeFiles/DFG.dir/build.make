# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /snap/cmake/549/bin/cmake

# The command to remove a file.
RM = /snap/cmake/549/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/canesche/git/masters/place_route/compiler/DFG

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/canesche/git/masters/place_route/compiler/DFG/build

# Include any dependencies generated for this target.
include CMakeFiles/DFG.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DFG.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DFG.dir/flags.make

CMakeFiles/DFG.dir/src/DFGGeneration.cpp.o: CMakeFiles/DFG.dir/flags.make
CMakeFiles/DFG.dir/src/DFGGeneration.cpp.o: ../src/DFGGeneration.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/canesche/git/masters/place_route/compiler/DFG/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DFG.dir/src/DFGGeneration.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DFG.dir/src/DFGGeneration.cpp.o -c /home/canesche/git/masters/place_route/compiler/DFG/src/DFGGeneration.cpp

CMakeFiles/DFG.dir/src/DFGGeneration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DFG.dir/src/DFGGeneration.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/canesche/git/masters/place_route/compiler/DFG/src/DFGGeneration.cpp > CMakeFiles/DFG.dir/src/DFGGeneration.cpp.i

CMakeFiles/DFG.dir/src/DFGGeneration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DFG.dir/src/DFGGeneration.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/canesche/git/masters/place_route/compiler/DFG/src/DFGGeneration.cpp -o CMakeFiles/DFG.dir/src/DFGGeneration.cpp.s

# Object files for target DFG
DFG_OBJECTS = \
"CMakeFiles/DFG.dir/src/DFGGeneration.cpp.o"

# External object files for target DFG
DFG_EXTERNAL_OBJECTS =

libDFG.so: CMakeFiles/DFG.dir/src/DFGGeneration.cpp.o
libDFG.so: CMakeFiles/DFG.dir/build.make
libDFG.so: CMakeFiles/DFG.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/canesche/git/masters/place_route/compiler/DFG/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module libDFG.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DFG.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DFG.dir/build: libDFG.so

.PHONY : CMakeFiles/DFG.dir/build

CMakeFiles/DFG.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DFG.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DFG.dir/clean

CMakeFiles/DFG.dir/depend:
	cd /home/canesche/git/masters/place_route/compiler/DFG/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/canesche/git/masters/place_route/compiler/DFG /home/canesche/git/masters/place_route/compiler/DFG /home/canesche/git/masters/place_route/compiler/DFG/build /home/canesche/git/masters/place_route/compiler/DFG/build /home/canesche/git/masters/place_route/compiler/DFG/build/CMakeFiles/DFG.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DFG.dir/depend

