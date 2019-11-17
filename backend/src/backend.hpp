// backend.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <proton/messaging_handler.hpp>
#include <iostream>
#include <sqlite3.h>

class Backend : public proton::messaging_handler {
	std::string conn_url_;
	std::string addr_;

public:
	/* the constructor takes a function pfnIsReady that is
	    called with an object that has a member function send and receive */
	Backend(const std::string& u, const std::string& a, void(*pfnIsReady)(proton::sender &s), std::string szDbPath = std::string()); 

	/* destructor */
	//~Backend();

	/* the class needs to be initialized; 
	    only then is the sender started and the database opened */
	void init();

	//void send();
	//void receive(); // virtual void on_message(delivery&, message&);

	void on_container_start(proton::container& c) override;
	void on_connection_open(proton::connection& c) override;
	void on_sendable(proton::sender &s) override;
	void on_message(proton::delivery &d, proton::message &m) override;
	void on_connection_close(proton::connection &) override;
private: 
	void* m_pfnSendable;  //  void m_pfnSendable(proton::sender &s);
	proton::sender m_sender;
	sqlite3 *m_db;
	std::string m_szDBPath;
};

// TODO: Verweisen Sie hier auf zusätzliche Header, die Ihr Programm erfordert.
