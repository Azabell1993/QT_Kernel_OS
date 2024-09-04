# Makefile for building both kernel_printf.a, kernel_lib.a, and td_kernel_engine specific targets

# Determine the OS and set the compiler
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S), Darwin)      # macOS
    CC = clang
    ARCH_FLAGS = -arch $(UNAME_M)
    # macOS에서는 '-Wno-format-truncation'을 지원하지 않으므로 제외
    CFLAGS = -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -Wno-unused-variable -fcommon
else ifeq ($(UNAME_S), Linux)  # Ubuntu (Linux)
    CC = gcc
    ARCH_FLAGS =
    # Linux에서는 '-Wno-format-truncation'을 사용
    CFLAGS = -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -Wno-unused-variable -Wno-format-truncation -fcommon
else
    $(error Unsupported OS: $(UNAME_S))
endif

# Source directories
STDIO_SRC_DIR = include_printf
KERNEL_SRC_DIR = src

# Object files
STDIO_SRCS = $(wildcard $(STDIO_SRC_DIR)/*.c)
KERNEL_SRCS = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_OBJS_X86_64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.x86_64.o)
STDIO_OBJS_X86_64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.x86_64.o)
KERNEL_OBJS_ARM64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.arm64.o)
STDIO_OBJS_ARM64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.arm64.o)

# Libraries
KERNEL_LIB = kernel_lib.a
STDIO_LIB = kernel_printf.a

# Default target
all: $(KERNEL_LIB) $(STDIO_LIB) td_kernel_engine

# Kernel library creation
$(KERNEL_LIB): $(KERNEL_OBJS_X86_64) $(KERNEL_OBJS_ARM64)
	@echo "Creating universal static library $(KERNEL_LIB)"
ifeq ($(UNAME_S), Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# Stdio library creation
$(STDIO_LIB): $(STDIO_OBJS_X86_64) $(STDIO_OBJS_ARM64)
	@echo "Creating universal static library $(STDIO_LIB)"
ifeq ($(UNAME_S), Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# Compile .c files to .o files for x86_64
$(KERNEL_SRC_DIR)/%.x86_64.o: $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	$(CC) $(CFLAGS) $(ARCH_FLAGS) -c $< -o $@

$(STDIO_SRC_DIR)/%.x86_64.o: $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	$(CC) $(CFLAGS) $(ARCH_FLAGS) -c $< -o $@

# Compile .c files to .o files for arm64
$(KERNEL_SRC_DIR)/%.arm64.o: $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	$(CC) $(CFLAGS) $(ARCH_FLAGS) -c $< -o $@

$(STDIO_SRC_DIR)/%.arm64.o: $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	$(CC) $(CFLAGS) $(ARCH_FLAGS) -c $< -o $@

# Target to build and link td_kernel_engine executable
td_kernel_engine: $(filter-out $(KERNEL_SRC_DIR)/td_kernel_engine.x86_64.o, $(KERNEL_OBJS_X86_64)) $(STDIO_LIB)
	@echo "Building test for td_kernel_engine.c"
	$(CC) $(CFLAGS) -o td_kernel_engine src/td_kernel_engine.c $(filter-out $(KERNEL_SRC_DIR)/td_kernel_engine.x86_64.o, $(KERNEL_OBJS_X86_64)) $(STDIO_LIB)

# Clean up (excluding specific files)
clean:
	@echo "Cleaning up..."
	@rm -f $(KERNEL_LIB) $(STDIO_LIB) td_kernel_engine
	@find . -name "*.o" -delete

.PHONY: all clean td_kernel_engine
