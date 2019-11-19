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


BSender::BSender(const std::string &s, 
	            const std::string& a, 
				void(*pfnIsReady)(Backend& b),
				std::string szDbPath, 
				int c)
{
    conn_url_ = s;
    addr_ = a;
    sent = 0;
    confirmed = 0;
    total = 0;
}

void BSender::on_sendable(proton::sender &sender) {
    while (sender.credit() && sent < total) {
        proton::message msg;
        std::map<std::string, int> m;
        m["sequence"] = sent + 1;

        msg.id(sent + 1);
        msg.body(m);

        sender.send(msg);
        sent++;
    }
}

void BSender::on_container_start(proton::container &c) {
    listener = c.listen(url, listen_handler);
}
void BSender::on_tracker_accept(proton::tracker &t) {
    confirmed++;

    if (confirmed == total) {
        std::cout << "all messages confirmed" << std::endl;
        t.connection().close();
        listener.stop();
    }
}

void BSender::on_transport_close(proton::transport &) {
    sent = confirmed;
}
