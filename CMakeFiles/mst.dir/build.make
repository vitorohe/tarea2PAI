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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2"

# Include any dependencies generated for this target.
include CMakeFiles/mst.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mst.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mst.dir/flags.make

CMakeFiles/mst.dir/mst.cpp.o: CMakeFiles/mst.dir/flags.make
CMakeFiles/mst.dir/mst.cpp.o: mst.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/mst.dir/mst.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/mst.dir/mst.cpp.o -c "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2/mst.cpp"

CMakeFiles/mst.dir/mst.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mst.dir/mst.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2/mst.cpp" > CMakeFiles/mst.dir/mst.cpp.i

CMakeFiles/mst.dir/mst.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mst.dir/mst.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2/mst.cpp" -o CMakeFiles/mst.dir/mst.cpp.s

CMakeFiles/mst.dir/mst.cpp.o.requires:
.PHONY : CMakeFiles/mst.dir/mst.cpp.o.requires

CMakeFiles/mst.dir/mst.cpp.o.provides: CMakeFiles/mst.dir/mst.cpp.o.requires
	$(MAKE) -f CMakeFiles/mst.dir/build.make CMakeFiles/mst.dir/mst.cpp.o.provides.build
.PHONY : CMakeFiles/mst.dir/mst.cpp.o.provides

CMakeFiles/mst.dir/mst.cpp.o.provides.build: CMakeFiles/mst.dir/mst.cpp.o

# Object files for target mst
mst_OBJECTS = \
"CMakeFiles/mst.dir/mst.cpp.o"

# External object files for target mst
mst_EXTERNAL_OBJECTS =

libmst.a: CMakeFiles/mst.dir/mst.cpp.o
libmst.a: CMakeFiles/mst.dir/build.make
libmst.a: CMakeFiles/mst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libmst.a"
	$(CMAKE_COMMAND) -P CMakeFiles/mst.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mst.dir/build: libmst.a
.PHONY : CMakeFiles/mst.dir/build

CMakeFiles/mst.dir/requires: CMakeFiles/mst.dir/mst.cpp.o.requires
.PHONY : CMakeFiles/mst.dir/requires

CMakeFiles/mst.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mst.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mst.dir/clean

CMakeFiles/mst.dir/depend:
	cd "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2" "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2" "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2" "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2" "/home/vito/11vo semestre/Procesamiento y Análisis de Imágenes/Tarea 2/CMakeFiles/mst.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/mst.dir/depend

