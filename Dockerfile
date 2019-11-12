From ubuntu:latest as BASE

RUN apt-get update && apt-get install -y  \
    libboost-all-dev \
    ruby-full \
    gcc g++ git-core \
    make \
    cmake \
    sqlite3 \
    uuid-dev \
    cmake-curses-gui \
    libssl-dev \
    libsasl2-2 libsasl2-dev libsasl2-modules 
RUN apt-get install -y \
    python-setuptools
    #e2fsprogs \

RUN mkdir /project

WORKDIR /build