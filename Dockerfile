FROM ubuntu:latest as BASE

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

# Dependencies needed to generate documentation
# RUN apt-get install -y  \
#    epydoc rubygem-yard doxygen

# dependencies needed for bindings
# we install tzdata first to prevent interactive zime zone setup questions
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y tzdata && \
    ln -fs /usr/share/zoneinfo/Europe/Berlin /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata
RUN apt-get install -y swig python-dev ruby-dev python-epydoc
RUN apt-get install -y libsqlite3-dev --fix-missing

RUN mkdir /project

WORKDIR /build