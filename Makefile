#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
#GCC_PATH = /home/jean/opt/gcc-arm-none-eabi-10.3-2021.10/bin



ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPU = -mcpu=cortex-m0plus

MCU = $(CPU) -mthumb -mfloat-abi=soft


BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

TARGET = CatFeeder


INCDIRS =  \
	.  \
	./drivers/cmsis \
	./drivers/st \
	./inc/display \
	./inc \

SOURCEDIRS =  \
	./src \
	./src/display \
	./drivers/st \


CFILES = $(foreach D, $(SOURCEDIRS),$(wildcard $(D)/*.c))
CXXFILES = $(foreach D, $(SOURCEDIRS),$(wildcard $(D)/*.cpp))

ASM_SOURCES = $(foreach D, $(SOURCEDIRS),$(wildcard $(D)/*.s))

OBJECTS_TEMP = $(patsubst %.c, %.o, $(CFILES)) $(patsubst %.cpp, %.o, $(CXXFILES)) $(patsubst %.s, %.o, $(ASM_SOURCES))


## TODO: Need to improve this :)
#$(info objects_temp $(OBJECTS_TEMP))

OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(OBJECTS_TEMP)))   

#$(error objects $(OBJECTS))

C_DEFS =  \
	-DSTM32L073xx \
	-DUSE_HAL_DRIVER \
	-DDEBUG


DEPFLAGS=-MP -MD
OPTIONS = -Wall -Wextra -Wpedantic -O0 -g3 -fdata-sections -ffunction-sections 
CFLAGS = $(MCU) $(C_DEFS) $(foreach D, $(INCDIRS), -I$(D)) $(OPTIONS) $(DEPFLAGS)
CXXFLAGS = $(CFLAGS) -std=c++20 -fno-rtti -fno-exceptions -fno-threadsafe-statics 

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L073RZTX_FLASH.ld
LIBS = -lc -lm -lnosys 
LDFLAGS = $(MCU) -specs=nano.specs -u _printf_float -T $(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--print-memory-usage -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group  

##All C++ files
#$(OBJ_DIR)/main.o: src/main.cpp
#	mkdir -p $(OBJ_DIR)
#	$(CC) $(CFLAGS) -c -o $@ $^

##All C files


## Compiling C++

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(OBJ_DIR)/%.o: src/display/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $^



## Compiling C

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -std=c17 -c -o $@ $^


$(OBJ_DIR)/%.o: drivers/st/%.c | $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -std=c17 -c -o $@ $^

$(OBJ_DIR)/%.o: src/display/%.c | $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -std=c17 -c -o $@ $^



## Compiling ASM

$(OBJ_DIR)/%.o: src/%.s | $(OBJ_DIR)
	$(AS) $(CXXFLAGS) -c -o $@ $^	


all: $(BIN_DIR)/$(TARGET).elf $(BIN_DIR)/$(TARGET).hex $(BIN_DIR)/$(TARGET).bin

## Linking
$(BIN_DIR)/$(TARGET).elf: $(OBJECTS) 
	$(CXX) $(LDFLAGS) $^ -o $@
	$(SZ) $@

$(BIN_DIR)/$(TARGET).hex: $(BIN_DIR)/$(TARGET).elf 
	$(HEX) $< $@

$(BIN_DIR)/$(TARGET).bin: $(BIN_DIR)/$(TARGET).elf 
	$(BIN) $< $@


$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR): $(BIN_DIR)
	mkdir -p $@

#DEVICE = STM32L073RZ

#$(BUILD_DIR)/jflash: $(BIN_DIR)/$(TARGET).bin
#	@touch $@
#	@echo device $(DEVICE) > $@
	
#	@echo loadbin $< 0x8000000 >> $@
	

#jflash: $(BUILD_DIR)/jflash
#	$(JLINK_FLASH)/JLinkExe -commanderscript $<



#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
