# Set the build directory

BUILD_DIR = build
BUILD_DIR_DEBUG = debug

BUILD_DIR_OSX = $(BUILD_DIR)/osx
BUILD_DIR_WIN = $(BUILD_DIR)/win

# Default build type (Release or Debug)
PROJECT_NAME = TheLonelyTree

# Default target to configure and build
all: build

# Configure the project (this will invoke CMake with the build type)
# cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..
configure:
	cmake -E make_directory $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -S . -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build the project
build: configure
	cmake --build $(BUILD_DIR) --target $(PROJECT_NAME)

run: build
ifeq ($(OS),Windows_NT)
	cd $(BUILD_DIR) && $(PROJECT_NAME).exe;
else
	cd $(BUILD_DIR) && ./$(PROJECT_NAME);
endif



configure-debug:
	cmake -E make_directory $(BUILD_DIR_DEBUG)
	cmake -B $(BUILD_DIR_DEBUG) -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug

build-debug: configure-debug
	cmake --build $(BUILD_DIR_DEBUG) --target $(PROJECT_NAME)

run-debug: build-debug
	cd $(BUILD_DIR_DEBUG) && ./$(PROJECT_NAME)

run-lldb: build-debug
	cd $(BUILD_DIR_DEBUG) && lldb ./$(PROJECT_NAME)



# Clean the build files
clean:
	cmake -E remove_directory $(BUILD_DIR)


# Application bundle variables
APP_NAME = TheLonelyTreebase
APP_BUNDLE = $(BUILD_DIR)/$(APP_NAME).app
APP_CONTENTS = $(APP_BUNDLE)/Contents
APP_MACOS = $(APP_CONTENTS)/MacOS
APP_RESOURCES = $(APP_CONTENTS)/Resources

build-osx:
	cmake -E make_directory $(BUILD_DIR_OSX)
	cd $(BUILD_DIR_OSX) && cmake -G Ninja -DBUILD_TARGET=OSX ../..
	cd $(BUILD_DIR_OSX) && ninja $(PROJECT_NAME)

BUILD_DIR_WIN_TARGET = $(BUILD_DIR_WIN)/$(PROJECT_NAME)
BIN_DIR_WIN = $(BUILD_DIR_WIN_TARGET)/bin
RESOURCES_DIR_WIN = $(BUILD_DIR_WIN_TARGET)/resources

build-win:
	cmake -E make_directory $(BIN_DIR_WIN)
	cmake -E make_directory $(RESOURCES_DIR_WIN)
	cmake -B $(BIN_DIR_WIN) -S . -G Ninja -DBUILD_TARGET=Windows -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BIN_DIR_WIN) --target $(PROJECT_NAME)


post-process: merge-images-leaves-diffuse merge-images-leaves-normal

merge-images-leaves-diffuse:
	python scripts/imageMerge.py data/leaves/diffuse resources/textures/tree/leaves/Diffuse.png

merge-images-leaves-normal:
	python scripts/imageMerge.py data/leaves/normal resources/textures/tree/leaves/Normal.png

.PHONY: all configure build run clean release debug build-osx build-win merge-images