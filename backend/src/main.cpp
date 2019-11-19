


#include "backend.hpp"

using namespace std;


static void testFunction(Backend& backend){


	proton::message m("Hello from testcase!");    // proton::message* pcM = new proton::message("Hello from testcase!");
	std::cerr << m.body() << std::endl;

	// 1. Send a message to MQ
	backend.send(m);

	// 2. Receive message from MQ
	proton::message resMessage;
	resMessage = backend.receive();
	// 3. Persist Messages in a Database (choose Columns appropriate)

    // 4. Return invalid Message to Sender 
}


/* receiver main */
int receiver_main(std::string& conn_url, std::string& addr) {
    int message_count = 100;
    try {
        BReceiver recv(conn_url, 
	            addr, 
				testFunction,
				std::string("database.db"), 
				message_count);
        proton::container(recv).run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 1;
}

int sender_main(std::string& conn_url, std::string& addr) {

    // conn_url, addr, testFunction, "database.db")

    int message_count = 100;
    try {
        BSender send(conn_url, 
	            addr, 
				testFunction,
				std::string("database.db"), 
				message_count);
        proton::container(send).run();
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

		/* create instance of ouf class and let it call our test function when it is ready */
		// Backend backend(conn_url, addr, testFunction, "database.db");
		//backend.init();
	    // proton::container(backend).run();

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
