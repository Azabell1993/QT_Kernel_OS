# Determine the OS and set the compiler
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S), Darwin)      # macOS (both x86_64 and ARM64)
    CC = clang
    AR = libtool -static -o
    ifeq ($(UNAME_M), arm64)
        ARCH_FLAGS = -arch arm64
    else
        ARCH_FLAGS = -arch x86_64
    endif
    CFLAGS = -std=c11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -fcommon
else ifeq ($(UNAME_S), Linux)  # Ubuntu (Linux)
    CC = gcc
    AR = ar rcs
    ARCH_FLAGS =
    CFLAGS = -std=gnu11 -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror -Wno-unused-label -Wno-unused-function -Wno-unused-variable -Wno-format-truncation -fcommon -D_GNU_SOURCE
else
    $(error Unsupported OS: $(UNAME_S))
endif

# Source directories
STDIO_SRC_DIR = include_printf
KERNEL_SRC_DIR = src

# Libraries
STDIO_LIB = kernel_printf.a
KERNEL_LIB = kernel_lib.a
KERNEL_ENGINE_LIB = kernel_engine.a
KERNEL_CHAT_LIB = kernel_chat.a
TD_KERNEL_ENGINE = td_kernel_engine.a

# Object files for different architectures (template)
STDIO_SRCS = $(wildcard $(STDIO_SRC_DIR)/*.c)
KERNEL_SRCS = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_OBJS_X86_64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.x86_64.o)
STDIO_OBJS_X86_64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.x86_64.o)
KERNEL_OBJS_ARM64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.arm64.o)
STDIO_OBJS_ARM64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.arm64.o)

# Default target
all: $(STDIO_LIB) $(KERNEL_LIB) $(KERNEL_ENGINE_LIB) $(KERNEL_CHAT_LIB) $(TD_KERNEL_ENGINE) td_kernel_engine

# Rule to create kernel_printf.a
$(STDIO_LIB): $(STDIO_OBJS_X86_64) $(STDIO_OBJS_ARM64)
	@echo "Creating static library $(STDIO_LIB)"
	$(AR) $@ $^
	ranlib $@

# Rule to create kernel_lib.a
$(KERNEL_LIB): $(KERNEL_OBJS_X86_64) $(KERNEL_OBJS_ARM64)
	@echo "Creating static library $(KERNEL_LIB)"
	$(AR) $@ $^
	ranlib $@

# Rule to create kernel_engine.a
$(KERNEL_ENGINE_LIB): $(KERNEL_SRC_DIR)/kernel_engine.x86_64.o
	@echo "Creating kernel_engine.a static library"
	$(AR) $@ $^
	ranlib $@

# Rule to create kernel_chat.a
$(KERNEL_CHAT_LIB): $(KERNEL_SRC_DIR)/kernel_chat.x86_64.o
	@echo "Creating kernel_chat.a static library"
	$(AR) $@ $^
	ranlib $@

$(TD_KERNEL_ENGINE): $(KERNEL_SRC_DIR)/td_kernel_engine.x86_64.o
	@echo "Creating td_kernel_engine.a static library"
	$(AR) $@ $^
	ranlib $@

# Compile .c files to .o files for x86_64
$(KERNEL_SRC_DIR)/%.x86_64.o: $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(STDIO_SRC_DIR)/%.x86_64.o: $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c files to .o files for arm64
$(KERNEL_SRC_DIR)/%.arm64.o: $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(STDIO_SRC_DIR)/%.arm64.o: $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Target to build and link td_kernel_engine executable
td_kernel_engine: $(filter-out $(KERNEL_SRC_DIR)/td_kernel_engine.x86_64.o, $(KERNEL_OBJS_X86_64)) $(STDIO_LIB) $(KERNEL_LIB) $(KERNEL_ENGINE_LIB) $(KERNEL_CHAT_LIB) $(TD_KERNEL_ENGINE)
	@echo "Building td_kernel_engine executable"
	$(CC) $(CFLAGS) -o td_kernel_engine.exec src/td_kernel_engine.c $(KERNEL_LIB) $(KERNEL_ENGINE_LIB) $(STDIO_LIB) $(KERNEL_CHAT_LIB) $(TD_KERNEL_ENGINE)
	
# Clean up
clean:
	@echo "Cleaning up..."
	@rm -f $(STDIO_LIB) $(KERNEL_LIB) $(KERNEL_ENGINE_LIB) $(KERNEL_CHAT_LIB) $(TD_KERNEL_ENGINE) td_kernel_engine.exec
	@find . -name "*.o" -delete

.PHONY: all clean td_kernel_engine