#pragma once

//
//  cpeers.h
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 10/12/2016.
//  Copyright © 2016 Jean-Luc Deltombe (LX3JL). All rights reserved.
//  Copyright © 2018 Thomas A. Early, N7TAE
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

#include "cpeer.h"


////////////////////////////////////////////////////////////////////////////////////////
// define


////////////////////////////////////////////////////////////////////////////////////////
// class

class CPeers
{
public:
	// constructors
	CPeers();

	// destructors
	virtual ~CPeers();

	// locks
	void Lock(void)                     { m_Mutex.lock(); }
	void Unlock(void)                   { m_Mutex.unlock(); }

	// manage peers
	void    AddPeer(CPeer *);
	void    RemovePeer(CPeer *);
	CPeer   *GetPeer(std::list<CPeer *>::iterator it) { return (it==m_Peers.end()) ? NULL : *it; }
	std::list<CPeer *>::iterator InitPeerIterator()   { return m_Peers.begin(); }

	// find peers
	CPeer *FindPeer(const CIp &, int);
	CPeer *FindPeer(const CCallsign &, const CIp &, int);
	CPeer *FindPeer(const CCallsign &, int);

	// iterate on peers
	CPeer *FindNextPeer(int, std::list<CPeer *>::iterator &);

protected:
	// data
	std::mutex         m_Mutex;
	std::list<CPeer *> m_Peers;
};

////////////////////////////////////////////////////////////////////////////////////////
