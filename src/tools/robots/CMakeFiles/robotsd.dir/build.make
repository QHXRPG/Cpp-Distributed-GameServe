# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /usr/bin/cmake3

# The command to remove a file.
RM = /usr/bin/cmake3 -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/qhx/reactor/src/tools/robots

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qhx/reactor/src/tools/robots

# Include any dependencies generated for this target.
include CMakeFiles/robotsd.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/robotsd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/robotsd.dir/flags.make

CMakeFiles/robotsd.dir/main.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/robotsd.dir/main.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/main.cpp.o -c /home/qhx/reactor/src/tools/robots/main.cpp

CMakeFiles/robotsd.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/main.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/tools/robots/main.cpp > CMakeFiles/robotsd.dir/main.cpp.i

CMakeFiles/robotsd.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/main.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/tools/robots/main.cpp -o CMakeFiles/robotsd.dir/main.cpp.s

CMakeFiles/robotsd.dir/robot.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/robot.cpp.o: robot.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/robotsd.dir/robot.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/robot.cpp.o -c /home/qhx/reactor/src/tools/robots/robot.cpp

CMakeFiles/robotsd.dir/robot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/robot.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/tools/robots/robot.cpp > CMakeFiles/robotsd.dir/robot.cpp.i

CMakeFiles/robotsd.dir/robot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/robot.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/tools/robots/robot.cpp -o CMakeFiles/robotsd.dir/robot.cpp.s

CMakeFiles/robotsd.dir/robots_app.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/robots_app.cpp.o: robots_app.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/robotsd.dir/robots_app.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/robots_app.cpp.o -c /home/qhx/reactor/src/tools/robots/robots_app.cpp

CMakeFiles/robotsd.dir/robots_app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/robots_app.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/tools/robots/robots_app.cpp > CMakeFiles/robotsd.dir/robots_app.cpp.i

CMakeFiles/robotsd.dir/robots_app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/robots_app.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/tools/robots/robots_app.cpp -o CMakeFiles/robotsd.dir/robots_app.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.o: /home/qhx/reactor/src/libs/libserver/base_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.o -c /home/qhx/reactor/src/libs/libserver/base_buffer.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/base_buffer.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/base_buffer.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.o: /home/qhx/reactor/src/libs/libserver/connect_obj.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.o -c /home/qhx/reactor/src/libs/libserver/connect_obj.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/connect_obj.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/connect_obj.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.o: /home/qhx/reactor/src/libs/libserver/global.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.o -c /home/qhx/reactor/src/libs/libserver/global.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/global.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/global.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.o: /home/qhx/reactor/src/libs/libserver/message_list.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.o -c /home/qhx/reactor/src/libs/libserver/message_list.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/message_list.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/message_list.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.o: /home/qhx/reactor/src/libs/libserver/network.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.o -c /home/qhx/reactor/src/libs/libserver/network.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/network.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/network.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.o: /home/qhx/reactor/src/libs/libserver/network_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.o -c /home/qhx/reactor/src/libs/libserver/network_buffer.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/network_buffer.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/network_buffer.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.o: /home/qhx/reactor/src/libs/libserver/network_connector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.o -c /home/qhx/reactor/src/libs/libserver/network_connector.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/network_connector.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/network_connector.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.o: /home/qhx/reactor/src/libs/libserver/network_listen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.o -c /home/qhx/reactor/src/libs/libserver/network_listen.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/network_listen.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/network_listen.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.o: /home/qhx/reactor/src/libs/libserver/packet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.o -c /home/qhx/reactor/src/libs/libserver/packet.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/packet.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/packet.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.o: /home/qhx/reactor/src/libs/libserver/server_app.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.o -c /home/qhx/reactor/src/libs/libserver/server_app.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/server_app.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/server_app.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.o: /home/qhx/reactor/src/libs/libserver/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.o -c /home/qhx/reactor/src/libs/libserver/thread.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/thread.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/thread.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.o: /home/qhx/reactor/src/libs/libserver/thread_mgr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.o -c /home/qhx/reactor/src/libs/libserver/thread_mgr.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/thread_mgr.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/thread_mgr.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.o: /home/qhx/reactor/src/libs/libserver/thread_obj.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.o -c /home/qhx/reactor/src/libs/libserver/thread_obj.cpp

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/thread_obj.cpp > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/thread_obj.cpp -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.o: /home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.o -c /home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.s

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.o: /home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.o -c /home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc > CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.i

CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc -o CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.s

# Object files for target robotsd
robotsd_OBJECTS = \
"CMakeFiles/robotsd.dir/main.cpp.o" \
"CMakeFiles/robotsd.dir/robot.cpp.o" \
"CMakeFiles/robotsd.dir/robots_app.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.o" \
"CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.o"

# External object files for target robotsd
robotsd_EXTERNAL_OBJECTS =

/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/main.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/robot.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/robots_app.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/base_buffer.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/connect_obj.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/global.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/message_list.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_buffer.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_connector.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/network_listen.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/packet.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/server_app.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_mgr.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/thread_obj.cpp.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/msg.pb.cc.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/home/qhx/reactor/src/libs/libserver/protobuf/proto_id.pb.cc.o
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/build.make
/home/qhx/reactor/bin/robotsd: CMakeFiles/robotsd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qhx/reactor/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking CXX executable /home/qhx/reactor/bin/robotsd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/robotsd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/robotsd.dir/build: /home/qhx/reactor/bin/robotsd

.PHONY : CMakeFiles/robotsd.dir/build

CMakeFiles/robotsd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/robotsd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/robotsd.dir/clean

CMakeFiles/robotsd.dir/depend:
	cd /home/qhx/reactor/src/tools/robots && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qhx/reactor/src/tools/robots /home/qhx/reactor/src/tools/robots /home/qhx/reactor/src/tools/robots /home/qhx/reactor/src/tools/robots /home/qhx/reactor/src/tools/robots/CMakeFiles/robotsd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/robotsd.dir/depend

