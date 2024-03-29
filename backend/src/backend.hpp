﻿// backend.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <proton/container.hpp>
#include <proton/message.hpp>
#include <proton/listen_handler.hpp>
#include <proton/listener.hpp>
#include <proton/messaging_handler.hpp>
#include <iostream>
#include <sqlite3.h>


class Backend : public proton::messaging_handler {
public:

	/* the constructor takes a function pfnIsReady that is
	    called with an object that has a member function send and receive */
	Backend();
	Backend(const std::string& u, const std::string& a, void(*pfnIsReady)(void* b), std::string szDbPath, int c);
	/* destructor */
	~Backend();


	/* the class needs to be initialized;
	    only then is the sender started and the database opened */
	void init();

	/* member to callback function */
	void clb(void* pvInstance);

	/* logs a message to the database takes current time and additionally dst and src */
	void logToDatabase(const proton::message& m, std::string dst = std::string(), std::string src = std::string());

	void on_container_start(proton::container& c) override;
	void on_connection_open(proton::connection& c) override;
	// void on_sendable(proton::sender &s) override;
	// void on_message(proton::delivery &d, proton::message &m) override;

	class listener_ready_handler : public proton::listen_handler {
		void on_open(proton::listener& l) override {
			std::cout << "listening on " << l.port() << std::endl;
		}
	};
private:

protected:
	std::string conn_url_; // this should be protected 
	std::string addr_;
	
	void* m_pfnSendable;      // similar to void m_pfnSendable(proton::sender &s);
	proton::sender m_sender;
	sqlite3 *m_db;
	std::string m_szDBPath;
	listener_ready_handler listen_handler;
};


class BSender : public Backend {
  private:

    proton::listener listener;
    int sent;
    int confirmed;
    int total;

  public:
    BSender(const std::string &s, 
	            const std::string& a, 
				void(*pfnIsReady)(void* b),
				std::string szDbPath = std::string(), 
				int c=1);
	~BSender();
	
	/* when class is ready use this function to send a message to the broker */
	void send(const proton::message &m);

    void on_container_start(proton::container &c) override;
    void on_sendable(proton::sender &sender) override;
    void on_tracker_accept(proton::tracker &t) override;
    void on_transport_close(proton::transport &) override;
	void on_connection_open(proton::connection&) override;
	void on_connection_close(proton::connection &) override;
};


class BReceiver : public Backend {
  private:
    proton::listener listener;
    int expected;
    int received;

  public:
    BReceiver( const std::string &s, 
	             const std::string& a, 
				 void(*pfnIsReady)(void* b), 
				 std::string szDbPath, 
				 int c);
	~BReceiver();

	/*  blocking call for on_message callback: virtual void on_message(delivery&, message&); */
	proton::message receive();

    void on_container_start(proton::container &c) override;
    void on_message(proton::delivery &d, proton::message &msg) override;
	void on_connection_open(proton::connection& c) override;
};


