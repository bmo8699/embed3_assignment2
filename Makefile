SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard ${SRC_DIR}/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
all: clean kernel8.img 
$(BUILD_DIR)/boot.o: ${SRC_DIR}/boot.S
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@
kernel8.img: $(BUILD_DIR)/boot.o $(OBJS)
	aarch64-none-elf-ld -nostdlib -nostartfiles $(BUILD_DIR)/boot.o $(OBJS) -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img
clean:
	del $(BUILD_DIR)\kernel8.elf $(BUILD_DIR)\*.o
run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio