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
include example/raftCoreExample/CMakeFiles/raftCoreRun.dir/depend.make

# Include the progress variables for this target.
include example/raftCoreExample/CMakeFiles/raftCoreRun.dir/progress.make

# Include the compile flags for this target's objects.
include example/raftCoreExample/CMakeFiles/raftCoreRun.dir/flags.make

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/flags.make
example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o: ../example/raftCoreExample/raftKvDB.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o -c /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample/raftKvDB.cpp

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.i"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample/raftKvDB.cpp > CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.i

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.s"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample/raftKvDB.cpp -o CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.s

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.requires:
.PHONY : example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.requires

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.provides: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.requires
	$(MAKE) -f example/raftCoreExample/CMakeFiles/raftCoreRun.dir/build.make example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.provides.build
.PHONY : example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.provides

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.provides.build: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o

# Object files for target raftCoreRun
raftCoreRun_OBJECTS = \
"CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o"

# External object files for target raftCoreRun
raftCoreRun_EXTERNAL_OBJECTS =

../bin/raftCoreRun: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o
../bin/raftCoreRun: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/build.make
../bin/raftCoreRun: ../lib/libskip_list_on_raft.a
../bin/raftCoreRun: ../lib/librpc_lib.a
../bin/raftCoreRun: /usr/local/lib/libprotobuf.so
../bin/raftCoreRun: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/raftCoreRun"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/raftCoreRun.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/raftCoreExample/CMakeFiles/raftCoreRun.dir/build: ../bin/raftCoreRun
.PHONY : example/raftCoreExample/CMakeFiles/raftCoreRun.dir/build

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/requires: example/raftCoreExample/CMakeFiles/raftCoreRun.dir/raftKvDB.cpp.o.requires
.PHONY : example/raftCoreExample/CMakeFiles/raftCoreRun.dir/requires

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/clean:
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && $(CMAKE_COMMAND) -P CMakeFiles/raftCoreRun.dir/cmake_clean.cmake
.PHONY : example/raftCoreExample/CMakeFiles/raftCoreRun.dir/clean

example/raftCoreExample/CMakeFiles/raftCoreRun.dir/depend:
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/KV-Raft/KVstorageBaseRaft-cpp-main /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample/CMakeFiles/raftCoreRun.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/raftCoreExample/CMakeFiles/raftCoreRun.dir/depend
