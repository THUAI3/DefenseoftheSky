# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aglove/desktop/DefenseoftheSky/AI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aglove/desktop/DefenseoftheSky/AI

# Include any dependencies generated for this target.
include CMakeFiles/sample_ai.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sample_ai.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sample_ai.dir/flags.make

CMakeFiles/sample_ai.dir/src/main.cpp.o: CMakeFiles/sample_ai.dir/flags.make
CMakeFiles/sample_ai.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aglove/desktop/DefenseoftheSky/AI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sample_ai.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sample_ai.dir/src/main.cpp.o -c /Users/aglove/desktop/DefenseoftheSky/AI/src/main.cpp

CMakeFiles/sample_ai.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sample_ai.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aglove/desktop/DefenseoftheSky/AI/src/main.cpp > CMakeFiles/sample_ai.dir/src/main.cpp.i

CMakeFiles/sample_ai.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sample_ai.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aglove/desktop/DefenseoftheSky/AI/src/main.cpp -o CMakeFiles/sample_ai.dir/src/main.cpp.s

CMakeFiles/sample_ai.dir/include/client/client.cpp.o: CMakeFiles/sample_ai.dir/flags.make
CMakeFiles/sample_ai.dir/include/client/client.cpp.o: include/client/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aglove/desktop/DefenseoftheSky/AI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sample_ai.dir/include/client/client.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sample_ai.dir/include/client/client.cpp.o -c /Users/aglove/desktop/DefenseoftheSky/AI/include/client/client.cpp

CMakeFiles/sample_ai.dir/include/client/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sample_ai.dir/include/client/client.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aglove/desktop/DefenseoftheSky/AI/include/client/client.cpp > CMakeFiles/sample_ai.dir/include/client/client.cpp.i

CMakeFiles/sample_ai.dir/include/client/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sample_ai.dir/include/client/client.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aglove/desktop/DefenseoftheSky/AI/include/client/client.cpp -o CMakeFiles/sample_ai.dir/include/client/client.cpp.s

CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.o: CMakeFiles/sample_ai.dir/flags.make
CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.o: include/jsoncpp/jsoncpp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aglove/desktop/DefenseoftheSky/AI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.o -c /Users/aglove/desktop/DefenseoftheSky/AI/include/jsoncpp/jsoncpp.cpp

CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aglove/desktop/DefenseoftheSky/AI/include/jsoncpp/jsoncpp.cpp > CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.i

CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aglove/desktop/DefenseoftheSky/AI/include/jsoncpp/jsoncpp.cpp -o CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.s

# Object files for target sample_ai
sample_ai_OBJECTS = \
"CMakeFiles/sample_ai.dir/src/main.cpp.o" \
"CMakeFiles/sample_ai.dir/include/client/client.cpp.o" \
"CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.o"

# External object files for target sample_ai
sample_ai_EXTERNAL_OBJECTS =

sample_ai: CMakeFiles/sample_ai.dir/src/main.cpp.o
sample_ai: CMakeFiles/sample_ai.dir/include/client/client.cpp.o
sample_ai: CMakeFiles/sample_ai.dir/include/jsoncpp/jsoncpp.cpp.o
sample_ai: CMakeFiles/sample_ai.dir/build.make
sample_ai: CMakeFiles/sample_ai.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/aglove/desktop/DefenseoftheSky/AI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable sample_ai"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sample_ai.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sample_ai.dir/build: sample_ai

.PHONY : CMakeFiles/sample_ai.dir/build

CMakeFiles/sample_ai.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sample_ai.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sample_ai.dir/clean

CMakeFiles/sample_ai.dir/depend:
	cd /Users/aglove/desktop/DefenseoftheSky/AI && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/aglove/desktop/DefenseoftheSky/AI /Users/aglove/desktop/DefenseoftheSky/AI /Users/aglove/desktop/DefenseoftheSky/AI /Users/aglove/desktop/DefenseoftheSky/AI /Users/aglove/desktop/DefenseoftheSky/AI/CMakeFiles/sample_ai.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sample_ai.dir/depend

