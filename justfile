set working-directory := 'out'

default:
  @just --list

# Build the project in debug mode
build:
  cmake ..

# Build and run the main executable
run: build
  ./Debug/PSI_projekt

# Build and run unit tests
test: build
  ./Debug/all-unit-tests

# Format the code using clang-format
format:
  clang-format -i ../src/**/*.cpp  ../src/**/*.h

# Lint the code using clang-tidy
just lint:
  clang-tidy ../src/**/*.cpp  ../src/**/*.h

