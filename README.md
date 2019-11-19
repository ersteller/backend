# backend

3. BACKEND (C++ AND JAVA) 
Create client libraries to interact with selected Message Broker, preferred Qpid over AMQP, but can be any JMS broker. 
Libraries must have classes to: 
- Send a message to MQ 
- Receive message from MQ 
- Persist Messages in a Database (choose Columns appropriate) 
- Return invalid Message to Sender 
We would like to see working solution and review code, project structure, tests. Please consider one of these two options: 
- Create and share your git repository 
- (or) Send, via email, project sources and instructions how to start it 


DONE
1. Send a message to MQ
3. Persist Messages in a Database (choose Columns appropriate)
TODO: 
2. Receive message from MQ
4. Return invalid Message to Sender 
5. review code, 
6. project structure
7. tests

review code, project structure, tests. 


checkout: 
```
git clone https://github.com/ersteller/backend.git --recurse-submodules
git submodule update --init
```

Build docker: 
```
docker build . -t builder:latest -f ./dev/Dockerfile
```

Btart docker with following command:
```
docker run -it -v D:\Users\z_jan_cronus\workspace\backend\build:/build -v D:\Users\z_jan_cronus\workspace\backend\:/project --name builder builder:latest /bin/bash
```

To build qpid-cpp
```
mkdir /project/libs/qpid-cpp/build -p && cd /project/libs/qpid-cpp/build && cmake .. && make
```

To build qpid-proton
maybe we dont need this:  -DCMAKE_CXX_FLAGS=-std=c++11
```
mkdir /project/libs/qpid-proton/build -p && cd /project/libs/qpid-proton/build && cmake .. && make
``` 
To build qpid proton python bindings 
```
cd /project/libs/qpid-proton/build && cmake .. -DSYSINSTALL_PYTHON=ON -DSYSINSTALL_BINDINGS=OFF
```

For building backend 
```
cd /build && cmake /project/backend/ && make
cd /build && cmake -DCMAKE_BUILD_TYPE=Debug /project/backend/ && make 
```
To run a backend test:  
```
cd /build && python /project/tests/loadlib.py
```

To start and stop a local broker call following script
this boker will have a queue named "examples" ready
```
./dev/start_broker.sh
./dev/stop_broker.sh
``` 

To run the application call
```
./backend //<yourBrokerIp>:5672 examples
./backend //192.168.178.56:5672 examples
```
