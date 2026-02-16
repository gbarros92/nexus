FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build \
    libeigen3-dev libboost-all-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . /src

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=ON \
    -DNEXUS_BUILD_PYTHON=OFF \
 && cmake --build build \
 && ctest --test-dir build --output-on-failure
