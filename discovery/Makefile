#2015 by tmoe, id10101 (and the internet :) )

#Name of the binary/project
TARGET=discoverpixy

#Tools
CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc -fdiagnostics-color=auto
OBJCOPY=$(CROSS_COMPILE)objcopy
GDB=$(CROSS_COMPILE)gdb
SIZE=$(CROSS_COMPILE)size

MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf
STUTIL=utils/st-util-daemon.sh
STFLASH=st-flash
BACKUPNAME=$(shell date +'%y.%m.%d')_BACKUP

#Directories
SRC_DIR=./src
OBJ_DIR=./obj
BUILD_DIR=./build
LIB_DIR=./libs
COMMON_DIR=../common


#Architecture flags
FP_FLAGS?=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
ARCH_FLAGS=-mthumb -mcpu=cortex-m4 $(FP_FLAGS)

#Compiler, Linker Options
#LIB_FOLDERS=$(shell  find $(LIB_DIR) -maxdepth 1 -type d ! -path $(LIB_DIR))
INCLUDES=$(LIB_DIR)/StmCoreNPheriph/inc 
INCLUDES+=$(LIB_DIR)/StmUsbHost/STM32_USB_Device_Specific 
INCLUDES+=$(LIB_DIR)/StmUsbHost/STM32_USB_OTG_Driver/inc
INCLUDES+=$(LIB_DIR)/StmUsbHost/STM32_USB_HOST_Library/Core/inc 
INCLUDES+=$(shell  find $(COMMON_DIR) -maxdepth 1 -type d ! -path $(COMMON_DIR))

INCLUDES:=$(addprefix -I,$(INCLUDES))


CPPFLAGS=-DUSE_USB_OTG_FS -DUSE_HOST_MODE $(INCLUDES) 
CFLAGS=$(ARCH_FLAGS) -O0 -g -std=c99


LIBS=pixy usbhost coreperiph stdc++
LIBSEARCHDIRS=$(LIB_DIR)/StmCoreNPheriph 
LIBSEARCHDIRS+=$(LIB_DIR)/StmUsbHost 
LIBSEARCHDIRS+=$(LIB_DIR)/Pixy


LDFLAGS=--specs=nano.specs -Wl,--gc-sections
LDFLAGS+=$(addprefix -L,$(LIBSEARCHDIRS)) 
LDFLAGS+=$(addprefix -l,$(LIBS))

#Finding Input files
CFILES=$(shell find $(SRC_DIR) -name '*.c')
SFILES=$(SRC_DIR)/startup.s
COMMON_CFILES=$(shell find $(COMMON_DIR) -name '*.c')


#Generate corresponding obj names
SOBJS=$(SFILES:.s=.o)
COBJS=$(CFILES:.c=.o)
OBJS=$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SOBJS) $(COBJS))
COMMON_OBJS=$(patsubst $(COMMON_DIR)/%,$(OBJ_DIR)/%,$(COMMON_CFILES:.c=.o))

#Mark targets which are not "file-targets"
.PHONY: all debug flash start stop backup clean

#keep objs files
.SECONDARY: $(OBJS) $(COMMON_OBJS)

# List of all binaries to build
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

start:
	$(STUTIL) start

stop:
	$(STUTIL) stop

#objects to elf
%.elf : $(OBJS) $(COMMON_OBJS)
	@echo Linking...
	$(MKDIR) $(BUILD_DIR)
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) -T./utils/stm32_flash.ld -Wl,-Map,$(BUILD_DIR)/$(TARGET).map $^ $(LDFLAGS) 
	#$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -Wl,--verbose -Wl,-Map,$(BUILD_DIR)/$(TARGET).map $^

#elf to binary
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

#Asm files to objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@echo Assembling $<...
	$(MKDIR) $(OBJ_DIR)
	$(CC) -x assembler-with-cpp $(CFLAGS) $(CPPFLAGS) -c $< -o $@

#C files to objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
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
	$(RMDIR) $(BUILD_DIR) $(OBJ_DIR)

#Debug target: starts the st-util server and gdb and leaves it open
debug: start all
	$(GDB) $(BUILD_DIR)/$(TARGET).elf -x ./utils/gdb.script

#Flash target: starts the st-util server flashes the elf with gdb and exits afterwards
flash: start all
	$(GDB) $(BUILD_DIR)/$(TARGET).elf -x ./utils/gdb.script -batch

backup: stop 
	$(STFLASH) read $(BACKUPNAME).bin 0x8000000 0x100000
