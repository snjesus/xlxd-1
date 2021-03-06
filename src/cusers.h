#pragma once

//
//  cusers.h
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 13/11/2015.
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

#include "cuser.h"

////////////////////////////////////////////////////////////////////////////////////////

class CUsers
{
public:
	// constructor
	CUsers();

	// destructor
	virtual ~CUsers() {}

	// locks
	void Lock(void)                     { m_Mutex.lock(); }
	void Unlock(void)                   { m_Mutex.unlock(); }

	// management
	void   AddUser(const CUser &);
	CUser *GetUser(std::list<CUser>::iterator it) { return (it==m_Users.end()) ? NULL : &(*it); }
	std::list<CUser>::iterator InitUserIterator() { return m_Users.begin(); }

	// operation
	void   Hearing(const CCallsign &, const CCallsign &, const CCallsign &);
	void   Hearing(const CCallsign &, const CCallsign &, const CCallsign &, const CCallsign &);

protected:
	// data
	std::mutex          m_Mutex;
	std::list<CUser>  m_Users;
};

////////////////////////////////////////////////////////////////////////////////////////
