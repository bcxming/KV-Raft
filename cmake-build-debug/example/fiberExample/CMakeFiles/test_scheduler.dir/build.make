# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/KV-Raft/KVstorageBaseRaft-cpp-main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug

# Include any dependencies generated for this target.
include example/fiberExample/CMakeFiles/test_scheduler.dir/depend.make

# Include the progress variables for this target.
include example/fiberExample/CMakeFiles/test_scheduler.dir/progress.make

# Include the compile flags for this target's objects.
include example/fiberExample/CMakeFiles/test_scheduler.dir/flags.make

example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o: example/fiberExample/CMakeFiles/test_scheduler.dir/flags.make
example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o: ../example/fiberExample/test_scheduler.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o -c /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/fiberExample/test_scheduler.cpp

example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_scheduler.dir/test_scheduler.cpp.i"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/fiberExample/test_scheduler.cpp > CMakeFiles/test_scheduler.dir/test_scheduler.cpp.i

example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_scheduler.dir/test_scheduler.cpp.s"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/fiberExample/test_scheduler.cpp -o CMakeFiles/test_scheduler.dir/test_scheduler.cpp.s

example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.requires:
.PHONY : example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.requires

example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.provides: example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.requires
	$(MAKE) -f example/fiberExample/CMakeFiles/test_scheduler.dir/build.make example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.provides.build
.PHONY : example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.provides

example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.provides.build: example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o

# Object files for target test_scheduler
test_scheduler_OBJECTS = \
"CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o"

# External object files for target test_scheduler
test_scheduler_EXTERNAL_OBJECTS =

../bin/test_scheduler: example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o
../bin/test_scheduler: example/fiberExample/CMakeFiles/test_scheduler.dir/build.make
../bin/test_scheduler: ../lib/libfiber_lib.a
../bin/test_scheduler: example/fiberExample/CMakeFiles/test_scheduler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/test_scheduler"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_scheduler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/fiberExample/CMakeFiles/test_scheduler.dir/build: ../bin/test_scheduler
.PHONY : example/fiberExample/CMakeFiles/test_scheduler.dir/build

example/fiberExample/CMakeFiles/test_scheduler.dir/requires: example/fiberExample/CMakeFiles/test_scheduler.dir/test_scheduler.cpp.o.requires
.PHONY : example/fiberExample/CMakeFiles/test_scheduler.dir/requires

example/fiberExample/CMakeFiles/test_scheduler.dir/clean:
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample && $(CMAKE_COMMAND) -P CMakeFiles/test_scheduler.dir/cmake_clean.cmake
.PHONY : example/fiberExample/CMakeFiles/test_scheduler.dir/clean

example/fiberExample/CMakeFiles/test_scheduler.dir/depend:
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/KV-Raft/KVstorageBaseRaft-cpp-main /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/fiberExample /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/fiberExample/CMakeFiles/test_scheduler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/fiberExample/CMakeFiles/test_scheduler.dir/depend

