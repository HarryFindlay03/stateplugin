# Makefile to build plugin for Darwin base system (.dylib) using GCC homebrew installation
# linking issue - solved for macos need to add flag -undefined dynamic_lookup to enable functions to be found from bundle
# https://stackoverflow.com/questions/25825979/building-gcc-plugins-on-mac-os-x-10-9-4

# g++ -I`gcc -print-file-name=plugin`/include -I/opt/homebrew/opt/gmp/include -fPIC -shared -fno-rtti -O2 statetool.cc -o statetool.so

HOST_GCC=g++-13
TARGET_GCC=gcc-13
PLUGIN_SOURCE_FILES= statetool.cc
GCCPLUGINS_DIR:= $(shell $(TARGET_GCC) -print-file-name=plugin)
EXTRA_DIR= /opt/homebrew/opt/gmp/include
CXXFLAGS+= -I$(GCCPLUGINS_DIR)/include -I$(EXTRA_DIR) -fPIC -fno-rtti -O2 -Wall -Wextra

all: build

statetool.dylib: $(PLUGIN_SOURCE_FILES)
	$(HOST_GCC) $(CXXFLAGS) -undefined dynamic_lookup -dynamiclib $^ -o $@

check: 
	g++-13 -fplugin=./statetool.dylib -c test.cpp -o test.o

clean:
	rm -rf *.o *.dylib

build: clean statetool.dylib

run: clean statetool.dylib check
