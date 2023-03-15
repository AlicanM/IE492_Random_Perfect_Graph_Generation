FROM danger89/cmake:4.6

WORKDIR /rpgg

ADD igraph-0.10.4.tar ./igraph-0.10.4

WORKDIR /rpgg/igraph-0.10.4

RUN mkdir build

WORKDIR /rpgg/igraph-0.10.4/build

RUN cmake ..
RUN cmake --build .
RUN cmake --install .

WORKDIR /rpgg

COPY . .

CMD mkdir build