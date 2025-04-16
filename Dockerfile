FROM ubuntu:25.04

WORKDIR /app

RUN apt-get update && apt-get install -y \
  g++ \
  ninja-build \
  cmake \
  gdb \
  libgtest-dev

COPY * .

RUN mkdir build
WORKDIR /app/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local 
RUN cmake --build .
RUN ctest .
RUN cmake --install . 

ENTRYPOINT [ "/usr/local/bin/quiz" ]
CMD [ ]
