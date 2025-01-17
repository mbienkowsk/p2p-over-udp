# Base stage: common setup for all tasks
FROM ubuntu:22.04 AS base

# Set non-interactive frontend for apt-get
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y

# Install required packages: cmake, clang-format, clang-tidy, build tools, and curl for installing 'just'
RUN apt-get install -y \
    cmake \
    clang-format \
    clang-tidy \
    build-essential \
    curl \
    libssl-dev \
    pkg-config \
    libboost-all-dev \
    libspdlog-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Install just and update path
RUN curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to /usr/local/bin

# Set working directory
WORKDIR /app

# Copy the entire project into the container
COPY . .

# Create and switch to the 'out' directory as per your Justfile
RUN mkdir -p out
WORKDIR /app/out

ENTRYPOINT ["just"]
