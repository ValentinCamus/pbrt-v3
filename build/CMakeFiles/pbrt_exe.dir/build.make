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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/valentin/Desktop/Workspace/pbrt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/valentin/Desktop/Workspace/pbrt/build

# Include any dependencies generated for this target.
include CMakeFiles/pbrt_exe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pbrt_exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pbrt_exe.dir/flags.make

CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.o: CMakeFiles/pbrt_exe.dir/flags.make
CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.o: ../src/main/pbrt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valentin/Desktop/Workspace/pbrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.o -c /Users/valentin/Desktop/Workspace/pbrt/src/main/pbrt.cpp

CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valentin/Desktop/Workspace/pbrt/src/main/pbrt.cpp > CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.i

CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valentin/Desktop/Workspace/pbrt/src/main/pbrt.cpp -o CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.s

# Object files for target pbrt_exe
pbrt_exe_OBJECTS = \
"CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.o"

# External object files for target pbrt_exe
pbrt_exe_EXTERNAL_OBJECTS =

pbrt: CMakeFiles/pbrt_exe.dir/src/main/pbrt.cpp.o
pbrt: CMakeFiles/pbrt_exe.dir/build.make
pbrt: libpbrt.a
pbrt: src/ext/openexr/OpenEXR/IlmImf/libIlmImf.a
pbrt: src/ext/openexr/IlmBase/Imath/libImath.a
pbrt: src/ext/openexr/IlmBase/Half/libHalf.a
pbrt: src/ext/glog/libglog.a
pbrt: src/ext/ptex/src/ptex/libPtex.a
pbrt: /usr/lib/libz.dylib
pbrt: src/ext/openexr/IlmBase/IlmThread/libIlmThread.a
pbrt: src/ext/openexr/IlmBase/Iex/libIex.a
pbrt: /usr/lib/libz.dylib
pbrt: CMakeFiles/pbrt_exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/valentin/Desktop/Workspace/pbrt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pbrt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pbrt_exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pbrt_exe.dir/build: pbrt

.PHONY : CMakeFiles/pbrt_exe.dir/build

CMakeFiles/pbrt_exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pbrt_exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pbrt_exe.dir/clean

CMakeFiles/pbrt_exe.dir/depend:
	cd /Users/valentin/Desktop/Workspace/pbrt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/valentin/Desktop/Workspace/pbrt /Users/valentin/Desktop/Workspace/pbrt /Users/valentin/Desktop/Workspace/pbrt/build /Users/valentin/Desktop/Workspace/pbrt/build /Users/valentin/Desktop/Workspace/pbrt/build/CMakeFiles/pbrt_exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pbrt_exe.dir/depend

