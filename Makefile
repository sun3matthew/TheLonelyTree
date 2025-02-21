# Set the build directory

BUILD_DIR = build
BUILD_DIR_OSX = $(BUILD_DIR)/osx
BUILD_DIR_WIN = $(BUILD_DIR)/win

# Default build type (Release or Debug)
BUILD_TYPE ?= Release

PROJECT_NAME = TheLonelyTree

# Application bundle variables
APP_NAME = TheLonelyTreebase
APP_BUNDLE = $(BUILD_DIR)/$(APP_NAME).app
APP_CONTENTS = $(APP_BUNDLE)/Contents
APP_MACOS = $(APP_CONTENTS)/MacOS
APP_RESOURCES = $(APP_CONTENTS)/Resources

run-lldb: BUILD_TYPE = debug

# Default target to configure and build
all: build

# Configure the project (this will invoke CMake with the build type)
# cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..
configure:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

# Build the project (invokes make)
build: configure
	cd $(BUILD_DIR) && ninja $(PROJECT_NAME)

# Run the built project
run: build
	cd $(BUILD_DIR) && ./$(PROJECT_NAME)

run-lldb: build
	cd $(BUILD_DIR) && lldb ./$(PROJECT_NAME)

# Clean the build files
clean:
	rm -rf $(BUILD_DIR)

build-osx:
	mkdir -p $(BUILD_DIR_OSX)
	cd $(BUILD_DIR_OSX) && cmake -G Ninja -DBUILD_TARGET=OSX ../..
	cd $(BUILD_DIR_OSX) && ninja $(PROJECT_NAME)

post-process: merge-images-leaves-diffuse merge-images-leaves-normal

merge-images-leaves-diffuse:
	python scripts/imageMerge.py data/leaves/diffuse resources/textures/tree/leaves/Diffuse.png

merge-images-leaves-normal:
	python scripts/imageMerge.py data/leaves/normal resources/textures/tree/leaves/Normal.png

.PHONY: all configure build run clean release debug build-osx merge-images