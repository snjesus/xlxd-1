//
//  ccallsignlist.h
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 30/12/2015.
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


#ifndef ccallsignlist_h
#define ccallsignlist_h

#include "main.h"
#include "ccallsignlistitem.h"

////////////////////////////////////////////////////////////////////////////////////////
// class

class CCallsignList
{
public:
    // constructor
    CCallsignList();

    // destructor
    virtual ~CCallsignList() {}

    // locks
    void Lock(void)                        { m_Mutex.lock(); }
    void Unlock(void)                      { m_Mutex.unlock(); }

    // file io
    virtual bool LoadFromFile(const char *);
    bool ReloadFromFile(void);
    bool NeedReload(void);

    // compare
    bool IsCallsignListedWithWildcard(const CCallsign &) const;
    bool IsCallsignListedWithWildcard(const CCallsign &, char) const;
    bool IsCallsignListed(const CCallsign &, char) const;
    bool IsCallsignListed(const CCallsign &, char*) const;

    // find
    std::list<CCallsignListItem>::iterator InitCallsignListIterator()	{ return m_Callsigns.begin(); }
    CCallsignListItem *GetCallsignListItem(std::list<CCallsignListItem>::iterator);
    CCallsignListItem *FindListItem(const CCallsign &);

    // functions
    bool empty() const	{ return m_Callsigns.empty(); };
    size_t size() const	{ return m_Callsigns.size(); }

protected:
    //
    bool GetLastModTime(time_t *);
    char *TrimWhiteSpaces(char *);

protected:
    // data
    std::mutex      m_Mutex;
    const char *    m_Filename;
    time_t          m_LastModTime;
    std::list<CCallsignListItem> m_Callsigns;
};


////////////////////////////////////////////////////////////////////////////////////////
#endif /* ccallsignlist_h */
