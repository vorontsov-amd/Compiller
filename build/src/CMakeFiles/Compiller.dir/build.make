# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/amadey/Projects/Compiller

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amadey/Projects/Compiller/build

# Include any dependencies generated for this target.
include src/CMakeFiles/Compiller.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/Compiller.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/Compiller.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/Compiller.dir/flags.make

src/CMakeFiles/Compiller.dir/ByteArray.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/ByteArray.cpp.o: /home/amadey/Projects/Compiller/src/ByteArray.cpp
src/CMakeFiles/Compiller.dir/ByteArray.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/Compiller.dir/ByteArray.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/ByteArray.cpp.o -MF CMakeFiles/Compiller.dir/ByteArray.cpp.o.d -o CMakeFiles/Compiller.dir/ByteArray.cpp.o -c /home/amadey/Projects/Compiller/src/ByteArray.cpp

src/CMakeFiles/Compiller.dir/ByteArray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/ByteArray.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/ByteArray.cpp > CMakeFiles/Compiller.dir/ByteArray.cpp.i

src/CMakeFiles/Compiller.dir/ByteArray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/ByteArray.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/ByteArray.cpp -o CMakeFiles/Compiller.dir/ByteArray.cpp.s

src/CMakeFiles/Compiller.dir/Compiller.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/Compiller.cpp.o: /home/amadey/Projects/Compiller/src/Compiller.cpp
src/CMakeFiles/Compiller.dir/Compiller.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/Compiller.dir/Compiller.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/Compiller.cpp.o -MF CMakeFiles/Compiller.dir/Compiller.cpp.o.d -o CMakeFiles/Compiller.dir/Compiller.cpp.o -c /home/amadey/Projects/Compiller/src/Compiller.cpp

src/CMakeFiles/Compiller.dir/Compiller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/Compiller.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/Compiller.cpp > CMakeFiles/Compiller.dir/Compiller.cpp.i

src/CMakeFiles/Compiller.dir/Compiller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/Compiller.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/Compiller.cpp -o CMakeFiles/Compiller.dir/Compiller.cpp.s

src/CMakeFiles/Compiller.dir/data_t.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/data_t.cpp.o: /home/amadey/Projects/Compiller/src/data_t.cpp
src/CMakeFiles/Compiller.dir/data_t.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/Compiller.dir/data_t.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/data_t.cpp.o -MF CMakeFiles/Compiller.dir/data_t.cpp.o.d -o CMakeFiles/Compiller.dir/data_t.cpp.o -c /home/amadey/Projects/Compiller/src/data_t.cpp

src/CMakeFiles/Compiller.dir/data_t.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/data_t.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/data_t.cpp > CMakeFiles/Compiller.dir/data_t.cpp.i

src/CMakeFiles/Compiller.dir/data_t.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/data_t.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/data_t.cpp -o CMakeFiles/Compiller.dir/data_t.cpp.s

src/CMakeFiles/Compiller.dir/Differentiator.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/Differentiator.cpp.o: /home/amadey/Projects/Compiller/src/Differentiator.cpp
src/CMakeFiles/Compiller.dir/Differentiator.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/Compiller.dir/Differentiator.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/Differentiator.cpp.o -MF CMakeFiles/Compiller.dir/Differentiator.cpp.o.d -o CMakeFiles/Compiller.dir/Differentiator.cpp.o -c /home/amadey/Projects/Compiller/src/Differentiator.cpp

src/CMakeFiles/Compiller.dir/Differentiator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/Differentiator.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/Differentiator.cpp > CMakeFiles/Compiller.dir/Differentiator.cpp.i

src/CMakeFiles/Compiller.dir/Differentiator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/Differentiator.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/Differentiator.cpp -o CMakeFiles/Compiller.dir/Differentiator.cpp.s

src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.o: /home/amadey/Projects/Compiller/src/lexical_analysis.cpp
src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.o -MF CMakeFiles/Compiller.dir/lexical_analysis.cpp.o.d -o CMakeFiles/Compiller.dir/lexical_analysis.cpp.o -c /home/amadey/Projects/Compiller/src/lexical_analysis.cpp

src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/lexical_analysis.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/lexical_analysis.cpp > CMakeFiles/Compiller.dir/lexical_analysis.cpp.i

src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/lexical_analysis.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/lexical_analysis.cpp -o CMakeFiles/Compiller.dir/lexical_analysis.cpp.s

