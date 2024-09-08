# Makefile for building both kernel_printf.a, kernel_lib.a, DTCASE, and td_kernel_engine specific targets

# Determine the OS and set the compiler
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S), Darwin)      # macOS (both x86_64 and ARM64)
    CC = clang
    ifeq ($(UNAME_M), arm64)
        ARCH_FLAGS = -arch arm64
    else
        ARCH_FLAGS = -arch x86_64
    endif
    # macOS에서는 '-Wno-format-truncation'을 지원하지 않으므로 제외
    CFLAGS = -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon
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

# DTCASE values
DTCASES = 1 2 3 4 5 6 7

# Source files for kernel_asm.c and td_kernel_engine.c
KERNEL_ASM_SRC = $(KERNEL_SRC_DIR)/kernel_asm.c
TD_KERNEL_SRC = $(KERNEL_SRC_DIR)/td_kernel_engine.c

# Libraries
STDIO_LIB = kernel_printf.a
KERNEL_LIB = kernel_lib.a
TD_KERNEL_LIB = td_kernel_lib.a

# Object files for different architectures (template)
STDIO_SRCS = $(wildcard $(STDIO_SRC_DIR)/*.c)
KERNEL_SRCS = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_OBJS_X86_64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.x86_64.o)
STDIO_OBJS_X86_64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.x86_64.o)
KERNEL_OBJS_ARM64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.arm64.o)
STDIO_OBJS_ARM64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.arm64.o)

# Object files for DTCASE-specific builds
KERNEL_ASM_OBJS_X86_64 = $(DTCASES:%=kernel_asm_%.x86_64.o)
KERNEL_ASM_OBJS_ARM64 = $(DTCASES:%=kernel_asm_%.arm64.o)

# Archive files for DTCASE builds
KERNEL_ASM_LIB_X86_64 = $(DTCASES:%=kernel_asm_%.x86_64.a)
KERNEL_ASM_LIB_ARM64 = $(DTCASES:%=kernel_asm_%.arm64.a)

# Universal DTCASE binaries
KERNEL_ASM_LIB_UNIVERSAL = $(DTCASES:%=kernel_asm_%.a)

# Default target
all: $(KERNEL_ASM_LIB_UNIVERSAL) $(STDIO_LIB) $(KERNEL_LIB) $(TD_KERNEL_LIB) td_kernel_engine

# Rule to build each DTCASE object file for x86_64
kernel_asm_%.x86_64.o: $(KERNEL_ASM_SRC)
	$(CC) $(CFLAGS) -arch x86_64 -DDTCASE=$* -c $< -o $@

# Rule to build each DTCASE object file for arm64
kernel_asm_%.arm64.o: $(KERNEL_ASM_SRC)
	$(CC) $(CFLAGS) -arch arm64 -DDTCASE=$* -c $< -o $@

# Rule to build each DTCASE archive file for x86_64
kernel_asm_%.x86_64.a: kernel_asm_%.x86_64.o
	$(AR) rcs $@ $<

# Rule to build each DTCASE archive file for arm64
kernel_asm_%.arm64.a: kernel_asm_%.arm64.o
	$(AR) rcs $@ $<

# Rule to create universal DTCASE binaries combining x86_64 and arm64 versions
kernel_asm_%.a: kernel_asm_%.x86_64.a kernel_asm_%.arm64.a
	@libtool -static -o $@ $^

# Rule to build the td_kernel_engine static library
$(TD_KERNEL_LIB): $(KERNEL_OBJS_X86_64) $(STDIO_LIB)
	@echo "Creating td_kernel_lib.a static library"
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
	@echo "Building td_kernel_engine executable"
	$(CC) $(CFLAGS) -o td_kernel_engine.exec src/td_kernel_engine.c $(filter-out $(KERNEL_SRC_DIR)/td_kernel_engine.x86_64.o, $(KERNEL_OBJS_X86_64)) $(STDIO_LIB) $(TD_KERNEL_LIB)


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

# Clean up (excluding specific files)
clean:
	@echo "Cleaning up..."
	@rm -f $(STDIO_LIB) $(KERNEL_LIB) $(KERNEL_ASM_LIB_UNIVERSAL) $(TD_KERNEL_LIB) td_kernel_engine.exec
	@find . -name "*.o" -delete

.PHONY: all clean td_kernel_engine
