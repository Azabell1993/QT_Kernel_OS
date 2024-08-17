# Makefile for building the kernel_lib static library

# 컴파일러와 플래그 설정
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# 라이브러리 이름
LIB_NAME = kernel_lib.a

# 소스 파일과 오브젝트 파일 목록
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)

# 아키텍처에 따른 오브젝트 파일 목록
OBJS_X86_64 = $(SRCS:$(SRC_DIR)/%.c=$(SRC_DIR)/%.x86_64.o)
OBJS_ARM64 = $(SRCS:$(SRC_DIR)/%.c=$(SRC_DIR)/%.arm64.o)

# OS 감지 (Linux, macOS)
UNAME_S := $(shell uname -s)

# 기본 타겟: 유니버설 바이너리 생성
all: clean $(LIB_NAME)

# x86_64용 컴파일
$(OBJS_X86_64): $(SRC_DIR)/%.x86_64.o : $(SRC_DIR)/%.c
	@echo "Compiling for x86_64: $<"
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(CFLAGS) -arch x86_64 -c $< -o $@
else
	@$(CC) $(CFLAGS) -m64 -c $< -o $@
endif

# arm64용 컴파일
$(OBJS_ARM64): $(SRC_DIR)/%.arm64.o : $(SRC_DIR)/%.c
	@echo "Compiling for arm64: $<"
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(CFLAGS) -arch arm64 -c $< -o $@
else
	@$(CC) $(CFLAGS) -m64 -c $< -o $@
endif

# 유니버설 라이브러리 생성
$(LIB_NAME): $(OBJS_X86_64) $(OBJS_ARM64)
	@echo "Creating universal static library $(LIB_NAME)"
ifeq ($(UNAME_S),Darwin)
	@libtool -static -o $@ $^
else
	@ar rcs $@ $^
endif

# 정리
clean:
	@echo "Cleaning up..."
	@rm -f $(SRC_DIR)/*.o $(LIB_NAME)

.PHONY: all clean
