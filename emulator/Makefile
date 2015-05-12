#Name of the binary/project
TARGET=emulator

BUILD_DIR=./build
OBJ_DIR=./obj
LIB_DIR=./libs


QT_DIR=./qt
COMMON_DIR=../common

#Tools
CC=gcc -fdiagnostics-color=auto
GDB=gdb


MAKE=make
MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf

INCLUDES=$(shell  find $(COMMON_DIR) -maxdepth 1 -type d ! -path $(COMMON_DIR))
INCLUDES:=$(addprefix -I,$(INCLUDES))
QT_LIB=$(QT_DIR)/libemulatorqt.a


CPPFLAGS= -march=x86-64 -mtune=generic -fPIC $(INCLUDES)
CFLAGS= -O0 -g -std=c99


LIBS= pixy usb-1.0 boost_system boost_timer boost_chrono
LIBS+=Qt5Core Qt5Gui Qt5Widgets emulatorqt m stdc++


LDFLAGS= -L$(QT_DIR) $(addprefix -l,$(LIBS)) 
LDFLAGS+= -L$(LIB_DIR)/Pixy 


#Finding Input files
CFILES=$(shell find . -maxdepth 1 -name '*.c')
COMMON_CFILES=$(shell find $(COMMON_DIR) -name '*.c')

#Generate corresponding obj names
OBJS=$(patsubst ./%,$(OBJ_DIR)/%,$(CFILES:.c=.o))
COMMON_OBJS=$(patsubst $(COMMON_DIR)/%,$(OBJ_DIR)/%,$(COMMON_CFILES:.c=.o))


#Mark targets which are not "file-targets"
.PHONY: all clean run debug 

# List of all binaries to build
all: $(BUILD_DIR)/$(TARGET) 

run: all
	./build/emulator

debug: all
	$(GDB) ./build/emulator

$(QT_LIB): 
	cd $(QT_DIR) && qmake &&make

#objects to elf
$(BUILD_DIR)/$(TARGET): $(OBJS) $(COMMON_OBJS) $(QT_LIB)
	@echo Linking...
	$(MKDIR) $(BUILD_DIR)
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) 
	@echo $(COMMON_OBJS)

#C files to objects
$(OBJ_DIR)/%.o: %.c
	@echo Compiling $<...
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

#common C files to objects
$(OBJ_DIR)/%.o: $(COMMON_DIR)/%.c
	@echo Compiling Common file $<...
	$(MKDIR) $(dir $(patsubst $(COMMON_DIR)/%,$(OBJ_DIR)/%, $<))
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<



#Clean Obj files and builded stuff
clean:
	cd $(QT_DIR) && $(MAKE) clean && $(RM) Makefile && $(RM) *.a
	$(RMDIR) $(BUILD_DIR) $(OBJ_DIR)


