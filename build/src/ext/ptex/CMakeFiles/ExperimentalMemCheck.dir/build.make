# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.13.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.13.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/valentin/Desktop/Workspace/pbrt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/valentin/Desktop/Workspace/pbrt/build

# Utility rule file for ExperimentalMemCheck.

# Include the progress variables for this target.
include src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/progress.make

src/ext/ptex/CMakeFiles/ExperimentalMemCheck:
	cd /Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex && /usr/local/Cellar/cmake/3.13.2/bin/ctest -D ExperimentalMemCheck

ExperimentalMemCheck: src/ext/ptex/CMakeFiles/ExperimentalMemCheck
ExperimentalMemCheck: src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/build.make

.PHONY : ExperimentalMemCheck

# Rule to build all files generated by this target.
src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/build: ExperimentalMemCheck

.PHONY : src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/build

src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/clean:
	cd /Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex && $(CMAKE_COMMAND) -P CMakeFiles/ExperimentalMemCheck.dir/cmake_clean.cmake
.PHONY : src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/clean

src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/depend:
	cd /Users/valentin/Desktop/Workspace/pbrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/valentin/Desktop/Workspace/pbrt /Users/valentin/Desktop/Workspace/pbrt/src/ext/ptex /Users/valentin/Desktop/Workspace/pbrt/build /Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex /Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ext/ptex/CMakeFiles/ExperimentalMemCheck.dir/depend

