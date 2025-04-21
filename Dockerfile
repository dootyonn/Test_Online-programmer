FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
  g++ \
  ninja-build \
  cmake \
  gdb \
  libgtest-dev \
  libgmock-dev 

USER ubuntu

WORKDIR /home/ubuntu/app
COPY . /home/ubuntu/app

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .
RUN cmake --build build
RUN ctest --test-dir build

USER root
RUN cmake --install build

CMD ["ctest", "--test-dir", "build"]
