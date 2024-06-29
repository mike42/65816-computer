FROM ubuntu:22.04

RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y \
  build-essential \
  cmake \
  clang \
  cc65 \
  libgtest-dev \
  libboost-dev \
  libboost-program-options-dev \
  libboost-filesystem-dev && \
  rm -rf /var/lib/apt/lists/*

