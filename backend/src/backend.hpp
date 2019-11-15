// backend.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <proton/messaging_handler.hpp>
#include <iostream>

class hello_world : public proton::messaging_handler {
	std::string conn_url_;
	std::string addr_;
public:
	hello_world(const std::string& u, const std::string& a);

	void on_container_start(proton::container& c) override;
	void on_connection_open(proton::connection& c) override;
	void on_sendable(proton::sender &s) override;
	void on_message(proton::delivery &d, proton::message &m) override;
};

// TODO: Verweisen Sie hier auf zusätzliche Header, die Ihr Programm erfordert.
