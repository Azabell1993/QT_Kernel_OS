# Makefile for building both kernel_printf.a and kernel_lib.a

# 컴파일러와 플래그 설정
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Iinclude_printf -Wformat -Werror

# 라이브러리 이름
STDIO_LIB = kernel_printf.a
KERNEL_LIB = kernel_lib.a

# 소스 파일과 오브젝트 파일 목록
STDIO_SRC_DIR = include_printf
STDIO_SRCS = $(wildcard $(STDIO_SRC_DIR)/*.c)
STDIO_OBJS_X86_64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.x86_64.o)
STDIO_OBJS_ARM64 = $(STDIO_SRCS:$(STDIO_SRC_DIR)/%.c=$(STDIO_SRC_DIR)/%.arm64.o)

# kernel_lib 관련 설정
KERNEL_SRC_DIR = src
KERNEL_SRCS = $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_OBJS_X86_64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.x86_64.o)
KERNEL_OBJS_ARM64 = $(KERNEL_SRCS:$(KERNEL_SRC_DIR)/%.c=$(KERNEL_SRC_DIR)/%.arm64.o)

# OS 감지 (Linux, macOS)
UNAME_S := $(shell uname -s)

# 기본 타겟: 유니버설 바이너리 생성
all: clean $(STDIO_LIB) $(KERNEL_LIB)

# stdio 라이브러리 생성
$(STDIO_LIB): $(STDIO_OBJS_X86_64) $(STDIO_OBJS_ARM64)
	@echo "Creating universal static library $(STDIO_LIB)"
ifeq ($(UNAME_S),Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# kernel_lib 라이브러리 생성
$(KERNEL_LIB): $(KERNEL_OBJS_X86_64) $(KERNEL_OBJS_ARM64)
	@echo "Creating universal static library $(KERNEL_LIB)"
ifeq ($(UNAME_S),Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# x86_64용 컴파일 (stdio)
$(STDIO_OBJS_X86_64): $(STDIO_SRC_DIR)/%.x86_64.o : $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	@$(CC) $(CFLAGS) -arch x86_64 -c $< -o $@

# arm64용 컴파일 (stdio)
$(STDIO_OBJS_ARM64): $(STDIO_SRC_DIR)/%.arm64.o : $(STDIO_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	@$(CC) $(CFLAGS) -arch arm64 -c $< -o $@

# x86_64용 컴파일 (kernel_lib)
$(KERNEL_OBJS_X86_64): $(KERNEL_SRC_DIR)/%.x86_64.o : $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
	@$(CC) $(CFLAGS) -arch x86_64 -c $< -o $@

# arm64용 컴파일 (kernel_lib)
$(KERNEL_OBJS_ARM64): $(KERNEL_SRC_DIR)/%.arm64.o : $(KERNEL_SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
	@$(CC) $(CFLAGS) -arch arm64 -c $< -o $@

# 정리
clean:
	@echo "Cleaning up..."
	@rm -f $(STDIO_SRC_DIR)/*.o $(KERNEL_SRC_DIR)/*.o $(STDIO_LIB) $(KERNEL_LIB)

.PHONY: all clean
