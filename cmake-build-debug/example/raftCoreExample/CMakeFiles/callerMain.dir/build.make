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
include example/raftCoreExample/CMakeFiles/callerMain.dir/depend.make

# Include the progress variables for this target.
include example/raftCoreExample/CMakeFiles/callerMain.dir/progress.make

# Include the compile flags for this target's objects.
include example/raftCoreExample/CMakeFiles/callerMain.dir/flags.make

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o: example/raftCoreExample/CMakeFiles/callerMain.dir/flags.make
example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o: ../src/raftClerk/clerk.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o -c /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/raftClerk/clerk.cpp

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.i"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/raftClerk/clerk.cpp > CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.i

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.s"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/raftClerk/clerk.cpp -o CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.s

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.requires:
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.requires

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.provides: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.requires
	$(MAKE) -f example/raftCoreExample/CMakeFiles/callerMain.dir/build.make example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.provides.build
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.provides

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.provides.build: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o: example/raftCoreExample/CMakeFiles/callerMain.dir/flags.make
example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o: ../src/raftClerk/raftServerRpcUtil.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o -c /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/raftClerk/raftServerRpcUtil.cpp

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.i"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/raftClerk/raftServerRpcUtil.cpp > CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.i

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.s"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/raftClerk/raftServerRpcUtil.cpp -o CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.s

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.requires:
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.requires

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.provides: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.requires
	$(MAKE) -f example/raftCoreExample/CMakeFiles/callerMain.dir/build.make example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.provides.build
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.provides

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.provides.build: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o

example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o: example/raftCoreExample/CMakeFiles/callerMain.dir/flags.make
example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o: ../example/raftCoreExample/caller.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/callerMain.dir/caller.cpp.o -c /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample/caller.cpp

example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/callerMain.dir/caller.cpp.i"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample/caller.cpp > CMakeFiles/callerMain.dir/caller.cpp.i

example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/callerMain.dir/caller.cpp.s"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample/caller.cpp -o CMakeFiles/callerMain.dir/caller.cpp.s

example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.requires:
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.requires

example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.provides: example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.requires
	$(MAKE) -f example/raftCoreExample/CMakeFiles/callerMain.dir/build.make example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.provides.build
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.provides

example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.provides.build: example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o: example/raftCoreExample/CMakeFiles/callerMain.dir/flags.make
example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o: ../src/common/util.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o -c /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/common/util.cpp

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.i"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/common/util.cpp > CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.i

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.s"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/KV-Raft/KVstorageBaseRaft-cpp-main/src/common/util.cpp -o CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.s

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.requires:
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.requires

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.provides: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.requires
	$(MAKE) -f example/raftCoreExample/CMakeFiles/callerMain.dir/build.make example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.provides.build
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.provides

example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.provides.build: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o

# Object files for target callerMain
callerMain_OBJECTS = \
"CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o" \
"CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o" \
"CMakeFiles/callerMain.dir/caller.cpp.o" \
"CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o"

# External object files for target callerMain
callerMain_EXTERNAL_OBJECTS =

../bin/callerMain: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o
../bin/callerMain: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o
../bin/callerMain: example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o
../bin/callerMain: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o
../bin/callerMain: example/raftCoreExample/CMakeFiles/callerMain.dir/build.make
../bin/callerMain: ../lib/libskip_list_on_raft.a
../bin/callerMain: /usr/local/lib/libprotobuf.so
../bin/callerMain: example/raftCoreExample/CMakeFiles/callerMain.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/callerMain"
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/callerMain.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/raftCoreExample/CMakeFiles/callerMain.dir/build: ../bin/callerMain
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/build

example/raftCoreExample/CMakeFiles/callerMain.dir/requires: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/clerk.cpp.o.requires
example/raftCoreExample/CMakeFiles/callerMain.dir/requires: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/raftClerk/raftServerRpcUtil.cpp.o.requires
example/raftCoreExample/CMakeFiles/callerMain.dir/requires: example/raftCoreExample/CMakeFiles/callerMain.dir/caller.cpp.o.requires
example/raftCoreExample/CMakeFiles/callerMain.dir/requires: example/raftCoreExample/CMakeFiles/callerMain.dir/__/__/src/common/util.cpp.o.requires
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/requires

example/raftCoreExample/CMakeFiles/callerMain.dir/clean:
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample && $(CMAKE_COMMAND) -P CMakeFiles/callerMain.dir/cmake_clean.cmake
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/clean

example/raftCoreExample/CMakeFiles/callerMain.dir/depend:
	cd /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/KV-Raft/KVstorageBaseRaft-cpp-main /root/KV-Raft/KVstorageBaseRaft-cpp-main/example/raftCoreExample /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample /root/KV-Raft/KVstorageBaseRaft-cpp-main/cmake-build-debug/example/raftCoreExample/CMakeFiles/callerMain.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/raftCoreExample/CMakeFiles/callerMain.dir/depend

