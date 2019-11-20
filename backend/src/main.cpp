
#include "backend.hpp"

using namespace std;


static void testSendFunction(void* pvSender){
	proton::message m("Hello from testcase!");    // proton::message* pcM = new proton::message("Hello from testcase!");
	std::cout << m.body() << std::endl;
	// 1. Send a message to MQ
	BSender* sender = reinterpret_cast<BSender*>(pvSender);
	sender->send(m);
}

// 2. Receive message from MQ
/* // this is not how we get the data from the MQ 
   // it is rather received in on_message and directly put into database
static void testRecvFunction(void* pvReceiver){
	BReceiver* receiver = reinterpret_cast<BReceiver*>(pvReceiver);
	proton::message resMessage = receiver->receive();
	std::cout << resMessage.body() << std::endl;
}*/

int sender_main(std::string& conn_url, std::string& addr) {
    // conn_url, addr, testFunction, "database.db")
    int message_count = 3;
    try {
        BSender send(	conn_url, 
	            		addr, 
						testSendFunction,
						std::string("database.db"), 
						message_count );
		send.init();
        proton::container(send).run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 1;
}

/* receiver main */
int receiver_main(std::string& conn_url, std::string& addr) {
    int message_count = 3;
    try {
        BReceiver recv( conn_url, 
	            		addr, 
						NULL,
						std::string("database.db"), 
						message_count );
		recv.init();
        proton::container(recv).run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 1;
}

int main(int argc, char **argv) {
	int iRet = 1;
	try {
		std::string conn_url = argc > 1 ? argv[1] : "//127.0.0.1:5672";
		std::string addr = argc > 2 ? argv[2] : "examples";

		sender_main(conn_url, addr);
		receiver_main(conn_url, addr);
		iRet = 0; 
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
	/* we are done now and start to clean up */
	cout << "cleanup now" << endl;


	return iRet;
}
