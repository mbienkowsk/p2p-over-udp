set working-directory := 'out'

default:
  @just --list

# Build the project in debug mode
build:
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=./Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_STANDARD=20 ..
  make

# Build and run the main executable listening on the given port, logging to the given file
run port logfile: build
  ./Debug/PSI_projekt {{port}} {{logfile}}

run_listener: build
  ./Debug/UDP_LISTENER

run_sender: build
  ./Debug/UDP_SENDER

run_manager: build
  ./Debug/RESOURCE_MANAGER

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
