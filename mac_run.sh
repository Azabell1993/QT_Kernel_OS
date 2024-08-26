#!/bin/bash

# 사용법 체크
if [ $# -ne 1 ]; then
    echo "Usage: $0 <build-directory>"
    exit 1
fi

BUILD_DIR=$1

# 빌드 디렉터리 존재 여부 확인
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Directory $BUILD_DIR does not exist."
    exit 1
fi

# 빌드 디렉터리로 이동
cd "$BUILD_DIR" || exit 1

# CMake 실행
cmake ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi

# Make 실행
make
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

# 애플리케이션 실행
if [ -f "./os.app/Contents/MacOS/os" ]; then
    ./os.app/Contents/MacOS/os
else
    echo "Executable not found: ./os.app/Contents/MacOS/os"
    exit 1
fi
