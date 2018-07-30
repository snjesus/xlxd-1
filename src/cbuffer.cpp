//
//  cbuffer.cpp
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

#include "main.h"
#include <string.h>
#include "cprotocol.h"

////////////////////////////////////////////////////////////////////////////////////////
// constructor

CBuffer::CBuffer(uint8 *buffer, int len)
{
    v.resize(len);
    ::memcpy(data(), buffer, len);
}

////////////////////////////////////////////////////////////////////////////////////////
// set

void CBuffer::Set(uint8 *buffer, int len)
{
    v.resize(len);
    ::memcpy(v.data(), buffer, len);
}

void CBuffer::Set(const char *sz)
{
    v.resize(::strlen(sz)+1);
    ::strcpy((char *)v.data(), sz);
}

void CBuffer::Append(uint8 *buffer, int len)
{
    int n = (int)v.size();
    v.resize(n+len);
    ::memcpy(&(v.data()[n]), buffer, len);
}

void CBuffer::Append(uint8 ui, int len)
{
    int n = (int)v.size();
    v.resize(n+len);
    ::memset(&(v.data()[n]), ui, len);
}

void CBuffer::Append(uint8 ui)
{
    int n = (int)v.size();
    v.resize(n+sizeof(uint8));
    ::memcpy(&(v.data()[n]), &ui, sizeof(uint8));
}

void CBuffer::Append(uint16 ui)
{
    int n = (int)v.size();
    v.resize(n+sizeof(uint16));
    ::memcpy(&(v.data()[n]), &ui, sizeof(uint16));
}

void CBuffer::Append(uint32 ui)
{
    int n = (int)v.size();
    v.resize(n+sizeof(uint32));
    ::memcpy(&(v.data()[n]), &ui, sizeof(uint32));
}

void CBuffer::Append(const char *sz)
{
    Append((uint8 *)sz, (int)strlen(sz));
    Append((uint8)0x00);
}

void CBuffer::ReplaceAt(int i, uint8 ui)
{
    if ( v.size() < (i+sizeof(uint8)) )
    {
        v.resize(i+sizeof(uint8));
    }
    *(uint8 *)(&(v.data()[i])) = ui;
}

void CBuffer::ReplaceAt(int i, uint16 ui)
{
    if ( v.size() < (i+sizeof(uint16)) )
    {
        v.resize(i+sizeof(uint16));
    }
    *(uint16 *)(&(v.data()[i])) = ui;
}

void CBuffer::ReplaceAt(int i, uint32 ui)
{
    if ( v.size() < (i+sizeof(uint32)) )
    {
        v.resize(i+sizeof(uint32));
    }
    *(uint32 *)(&(v.data()[i])) = ui;
}

void CBuffer::ReplaceAt(int i, const uint8 *ptr, int len)
{
    if ( v.size() < (unsigned int)(i+len) )
    {
        v.resize(i+len);
    }
    ::memcpy(&(v.data()[i]), ptr, len);
}

////////////////////////////////////////////////////////////////////////////////////////
// operation

int CBuffer::Compare(uint8 *buffer, int len) const
{
    int result = -1;
    if ( v.size() >= (unsigned int)len )
    {
        result = ::memcmp(v.data(), buffer, len);
    }
    return result;
}

int CBuffer::Compare(uint8 *buffer, int off, int len) const
{
    int result = -1;
    if ( v.size() >= (unsigned int)(off+len) )
    {
        result = ::memcmp(&(v.data()[off]), buffer, len);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////////
// operator

bool CBuffer::operator ==(const CBuffer &Buffer) const
{
    if ( size() == Buffer.size() )
    {
        return (::memcmp((const char *)v.data(), (const char *)Buffer.data(), v.size()) == 0);
    }
    return false;
}

bool CBuffer::operator ==(const char *sz) const
{
    if ( v.size() == ::strlen(sz) )
    {
        return (::memcmp((const char *)v.data(), sz, v.size()) == 0);
    }
    return false;
}

CBuffer::operator const char *() const
{
    return (const char *)v.data();
}

////////////////////////////////////////////////////////////////////////////////////////
// debug

void CBuffer::DebugDump(std::ofstream &debugout) const
{
    for ( unsigned int i = 0; i < size(); i++ )
    {
        char sz[16];
        //sprintf(sz, "%02X", data()[i]);
        sprintf(sz, "0x%02X", data()[i]);
        debugout << sz;
        if ( i == v.size()-1 )
        {
            debugout << std::endl;
        }
        else
        {
            debugout << ',';
        }
    }
}

