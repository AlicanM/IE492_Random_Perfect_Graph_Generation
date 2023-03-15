FROM danger89/cmake:4.6

WORKDIR /rpgg

COPY igraph-0.10.4 .

WORKDIR /rpgg/igraph-0.10.4

RUN mkdir igraph-0.10.4_build

WORKDIR /rpgg/igraph-0.10.4_build

RUN cmake ../igraph-0.10.4
RUN cmake --build .
RUN cmake --install .

WORKDIR /rpgg

COPY . .

CMD mkdir build