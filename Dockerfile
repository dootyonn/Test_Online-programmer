FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
  build-essential \
  ninja-build \
  cmake \
  cppcheck \
  libgtest-dev \
  libgmock-dev

WORKDIR /app
COPY . /app

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .
RUN cmake --build build
RUN ctest --test-dir build
RUN cmake --install build

CMD ["ctest", "--test-dir", "build"]
