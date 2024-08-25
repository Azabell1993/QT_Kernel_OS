# Makefile for building both kernel_printf.a, kernel_lib.a, and DTCASE specific targets

# Determine the OS and set the compiler
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)      # macOS
    CC = clang
else ifeq ($(UNAME_S), Linux)  # Linux
    CC = gcc
else
    $(error Unsupported OS: $(UNAME_S))
endif

# Compiler flags
CFLAGS = -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon

# Source files
KERNEL_ASM_SRC_DIR = src
SRCS = $(KERNEL_ASM_SRC_DIR)/kernel_asm.c

# DTCASE values
DTCASES = 1 2 3 4 5 6 7

# Libraries for printf and kernel
STDIO_LIB = kernel_printf.a
KERNEL_LIB = kernel_lib.a

# Source files for printf and kernel libraries
STDIO_SRC_DIR = include_printf
STDIO_SRCS = $(wildcard $(STDIO_SRC_DIR)/*.c)
KERNEL_SRC_DIR = src
KERNEL_SRCS = $(wildcard $(KERNEL_SRC_DIR)/*.c)

# Object files for different architectures (template)
STDIO_OBJS_X86_64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.x86_64.o)
STDIO_OBJS_ARM64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.arm64.o)
KERNEL_OBJS_X86_64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.x86_64.o)
KERNEL_OBJS_ARM64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.arm64.o)

# Object files for DTCASE-specific builds
KERNEL_ASM_OBJS_X86_64 = $(DTCASES:%=kernel_asm_%.x86_64.o)
KERNEL_ASM_OBJS_ARM64 = $(DTCASES:%=kernel_asm_%.arm64.o)

# Archive files (template)
KERNEL_ASM_LIB_X86_64 = $(DTCASES:%=kernel_asm_%.x86_64.a)
KERNEL_ASM_LIB_ARM64 = $(DTCASES:%=kernel_asm_%.arm64.a)

# Universal binaries
KERNEL_ASM_LIB_UNIVERSAL = $(DTCASES:%=kernel_asm_%.a)


# Default target
all: $(KERNEL_ASM_LIB_UNIVERSAL) $(STDIO_LIB) $(KERNEL_LIB)

# Rule to build each DTCASE object file for x86_64
kernel_asm_%.x86_64.o: $(SRCS)
	$(CC) $(CFLAGS) -arch x86_64 -DDTCASE=$* -c $< -o $@

# Rule to build each DTCASE object file for arm64
kernel_asm_%.arm64.o: $(SRCS)
	$(CC) $(CFLAGS) -arch arm64 -DDTCASE=$* -c $< -o $@

# Rule to build each DTCASE archive file for x86_64
kernel_asm_%.x86_64.a: kernel_asm_%.x86_64.o
	$(AR) rcs $@ $<

# Rule to build each DTCASE archive file for arm64
kernel_asm_%.arm64.a: kernel_asm_%.arm64.o
	$(AR) rcs $@ $<

# Rule to create universal binaries combining x86_64 and arm64 versions
kernel_asm_%.a: kernel_asm_%.x86_64.a kernel_asm_%.arm64.a
	@libtool -static -o $@ $^

# stdio library creation (object files and archive)
$(STDIO_LIB): $(STDIO_OBJS_X86_64) $(STDIO_OBJS_ARM64)
	@echo "Creating universal static library $(STDIO_LIB)"
ifeq ($(UNAME_S),Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# kernel_lib library creation (object files and archive)
$(KERNEL_LIB): $(KERNEL_OBJS_X86_64) $(KERNEL_OBJS_ARM64)
	@echo "Creating universal static library $(KERNEL_LIB)"
ifeq ($(UNAME_S),Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# Architecture-specific compilation rules for stdio and kernel libraries
$(STDIO_OBJS_X86_64): $(STDIO_SRC_DIR)/%.x86_64.o : $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	@$(CC) $(CFLAGS) -arch x86_64 -c $< -o $@

$(STDIO_OBJS_ARM64): $(STDIO_SRC_DIR)/%.arm64.o : $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	@$(CC) $(CFLAGS) -arch arm64 -c $< -o $@

$(KERNEL_OBJS_X86_64): $(KERNEL_SRC_DIR)/%.x86_64.o : $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	@$(CC) $(CFLAGS) -arch x86_64 -c $< -o $@

$(KERNEL_OBJS_ARM64): $(KERNEL_SRC_DIR)/%.arm64.o : $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	@$(CC) $(CFLAGS) -arch arm64 -c $< -o $@

# Clean up (excluding specific files)
clean:
	@echo "Cleaning up..."
	@rm -f $(STDIO_LIB) $(KERNEL_LIB) $(KERNEL_ASM_LIB_UNIVERSAL)
	@find . -name "*.o" -delete

.PHONY: all clean