src/CMakeFiles/Compiller.dir/main.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/main.cpp.o: /home/amadey/Projects/Compiller/src/main.cpp
src/CMakeFiles/Compiller.dir/main.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/Compiller.dir/main.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/main.cpp.o -MF CMakeFiles/Compiller.dir/main.cpp.o.d -o CMakeFiles/Compiller.dir/main.cpp.o -c /home/amadey/Projects/Compiller/src/main.cpp

src/CMakeFiles/Compiller.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/main.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/main.cpp > CMakeFiles/Compiller.dir/main.cpp.i

src/CMakeFiles/Compiller.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/main.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/main.cpp -o CMakeFiles/Compiller.dir/main.cpp.s

src/CMakeFiles/Compiller.dir/Reading.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/Reading.cpp.o: /home/amadey/Projects/Compiller/src/Reading.cpp
src/CMakeFiles/Compiller.dir/Reading.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/Compiller.dir/Reading.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/Reading.cpp.o -MF CMakeFiles/Compiller.dir/Reading.cpp.o.d -o CMakeFiles/Compiller.dir/Reading.cpp.o -c /home/amadey/Projects/Compiller/src/Reading.cpp

src/CMakeFiles/Compiller.dir/Reading.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/Reading.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/Reading.cpp > CMakeFiles/Compiller.dir/Reading.cpp.i

src/CMakeFiles/Compiller.dir/Reading.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/Reading.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/Reading.cpp -o CMakeFiles/Compiller.dir/Reading.cpp.s

src/CMakeFiles/Compiller.dir/Variable.cpp.o: src/CMakeFiles/Compiller.dir/flags.make
src/CMakeFiles/Compiller.dir/Variable.cpp.o: /home/amadey/Projects/Compiller/src/Variable.cpp
src/CMakeFiles/Compiller.dir/Variable.cpp.o: src/CMakeFiles/Compiller.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/Compiller.dir/Variable.cpp.o"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/Compiller.dir/Variable.cpp.o -MF CMakeFiles/Compiller.dir/Variable.cpp.o.d -o CMakeFiles/Compiller.dir/Variable.cpp.o -c /home/amadey/Projects/Compiller/src/Variable.cpp

src/CMakeFiles/Compiller.dir/Variable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Compiller.dir/Variable.cpp.i"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amadey/Projects/Compiller/src/Variable.cpp > CMakeFiles/Compiller.dir/Variable.cpp.i

src/CMakeFiles/Compiller.dir/Variable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Compiller.dir/Variable.cpp.s"
	cd /home/amadey/Projects/Compiller/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amadey/Projects/Compiller/src/Variable.cpp -o CMakeFiles/Compiller.dir/Variable.cpp.s

# Object files for target Compiller
Compiller_OBJECTS = \
"CMakeFiles/Compiller.dir/ByteArray.cpp.o" \
"CMakeFiles/Compiller.dir/Compiller.cpp.o" \
"CMakeFiles/Compiller.dir/data_t.cpp.o" \
"CMakeFiles/Compiller.dir/Differentiator.cpp.o" \
"CMakeFiles/Compiller.dir/lexical_analysis.cpp.o" \
"CMakeFiles/Compiller.dir/main.cpp.o" \
"CMakeFiles/Compiller.dir/Reading.cpp.o" \
"CMakeFiles/Compiller.dir/Variable.cpp.o"

# External object files for target Compiller
Compiller_EXTERNAL_OBJECTS =

src/Compiller: src/CMakeFiles/Compiller.dir/ByteArray.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/Compiller.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/data_t.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/Differentiator.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/lexical_analysis.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/main.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/Reading.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/Variable.cpp.o
src/Compiller: src/CMakeFiles/Compiller.dir/build.make
src/Compiller: src/CMakeFiles/Compiller.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amadey/Projects/Compiller/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable Compiller"
	cd /home/amadey/Projects/Compiller/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Compiller.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/Compiller.dir/build: src/Compiller
.PHONY : src/CMakeFiles/Compiller.dir/build

src/CMakeFiles/Compiller.dir/clean:
	cd /home/amadey/Projects/Compiller/build/src && $(CMAKE_COMMAND) -P CMakeFiles/Compiller.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/Compiller.dir/clean

src/CMakeFiles/Compiller.dir/depend:
	cd /home/amadey/Projects/Compiller/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amadey/Projects/Compiller /home/amadey/Projects/Compiller/src /home/amadey/Projects/Compiller/build /home/amadey/Projects/Compiller/build/src /home/amadey/Projects/Compiller/build/src/CMakeFiles/Compiller.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/Compiller.dir/depend

