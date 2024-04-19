#  * AUTHOR: Harry Findlay
#  * LICENSE: Shipped with package - GNU GPL v3.0
#  * FILE START: 20/03/2024
#  * FILE LAST UPDATED: 08/05/2024
#  * 
#  * REQUIREMENTS: make, GCC v13.2.0
#  * REFERENCES: 
#  * 
#  * DESCRIPTION: Makefile to build GCC plugin for deep-rl optimizer

# NOTES:
# Makefile to build plugin for Darwin base system (.dylib) using a GCC homebrew installation, please ensure correct symlinks have been made
# https://apple.stackexchange.com/questions/245891/installed-gcc-with-homebrew-now-how-to-use-that-gcc-instead-of-clang

# linking issue - solved for macos need to add flag -undefined dynamic_lookup to enable functions to be found from bundle
# https://stackoverflow.com/questions/25825979/building-gcc-plugins-on-mac-os-x-10-9-4

# plugin argument for filename
# -fplugin-arg-statetool.dylib-filename=test.txt


HOST_GCC=g++
TARGET_GCC=gcc
PLUGIN_SOURCE_FILES= statetool.cc
GCCPLUGINS_DIR:= $(shell $(TARGET_GCC) -print-file-name=plugin)
EXTRA_DIR= /opt/homebrew/opt/gmp/include
CXXFLAGS+= -I$(GCCPLUGINS_DIR)/include -I$(EXTRA_DIR) -fPIC -fno-rtti -O2 -Wall -Wextra


all: build


statetool: $(PLUGIN_SOURCE_FILES)
	$(HOST_GCC) $(CXXFLAGS) -undefined dynamic_lookup -dynamiclib $^ -o $@.dylib


check: 
	g++-13 -fplugin=./statetool.dylib -fplugin-arg-statetool.dylib-filename=test.txt -c test.cpp -o test.o


clean:
	rm -rf *.o *.dylib *.txt


build: clean statetool.dylib


run: clean statetool.dylib check
