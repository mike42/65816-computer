FROM ubuntu:22.04

RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y \
  build-essential \
  cmake \
  clang \
  cc65 \
  libgtest-dev \
  libboost1.74-dev \
  libboost-program-options1.74-dev \
  libboost-filesystem1.74-dev && \
  rm -rf /var/lib/apt/lists/*

