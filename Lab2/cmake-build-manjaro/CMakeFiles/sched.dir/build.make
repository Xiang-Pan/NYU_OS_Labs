# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/clion

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/clion/cmake-build-manjaro

# Include any dependencies generated for this target.
include CMakeFiles/sched.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/sched.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sched.dir/flags.make

CMakeFiles/sched.dir/main.cpp.o: CMakeFiles/sched.dir/flags.make
CMakeFiles/sched.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/clion/cmake-build-manjaro/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sched.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sched.dir/main.cpp.o -c /tmp/clion/main.cpp

CMakeFiles/sched.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sched.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/clion/main.cpp > CMakeFiles/sched.dir/main.cpp.i

CMakeFiles/sched.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sched.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/clion/main.cpp -o CMakeFiles/sched.dir/main.cpp.s

# Object files for target sched
sched_OBJECTS = \
"CMakeFiles/sched.dir/main.cpp.o"

# External object files for target sched
sched_EXTERNAL_OBJECTS =

sched: CMakeFiles/sched.dir/main.cpp.o
sched: CMakeFiles/sched.dir/build.make
sched: CMakeFiles/sched.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/clion/cmake-build-manjaro/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sched"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sched.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sched.dir/build: sched
.PHONY : CMakeFiles/sched.dir/build

CMakeFiles/sched.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sched.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sched.dir/clean

CMakeFiles/sched.dir/depend:
	cd /tmp/clion/cmake-build-manjaro && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/clion /tmp/clion /tmp/clion/cmake-build-manjaro /tmp/clion/cmake-build-manjaro /tmp/clion/cmake-build-manjaro/CMakeFiles/sched.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sched.dir/depend

