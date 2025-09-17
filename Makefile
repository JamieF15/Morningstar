CMAKE ?= cmake
BUILD_DIR ?= build
CONFIG ?= Debug
GENERATOR ?= Visual Studio 17 2022
ARCH ?= x64
TARGET ?= Morningstar_tests
CTEST ?= ctest

# Add .exe suffix on Windows
ifeq ($(OS),Windows_NT)
  EXE_EXT := .exe
else
  EXE_EXT :=
endif

# Paths to test binary
TEST_BIN := $(BUILD_DIR)/$(CONFIG)/$(TARGET)$(EXE_EXT)
TEST_BIN_SC := $(BUILD_DIR)/$(TARGET)$(EXE_EXT)

.PHONY: all configure build test run clean rebuild test-only

# Default: configure + build + run tests
all: configure build test

# Configure CMake project
configure:
	$(CMAKE) -S . -B $(BUILD_DIR) -G "$(GENERATOR)" -A $(ARCH)

# Build all targets
build: configure
	$(CMAKE) --build $(BUILD_DIR) --config $(CONFIG)

# Run CTest
test: build
	$(CTEST) -C $(CONFIG) --test-dir $(BUILD_DIR) --output-on-failure

# Run the test executable directly
run: build
ifneq ("$(wildcard $(TEST_BIN))","")
	"$(TEST_BIN)"
else ifneq ("$(wildcard $(TEST_BIN_SC))","")
	"$(TEST_BIN_SC)"
else
	$(error Could not find test binary at $(TEST_BIN) or $(TEST_BIN_SC).)
endif

# Run only the test binary (no build)
run-only:
ifneq ("$(wildcard $(TEST_BIN))","")
	"$(TEST_BIN)"
else ifneq ("$(wildcard $(TEST_BIN_SC))","")
	"$(TEST_BIN_SC)"
else
	$(error Could not find test binary at $(TEST_BIN) or $(TEST_BIN_SC).)
endif

# Clean build directory
clean:
	@echo "Removing $(BUILD_DIR)"
ifeq ($(OS),Windows_NT)
	-@rmdir /s /q $(BUILD_DIR) 2>nul || true
else
	-@rm -rf $(BUILD_DIR) 2> /dev/null || true
endif

# Force reconfigure + rebuild
rebuild: clean all

# Build only the test target
test-target: configure
	$(CMAKE) --build $(BUILD_DIR) --config $(CONFIG) --target $(TARGET)

# Run CTest only (without building)
test-only:
	$(CTEST) -C $(CONFIG) --test-dir $(BUILD_DIR) --output-on-failure
