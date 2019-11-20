#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/listen_handler.hpp>
#include <proton/listener.hpp>
#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/value.hpp>
#include <proton/tracker.hpp>
#include <proton/types.hpp>

#include <iostream>
#include <map>

#include "Backend.hpp"


BSender::BSender(const std::string &u, 
	            const std::string& a, 
				void(*pfnIsReady)(void* b),
				std::string szDbPath, 
				int c) : Backend(u, a, pfnIsReady, szDbPath, c)
{
    sent = 0;
    confirmed = 0;
    total = c;
}
BSender::~BSender(){}

void BSender::on_sendable(proton::sender &sender) {
	m_sender = sender;
    Backend::clb(this);
}

void BSender::on_container_start(proton::container &c) {
    //std::string url = conn_url_ + std::string("/") + addr_;
    //listener = c.listen(url, listen_handler);
    c.connect(conn_url_);
    //Backend::on_container_start(c);
}
void BSender::on_tracker_accept(proton::tracker &t) {
    confirmed++;

    if (confirmed == total) {
        std::cout << "all messages confirmed" << std::endl;
        t.connection().close();
        listener.stop();
    }
}

void BSender::on_connection_open(proton::connection& c) {
    c.open_sender(addr_);
}

void BSender::on_transport_close(proton::transport &) {
    sent = confirmed;
}

void BSender::on_connection_close(proton::connection &) {
	m_sender.close();
}

/* this is the send wrapper four our class */
void BSender::send(const proton::message &m) {
	logToDatabase(m, conn_url_, std::string("localhost"));
	m_sender.send(m);
    sent++;

    if (sent >= total)
        m_sender.close();
    
}
