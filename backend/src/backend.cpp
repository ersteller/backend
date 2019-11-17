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
#include <stdio.h>
#include <sys/time.h>

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>

#include "backend.hpp"

using namespace std;

typedef void (*ClbFunctionType)(proton::sender &s);

Backend::Backend(const std::string& u, const std::string& a, void(*funSendable)(proton::sender &s), string szDbPath ) {		
    conn_url_ = u;
	addr_ = a;
	m_pfnSendable = (void*)funSendable;	
	m_szDBPath = szDbPath;
}

/* destructor */
Backend::~Backend(){
	sqlite3_close(m_db);
}

void Backend::init(){
	/* if we have a path then we load the database */
	if (!m_szDBPath.empty())
	{
		int rc = sqlite3_open(m_szDBPath.c_str(), &m_db);
		if (NULL == m_db){
			cout << m_szDBPath << " not found" << endl;
			exit (1);
		} else {
			cout << m_szDBPath << " loaded" << endl; 
		}
	}
}

void Backend::on_container_start(proton::container& c)  {
	c.connect(conn_url_);
}
void Backend::on_connection_open(proton::connection& c)  {
	c.open_receiver(addr_);
	c.open_sender(addr_);
}
void Backend::on_sendable(proton::sender &s)  {
	m_sender = s;
    
	if (m_pfnSendable)
	{
		ClbFunctionType clbSendable = (ClbFunctionType)m_pfnSendable;
		clbSendable(s);
	}
	s.close();
}

static int dbCallback(void*,int,char**,char**)
{
	return 0;
}

/* this is the send wrapper four our class */
void Backend::send(const proton::message &m) {

	char* szError = NULL;
	// time, topic, src, dst, message
	const char* szSqlFmt = "INSERT INTO table_name (time, topic, src, dst, message) VALUES (%"PRId64", %s, %s, %s, %s); "
	const char szSql[4096] = {};

	char* szTopic = addr_.c_str();
	char* szSrc = "localhost";
	char* szDest = conn_url_;

	/* get a timestamp of microsec */
	uint64_t ullTsusec = 0;
	struct timeval tv = {0};
	int gettimeofday(&tv, NULL);
	ullTsusec = tv.tv_sec * 1000000 + tv.tv_usec
	
	sprintf(szSql, szSqlFmt, ullTsusec, szTopic, szSrc, szDest, m.body);

	sqlite3_exec(
					m_db,                                      /* An open database */
					sql,                                       /* SQL to be evaluated */
					dbCallbackr,                               /* Callback function */
					(void*)this,                               /* 1st argument to callback */
					&szError                                   /* Error msg written here */
				);
	
	m_sender.send(m);

}

// this is a wrapper with sql log for the base function: virtual void on_message(delivery&, message&);
void Backend::receive(proton::delivery &d,  proton::message &m) { 


	cout << "wait for received message: " << m.body() << endl; 
	return on_message(d, m);
}

void Backend::on_message(proton::delivery &d, proton::message &m)  {
	std::cout << "received: " << m.body() << std::endl;
	d.connection().close();
}
void Backend::on_connection_close(proton::connection &) {
	m_sender.close();
}

