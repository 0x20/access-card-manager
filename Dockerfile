FROM ubuntu:22.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt upgrade -y
RUN apt install -y build-essential pkg-config cmake libboost-all-dev libjsoncpp-dev

COPY . /work
WORKDIR /work
RUN mkdir build && cd build && cmake .. && make -j8
RUN strip /work/build/access-card-manager

FROM ubuntu:22.04
LABEL maintainer="Yvan Janssens <yvanj@cyber.gent>"
ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt upgrade -y
RUN apt install -y libboost-date-time1.74.0 libjsoncpp25 libboost-filesystem1.74.0 libssl3 libboost-thread1.74.0 libboost-program-options1.74.0
RUN mkdir /static
COPY --from=builder /work/wt/resources /static/resources
COPY --from=builder /work/build/access-card-manager /
EXPOSE 9090
COPY ./docker_entrypoint.sh /
RUN chmod a+x /docker_entrypoint.sh
CMD ["/docker_entrypoint.sh"]
