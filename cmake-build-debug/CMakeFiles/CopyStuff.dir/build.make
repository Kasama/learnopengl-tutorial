# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /opt/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug

# Utility rule file for CopyStuff.

# Include the progress variables for this target.
include CMakeFiles/CopyStuff.dir/progress.make

CMakeFiles/CopyStuff:
	/opt/clion/bin/cmake/bin/cmake -E copy_directory /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/shader /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug/shader
	/opt/clion/bin/cmake/bin/cmake -E copy_directory /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/content /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug/content

CopyStuff: CMakeFiles/CopyStuff
CopyStuff: CMakeFiles/CopyStuff.dir/build.make

.PHONY : CopyStuff

# Rule to build all files generated by this target.
CMakeFiles/CopyStuff.dir/build: CopyStuff

.PHONY : CMakeFiles/CopyStuff.dir/build

CMakeFiles/CopyStuff.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CopyStuff.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CopyStuff.dir/clean

CMakeFiles/CopyStuff.dir/depend:
	cd /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug /home/roberto/documents/USP/Programação/OpenGL/learnopengl/tutorial/cmake-build-debug/CMakeFiles/CopyStuff.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CopyStuff.dir/depend

