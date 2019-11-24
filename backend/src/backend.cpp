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

#include <ostream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>

#include "backend.hpp"

using namespace std;

typedef void (*ClbFunctionType)(void* s);

Backend::Backend(const std::string& u, const std::string& a, void(*funSendable)(void* s), string szDbPath, int c) {		
    conn_url_ = u;
	addr_ = a;
	m_pfnSendable = (void*)funSendable;	
	m_szDBPath = szDbPath;
	m_db = NULL;
}

/* default constructo */
Backend::Backend(){}

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

void Backend::clb(void* pvinstance){
	if (m_pfnSendable)
	{
		ClbFunctionType clbSendable = (ClbFunctionType)m_pfnSendable;
		clbSendable(pvinstance);
	}
}

static int dbCallback(void*,int,char**,char**)
{
	return 0;
}

void Backend::logToDatabase(const proton::message& m, std::string dst, std::string src){

	char* szError = NULL;
	/* time, topic, src, dst, message matches pur database template database.db */
	const char* szSqlFmt = "INSERT INTO messagelog (time, topic, src, dst, message) VALUES (%" PRId64 ", \"%s\", \"%s\", \"%s\", \"%s\"); ";
	char szSql[4096] = {};

	const char* szTopic = addr_.c_str();
	const char* szDest = dst.c_str();     // m.to().c_str();
	const char* szSrc = src.c_str();

	/* convert message to string */
	proton::value value = m.body();
	std::ostringstream os;
    os << std::boolalpha << value;
    std::string strmessage = os.str();
	const char* szBody = strmessage.c_str();       // m.body().c_str(); is not working so we explicitly convert to ostringstream and then to char* 

	/* get a timestamp of microsec */
	uint64_t ullTsusec = 0;
	struct timeval tv = {0};
	gettimeofday( &tv, NULL);
	ullTsusec = tv.tv_sec * 1000000 + tv.tv_usec;
	
	/* prepare the sql statement to be evaluated */
	sprintf(szSql, szSqlFmt, ullTsusec, szTopic, szSrc, szDest, szBody);
	sqlite3_exec(
					m_db,                                     /* An open database */
					szSql,                                    /* SQL to be evaluated */
					dbCallback,                               /* Callback function */
					(void*)this,                              /* 1st argument to callback */
					&szError                                  /* Error msg written here */
				);
}

