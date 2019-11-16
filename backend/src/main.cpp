
#include <dlfcn.h>
#include <sqlite3.h>
#include <proton/container.hpp>
#include "backend.hpp"
#include <errno.h>

using namespace std;

int main(int argc, char **argv) {
    sqlite3 *db;
	void* sql = dlopen("libsqlite3.so", RTLD_LAZY);
	if (NULL == sql)
	{
		cout << "Error: sqlite not found" << endl;
		exit(1);
	} else {
        cout << "sqlite loaded" << endl;
    }

    int rc = sqlite3_open("database.db", &db);
    if (NULL == db){
        cout << "database.db not found" << endl;
        exit (1);
    } else 
        cout << "database.db loaded" << endl;

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


    sqlite3_close(db);

	return 1;
}