#pragma once

//
//  cpacketqueue.h
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 02/11/2015.
//  Copyright © 2015 Jean-Luc Deltombe (LX3JL). All rights reserved.
//
// ----------------------------------------------------------------------------
//    This file is part of xlxd.
//
//    xlxd is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    xlxd is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#include "cpacket.h"
#include "cclient.h"

////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// CPacketQueue

class CClient;

class CPacketQueue
{
public:
	// constructor
	CPacketQueue() {}

	// destructor
	virtual ~CPacketQueue() { while (! q.empty()) { delete front(); pop(); } }

	// lock
	void Lock()                 { m_Mutex.lock(); }
	void Unlock()               { m_Mutex.unlock(); }

	// methods
	bool empty() const				{ return q.empty(); }
	CPacket *front()				{ return q.front(); }
	const CPacket *front() const	{ return q.front(); }
	void pop()						{ q.pop(); }
	void push(CPacket *packet)		{ q.push(packet); }

protected:
	// status
	bool        m_bOpen;
	uint16      m_uiStreamId;
	std::mutex  m_Mutex;

	// owner
	CClient     *m_Client;
	std::queue<CPacket *> q;
};

////////////////////////////////////////////////////////////////////////////////////////
