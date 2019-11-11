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



build docker: 
```
docker build . -t builder:latest
```
start docker with following command:
```
docker run -it -v D:\Users\z_jan_cronus\workspace\backend\build:/build -v D:\Users\z_jan_cronus\workspace\backend\:/project builder:latest /bin/bash
```
