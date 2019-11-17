

#include <proton/container.hpp>
#include "backend.hpp"

using namespace std;

static void testFunction(proton::sender &s){

	

	// 1. Send a message to MQ

	// 2. Receive message from MQ

	// 3. Persist Messages in a Database (choose Columns appropriate)
    // 4. Return invalid Message to Sender 

}

int main(int argc, char **argv) {
	int iRet = 1;

	try {
		std::string conn_url = argc > 1 ? argv[1] : "//127.0.0.1:5672";
		std::string addr = argc > 2 ? argv[2] : "examples";

		/* create instance of ouf class */
		Backend backend(conn_url, addr, testFunction, "database.db");
		backend.init();

		/* let proton handle handshaking */
	    proton::container(backend).run();

		iRet = 0;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
	/* we are done now and start to clean up */
	cout << "cleanup now" << endl;


	return iRet;
}