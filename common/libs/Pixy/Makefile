#2015 by tmoe, id10101 (and the internet :) )

TARGET=libpixy

#Tools
CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)g++
AR=$(CROSS_COMPILE)ar
RMDIR = rm -rf
RM=rm -f
MKDIR=mkdir -p

#Directories
SRC_DIR=./src
INC_DIR=./
OBJ_DIR=./obj

#Architecture flags
FP_FLAGS?=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
ARCH_FLAGS=-mthumb -mcpu=cortex-m4 $(FP_FLAGS)

#Compiler, Linker Options
CPPFLAGS=-I$(INC_DIR) -D__LINUX__=1 -DHOST=1 #-DDEBUG=1
CFLAGS=$(ARCH_FLAGS) -O0 -g #-ffunction-sections -fdata-sections -g
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16


#Finding Input files
CFILES=$(shell find $(SRC_DIR) -name '*.cpp')

#Generate corresponding obj names
COBJS=$(CFILES:.cpp=.o)
OBJS=$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(COBJS))

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
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling $<...
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

#Clean Obj files and builded stuff
clean:
	$(RMDIR)  $(OBJ_DIR)
	$(RM) $(TARGET).a
