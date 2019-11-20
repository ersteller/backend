# Backend
### Task Description 
> 
> 3. BACKEND (C++ AND JAVA) 
> Create client libraries to interact with selected Message Broker, preferred Qpid over AMQP, but can be any JMS Broker.
>  
> Libraries must have classes to: 
> - Send a message to MQ 
> - Receive message from MQ 
> - Persist Messages in a Database (choose Columns appropriate) 
> - Return invalid Message to Sender 
> 
> We would like to see working solution and review code, project structure, tests. Please consider > one of these two options: 
> - Create and share your git repository 
> - (or) Send, via email, project sources and instructions how to start it 

### DONE:
1. Project structure: Build tools, libraries, source, cmake project
1. Send a message to MQ: Class BSender
2. Receive message from MQ: Class BReceiver
3. Persist Messages in a Database Class Backend:

### TODO: 
- Return invalid Message to Sender: not jet implemented 
- Tests: for lack of modern test framework. With more time and reason i would look into googletest.



# Instructions 
I have tested this with windows10 but Linux should work as well. 

You can build localy with cmake if you have the libqpid-proton-cpp.so and sqlite3.so. Then Cmake is required. 

Alternatively you can use **Docker** for building ```qpid_proton``` and ```backend``` and testing. 
Here I describe the Docker approch. 

## Requirements
- Docker

### checkout: 
```
git clone https://github.com/ersteller/backend.git --recurse-submodules
git submodule update --init
```

### Go into project directory and create a build directory
``` 
cd backend && mkdir build
```

### Build docker image *builder*: 
```
docker build --rm  . -t builder:latest -f ./dev/Dockerfile
```

### Start docker with following command:
```
# Windows PS:
docker run -it -v  ${pwd}/build:/build -v ${pwd}:/project --name builder builder:latest /bin/bash

# Linux: 
docker run -it -v  $(pwd)/build:/build -v $(pwd):/project --name builder builder:latest /bin/bash
```

(OPTIONAL): To build qpid-cpp in docker
```
mkdir /project/libs/qpid-cpp/build -p && cd /project/libs/qpid-cpp/build && cmake .. && make
```

### To build qpid-proton in docker
```
mkdir /project/libs/qpid-proton/build -p && cd /project/libs/qpid-proton/build && cmake .. && make
``` 

(OPTIONAL): To build qpid proton python bindings 
```
cd /project/libs/qpid-proton/build && cmake .. -DSYSINSTALL_PYTHON=ON -DSYSINSTALL_BINDINGS=OFF
```

### For building backend in docker
```
# Release build
cd /build && cmake /project/backend/ && make

# Debug build with symbols unoptimized
cd /build && cmake -DCMAKE_BUILD_TYPE=Debug /project/backend/ && make 
```

To start and stop a local broker call following script on your **host machine**
This broker will have a queue named "examples" ready
```
./dev/start_broker.sh
./dev/stop_broker.sh
``` 

To run the application get your IP (to tell backend where the broker is) with "ipconfig" or "ip -a" call the binary in docker with the following command.
```
# syntax
./backend //<yourIp>:5672 examples
# example
./backend //192.168.178.56:5672 examples
```

There should be 6 messages in the database.db now. You can verify the content with the following command
```
echo "SELECT * from messagelog;" | sqlite3 database.db
```

# Debugging
Debugging in docker container is streight forward
1. Make sure the builder container is running. 
2. Replace the ip in ```.vscode/launch.json``` with your ip. ```"args": ["//192.168.178.56:5672", "examples"],``` 
3. Open vs code on the project next to the README.md and press F5 (cpp vscode extension required)
