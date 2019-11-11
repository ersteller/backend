From ubuntu:latest as BASE

RUN apt-get update && apt-get install -y  \
    libboost-all-dev \
    ruby-full \
    gcc g++ git-core \
    make \
    cmake \
    sqlite3

RUN mkdir /project

WORKDIR /build