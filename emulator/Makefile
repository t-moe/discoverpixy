#Name of the binary/project
TARGET=emulator

BUILD_DIR=./build
OBJ_DIR=./obj

QT_DIR=./qt
COMMON_DIR=../common

#Tools
CC=gcc

MAKE=make
MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf


COMMON_INC=-I$(COMMON_DIR)/lowlevel -I$(COMMON_DIR)/tft
QT_LIB=$(QT_DIR)/libemulatorqt.a


CPPFLAGS= -march=x86-64 -mtune=generic -fPIC $(COMMON_INC)
CFLAGS= -O0 -g

LDFLAGS= -L$(QT_DIR) -lQt5Core -lQt5Gui -lQt5Widgets -lemulatorqt -lm -lstdc++ 

#Finding Input files
CFILES=$(shell find . -maxdepth 1 -name '*.c')
COMMON_CFILES=$(shell find $(COMMON_DIR) -name '*.c')

#Generate corresponding obj names
OBJS=$(patsubst ./%,$(OBJ_DIR)/%,$(CFILES:.c=.o))
COMMON_OBJS=$(patsubst $(COMMON_DIR)/%,$(OBJ_DIR)/%,$(COMMON_CFILES:.c=.o))


#Mark targets which are not "file-targets"
.PHONY: all clean 

# List of all binaries to build
all: $(BUILD_DIR)/$(TARGET) $(OBJS) $(QT_LIB) 

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


