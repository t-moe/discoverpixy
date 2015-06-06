#Name of the binary/project
TARGET=emulator

BUILD_DIR=./build
OBJ_DIR=./obj
LIB_DIR=./libs


QT_DIR=./qt
COMMON_DIR=../common

#Tools
CC=gcc #-fdiagnostics=auto
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


LIBS= emulatorqt pixy usb-1.0 
LIBS+= boost_system-mgw49-mt-1_58 boost_timer-mgw49-mt-1_58 boost_chrono-mgw49-mt-1_58
LIBS+= Qt5Core Qt5Gui Qt5Widgets  m stdc++ 

LDFLAGS= -static-libgcc -static-libstdc++ #-Wl,-t  -Wl,--Map=a.map
LDFLAGS+= -L$(QT_DIR)
LDFLAGS+= -L$(LIB_DIR)/Pixy 
LDFLAGS+= -LC:/boost_1_58_0/buildsys/lib
LDFLAGS+= -LC:/Qt/5.4/mingw491_32/lib
LDFLAGS+= -L./libs/Pixy/windows
LDFLAGS+= $(addprefix -l,$(LIBS)) 

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
	cd $(QT_DIR) && qmake && $(MAKE)

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
	cd $(QT_DIR) && $(MAKE) clean && $(RM) Makefile* && $(RM) *.a && $(RMDIR) debug release
	$(RMDIR) $(BUILD_DIR) $(OBJ_DIR)


