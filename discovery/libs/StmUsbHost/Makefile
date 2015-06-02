#2015 by tmoe, id10101 (and the internet :) )
#TODO Fix hacky includes and sources directories and obj folder stuff


TARGET=libusbhost

#Tools
CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
RMDIR = rm -rf
RM=rm -f
MKDIR=mkdir -p

#Directories
SRC_DIR=./STM32_USB_HOST_Library/Core/src 
SRC_DIR2=./STM32_USB_OTG_Driver/src
INC_DIR=./STM32_USB_HOST_Library/Core/inc
INC_DIR2=./STM32_USB_OTG_Driver/inc
INC_DIR3=./STM32_USB_Device_Specific/
INC_CORE_DIR=../StmCoreNPheriph/inc
OBJ_DIR=./obj

#Architecture flags
FP_FLAGS?=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
ARCH_FLAGS=-mthumb -mcpu=cortex-m4 $(FP_FLAGS)

#Compiler, Linker Options
CPPFLAGS=-DUSE_USB_OTG_FS -DUSE_HOST_MODE -I$(INC_DIR) -I$(INC_DIR2) -I$(INC_DIR3) -I$(INC_CORE_DIR)
CFLAGS=$(ARCH_FLAGS) -O0 -g -ffunction-sections -fdata-sections 
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16


#Finding Input files
CFILES=$(shell find $(SRC_DIR) $(SRC_DIR2) -name '*.c')

#Generate corresponding obj names
COBJS=$(CFILES:.c=.o)
OBJS=$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(patsubst $(SRC_DIR2)/%,$(OBJ_DIR)/%,$(COBJS)))

#Keep the objects files
.SECONDARY: $(OBJS)

#Mark targets which are not "file-targets"
.PHONY: all  clean

# List of all binaries to build
all: $(TARGET).a

#objects to lib
%.a : $(OBJS)
	@echo Linking...
	$(AR) rcs $@ $^

#C files to objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo Compiling $<...
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR2)/%.c
	@echo Compiling $<...
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

#Clean Obj files and builded stuff
clean:
	$(RMDIR)  $(OBJ_DIR)
	$(RM) $(TARGET).a
