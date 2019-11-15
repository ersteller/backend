/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */
#include <iostream>
#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>

#include "backend.hpp"

using namespace std;


hello_world::hello_world(const std::string& u, const std::string& a) :
	conn_url_(u), addr_(a) {}
void hello_world::on_container_start(proton::container& c)  {
	c.connect(conn_url_);
}
void hello_world::on_connection_open(proton::connection& c)  {
	c.open_receiver(addr_);
	c.open_sender(addr_);
}
void hello_world::on_sendable(proton::sender &s)  {
	proton::message m("Hello World!");
	s.send(m);
	s.close();
}
void hello_world::on_message(proton::delivery &d, proton::message &m)  {
	std::cout << m.body() << std::endl;
	d.connection().close();
}
