set working-directory := 'out'

default:
  @just --list

# Build the test executable
build_tests:
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=./Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_STANDARD=20 ..
  make all-unit-tests

# Build the project in debug mode
build:
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=./Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_STANDARD=20 ..
  make PSI_projekt

# Build and run the main executable listening on the given port, logging to the given file
run port logfile: build
  ./Debug/PSI_projekt {{port}} {{logfile}}

# Build and run unit tests
test: build_tests
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
