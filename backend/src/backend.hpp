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
	Backend(const std::string& u, const std::string& a, void(*pfnIsReady)(Backend& b), std::string szDbPath = std::string()); 

	/* destructor */
	~Backend();

	/* the class needs to be initialized; 
	    only then is the sender started and the database opened */
	void init();

	/* when class is ready use this function to send a message to the broker */
	void send(const proton::message &m);

	/* logs a message to the database takes current time and additionally dst and src */
	void logToDatabase(const proton::message& m, std::string dst = std::string(), std::string src = std::string()); 

	/*  blocking call for on_message callback: virtual void on_message(delivery&, message&); */
	proton::message receive(); 

	void on_container_start(proton::container& c) override;
	void on_connection_open(proton::connection& c) override;
	void on_sendable(proton::sender &s) override;
	void on_message(proton::delivery &d, proton::message &m) override;
	void on_connection_close(proton::connection &) override;
private: 
	void* m_pfnSendable;      // similar to void m_pfnSendable(proton::sender &s);
	proton::sender m_sender;
	sqlite3 *m_db;
	std::string m_szDBPath;
};

// TODO: Verweisen Sie hier auf zusätzliche Header, die Ihr Programm erfordert.
