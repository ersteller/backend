
#include <dlfcn.h>
#include <proton/container.hpp>
#include "backend.hpp"

using namespace std;

int main(int argc, char **argv) {
	void* sql = dlopen("libsqlite3.so", RTLD_LAZY);
	if (NULL == sql)
	{
		cout << "sqlite not found" << endl;
		exit(1);
	}

	cout << "Hello CMake." << endl;
	try {
		std::string conn_url = argc > 1 ? argv[1] : "//127.0.0.1:5672";
		std::string addr = argc > 2 ? argv[2] : "examples";
		hello_world hw(conn_url, addr);
		proton::container(hw).run();
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 1;
}