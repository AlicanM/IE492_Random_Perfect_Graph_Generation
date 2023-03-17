FROM danger89/cmake:4.6

WORKDIR /ie492_rpgg

ADD igraph-0.10.4.tar.gz .

WORKDIR /ie492_rpgg/igraph-0.10.4

RUN mkdir build

WORKDIR /ie492_rpgg/igraph-0.10.4/build

RUN cmake ..
RUN cmake --build .
RUN cmake --install .

WORKDIR /ie492_rpgg

COPY . .