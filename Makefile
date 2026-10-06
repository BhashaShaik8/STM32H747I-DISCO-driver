TARGET = blink

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard

CORE_DIR   = Core
DRV_DIR    = Drivers
OBJ_DIR    = Debug/obj
EXE_DIR    = Debug/exe

CFLAGS  = $(CPU) -O0 -g -Wall -ffreestanding -nostdlib -I$(DRV_DIR)
LDFLAGS = $(CPU) -TSTM32H747XIHX_FLASH.ld -nostartfiles -Wl,--gc-sections \
          -Wl,-Map=$(EXE_DIR)/$(TARGET).map

# Auto-discover all .c and .s files in each source folder
SRCS_CORE_C = $(wildcard $(CORE_DIR)/*.c)
SRCS_CORE_S = $(wildcard $(CORE_DIR)/*.s)
SRCS_DRV_C  = $(wildcard $(DRV_DIR)/*.c)
SRCS_DRV_S  = $(wildcard $(DRV_DIR)/*.s)

# Map each discovered source to its object file under obj/<folder>/
OBJS_CORE = $(patsubst $(CORE_DIR)/%.c,$(OBJ_DIR)/$(CORE_DIR)/%.o,$(SRCS_CORE_C)) \
            $(patsubst $(CORE_DIR)/%.s,$(OBJ_DIR)/$(CORE_DIR)/%.o,$(SRCS_CORE_S))
OBJS_DRV  = $(patsubst $(DRV_DIR)/%.c,$(OBJ_DIR)/$(DRV_DIR)/%.o,$(SRCS_DRV_C)) \
            $(patsubst $(DRV_DIR)/%.s,$(OBJ_DIR)/$(DRV_DIR)/%.o,$(SRCS_DRV_S))

OBJS = $(OBJS_CORE) $(OBJS_DRV)

all: $(OBJ_DIR)/$(CORE_DIR) $(OBJ_DIR)/$(DRV_DIR) $(EXE_DIR) \
     $(EXE_DIR)/$(TARGET).elf $(EXE_DIR)/$(TARGET).bin
	$(SIZE) $(EXE_DIR)/$(TARGET).elf

$(OBJ_DIR)/$(CORE_DIR):
	mkdir -p $(OBJ_DIR)/$(CORE_DIR)

$(OBJ_DIR)/$(DRV_DIR):
	mkdir -p $(OBJ_DIR)/$(DRV_DIR)

$(EXE_DIR):
	mkdir -p $(EXE_DIR)

$(OBJ_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(DRV_DIR)/%.o: $(DRV_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(DRV_DIR)/%.o: $(DRV_DIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(EXE_DIR)/$(TARGET).bin: $(EXE_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf Debug

flash: all
	openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program $(EXE_DIR)/$(TARGET).elf verify reset exit"

.PHONY: all clean flash