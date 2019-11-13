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
    python-setuptools \
    jsoncpp-devel 

 # Dependencies needed to generate documentation
RUN apt-get install -y  \
    epydoc rubygem-yard doxygen

    # dependencies needed for bindings
RUN apt-get install -y swig python-dev ruby-dev python-epydoc



RUN mkdir /project

WORKDIR /build