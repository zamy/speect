# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aby/Development/speect/plugins/utils/viterbi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aby/Development/speect/plugins/utils/viterbi

# Include any dependencies generated for this target.
include swig/python/CMakeFiles/_your_wrapper.dir/depend.make

# Include the progress variables for this target.
include swig/python/CMakeFiles/_your_wrapper.dir/progress.make

# Include the compile flags for this target's objects.
include swig/python/CMakeFiles/_your_wrapper.dir/flags.make

swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o: swig/python/CMakeFiles/_your_wrapper.dir/flags.make
swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o: swig/python/your_wrapperPYTHON_wrap.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aby/Development/speect/plugins/utils/viterbi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o"
	cd /home/aby/Development/speect/plugins/utils/viterbi/swig/python && /home/aby/Bin/gcc  $(C_DEFINES) $(C_FLAGS) -fno-strict-aliasing -fwrapv -Wstrict-prototypes -Wall -o CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o   -c /home/aby/Development/speect/plugins/utils/viterbi/swig/python/your_wrapperPYTHON_wrap.c

swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.i"
	cd /home/aby/Development/speect/plugins/utils/viterbi/swig/python && /home/aby/Bin/gcc  $(C_DEFINES) $(C_FLAGS) -fno-strict-aliasing -fwrapv -Wstrict-prototypes -Wall -E /home/aby/Development/speect/plugins/utils/viterbi/swig/python/your_wrapperPYTHON_wrap.c > CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.i

swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.s"
	cd /home/aby/Development/speect/plugins/utils/viterbi/swig/python && /home/aby/Bin/gcc  $(C_DEFINES) $(C_FLAGS) -fno-strict-aliasing -fwrapv -Wstrict-prototypes -Wall -S /home/aby/Development/speect/plugins/utils/viterbi/swig/python/your_wrapperPYTHON_wrap.c -o CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.s

swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.requires:
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.requires

swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.provides: swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.requires
	$(MAKE) -f swig/python/CMakeFiles/_your_wrapper.dir/build.make swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.provides.build
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.provides

swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.provides.build: swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.provides.build

swig/python/your_wrapperPYTHON_wrap.c: swig/python/your_wrapper.i
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aby/Development/speect/plugins/utils/viterbi/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Swig source"
	cd /home/aby/Development/speect/plugins/utils/viterbi/swig/python && /usr/bin/swig -python -I/home/aby/Development/speect/engine/swig/python -outdir /home/aby/Development/speect/plugins/../swig/python -I/home/aby/Development/speect/plugins/utils/viterbi/src -I/home/aby/Development/speect/engine/src -I/usr/include/python2.6 -I/home/aby/Development/speect/plugins/utils/viterbi/swig/python -o /home/aby/Development/speect/plugins/utils/viterbi/swig/python/your_wrapperPYTHON_wrap.c /home/aby/Development/speect/plugins/utils/viterbi/swig/python/your_wrapper.i

/home/aby/Development/speect/plugins/../swig/python/your_wrapper.py: swig/python/your_wrapperPYTHON_wrap.c

# Object files for target _your_wrapper
_your_wrapper_OBJECTS = \
"CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o"

# External object files for target _your_wrapper
_your_wrapper_EXTERNAL_OBJECTS =

/home/aby/Development/speect/swig/python/_your_wrapper.so: swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o
/home/aby/Development/speect/swig/python/_your_wrapper.so: /usr/lib/python2.6/config/libpython2.6.so
/home/aby/Development/speect/swig/python/_your_wrapper.so: /home/aby/Development/speect/engine/lib/libspct.so.0.9.0
/home/aby/Development/speect/swig/python/_your_wrapper.so: swig/python/CMakeFiles/_your_wrapper.dir/build.make
/home/aby/Development/speect/swig/python/_your_wrapper.so: swig/python/CMakeFiles/_your_wrapper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module /home/aby/Development/speect/swig/python/_your_wrapper.so"
	cd /home/aby/Development/speect/plugins/utils/viterbi/swig/python && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/_your_wrapper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
swig/python/CMakeFiles/_your_wrapper.dir/build: /home/aby/Development/speect/swig/python/_your_wrapper.so
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/build

swig/python/CMakeFiles/_your_wrapper.dir/requires: swig/python/CMakeFiles/_your_wrapper.dir/your_wrapperPYTHON_wrap.c.o.requires
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/requires

swig/python/CMakeFiles/_your_wrapper.dir/clean:
	cd /home/aby/Development/speect/plugins/utils/viterbi/swig/python && $(CMAKE_COMMAND) -P CMakeFiles/_your_wrapper.dir/cmake_clean.cmake
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/clean

swig/python/CMakeFiles/_your_wrapper.dir/depend: swig/python/your_wrapperPYTHON_wrap.c
swig/python/CMakeFiles/_your_wrapper.dir/depend: /home/aby/Development/speect/plugins/../swig/python/your_wrapper.py
	cd /home/aby/Development/speect/plugins/utils/viterbi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aby/Development/speect/plugins/utils/viterbi /home/aby/Development/speect/plugins/utils/viterbi/swig/python /home/aby/Development/speect/plugins/utils/viterbi /home/aby/Development/speect/plugins/utils/viterbi/swig/python /home/aby/Development/speect/plugins/utils/viterbi/swig/python/CMakeFiles/_your_wrapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/python/CMakeFiles/_your_wrapper.dir/depend
