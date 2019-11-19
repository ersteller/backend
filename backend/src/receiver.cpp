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

#include "options.hpp"

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/link.hpp>
#include <proton/value.hpp>

#include <iostream>
#include <map>

#include "backend.hpp"

    BReceiver::BReceiver(const std::string &s, int c) : url(s), expected(c), received(0) {}

void BReceiver::on_container_start(proton::container &c)  {
    listener = c.listen(url, listen_handler);
}

void BReceiver::on_message(proton::delivery &d, proton::message &msg) {
    if (proton::coerce<int>(msg.id()) < received) {
        return; // Ignore duplicate
    }

    if (expected == 0 || received < expected) {
        std::cout << msg.body() << std::endl;
        received++;
    }

    if (received == expected) {
        d.receiver().close();
        d.connection().close();
        listener.stop();
    }
}

