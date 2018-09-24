#pragma once

//
//
//  cclients.h
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 31/10/2015.
//  Copyright © 2015 Jean-Luc Deltombe (LX3JL). All rights reserved.
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

#include "cclient.h"


////////////////////////////////////////////////////////////////////////////////////////
// define


////////////////////////////////////////////////////////////////////////////////////////
// class

class CClients
{
public:
	// constructors
	CClients();

	// destructors
	virtual ~CClients();

	// locks
	void Lock(void)                     { m_Mutex.lock(); }
	void Unlock(void)                   { m_Mutex.unlock(); }

	// manage Clients
	void AddClient(CClient *);
	void RemoveClient(CClient *);
	void RemoveAllClients();
	std::list<CClient *>::iterator InitClientIterator()   { return m_Clients.begin(); }
	CClient *GetClient(std::list<CClient *>::iterator it) { return (it==m_Clients.end()) ? NULL : *it; }
	bool    IsClient(CClient *) const;

	// find clients
	CClient *FindClient(const CIp &);
	CClient *FindClient(const CIp &, int);
	CClient *FindClient(const CIp &, int, char);
	CClient *FindClient(const CCallsign &, const CIp &, int);
	CClient *FindClient(const CCallsign &, char, const CIp &, int);
	CClient *FindClient(const CCallsign &, int);

	// iterate on clients
	CClient *FindNextClient(int, std::list<CClient *>::iterator &);
	CClient *FindNextClient(const CIp &, int, std::list<CClient *>::iterator &);
	CClient *FindNextClient(const CCallsign &, const CIp &, int, std::list<CClient *>::iterator &);

protected:
	// data
	std::mutex           m_Mutex;
	std::list<CClient *> m_Clients;
};


////////////////////////////////////////////////////////////////////////////////////////
