set working-directory := 'out'

default:
  @just --list

# Build the project in debug mode
build:
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=./Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  make

# Build and run the main executable
run: build
  ./Debug/PSI_projekt

# Build and run unit tests
test: build
  ./Debug/all-unit-tests

# Format the code using clang-format
format:
  find ../src -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i

# Lint the code using clang-tidy
lint:
  # Generate compile_commands.json
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  cp compile_commands.json ..
  clang-tidy ../src/**/*.cpp  ../src/**/*.h
