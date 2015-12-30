//
//  cprotocol.cpp
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 01/11/2015.
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
#include "cprotocol.h"
#include "cclients.h"
#include "creflector.h"


////////////////////////////////////////////////////////////////////////////////////////
// constructor


CProtocol::CProtocol()
{
    m_bStopThread = false;
    m_pThread = NULL;
    m_Streams.reserve(NB_OF_MODULES);
}


////////////////////////////////////////////////////////////////////////////////////////
// destructor

CProtocol::~CProtocol()
{
    // kill threads
    m_bStopThread = true;
    if ( m_pThread != NULL )
    {
        m_pThread->join();
        delete m_pThread;
    }
    
    // empty queue
    m_Queue.Lock();
    while ( !m_Queue.empty() )
    {
        m_Queue.pop();
    }
    m_Queue.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////////
// initialization

bool CProtocol::Init(void)
{
    // init reflector apparent callsign
    m_ReflectorCallsign = g_Reflector.GetCallsign();
    
    // reset stop flag
    m_bStopThread = false;

    // start  thread;
    m_pThread = new std::thread(CProtocol::Thread, this);
    
    // done
    return true;
}

void CProtocol::Close(void)
{
    m_bStopThread = true;
    if ( m_pThread != NULL )
    {
        m_pThread->join();
        delete m_pThread;
        m_pThread = NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////////
// thread

void CProtocol::Thread(CProtocol *This)
{
    while ( !This->m_bStopThread )
    {
        This->Task();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// packet encoding helpers

bool CProtocol::EncodeDvPacket(const CPacket &packet, CBuffer *buffer) const
{
    bool ok = false;
    if ( packet.IsDvFrame() )
    {
        if ( packet.IsLastPacket() )
        {
            ok = EncodeDvLastFramePacket((const CDvLastFramePacket &)packet, buffer);
        }
        else
        {
            ok = EncodeDvFramePacket((const CDvFramePacket &)packet, buffer);
        }
    }
    else if ( packet.IsDvHeader() )
    {
        ok = EncodeDvHeaderPacket((const CDvHeaderPacket &)packet, buffer);
    }
    else
    {
        buffer->clear();
    }
    return ok;
}

////////////////////////////////////////////////////////////////////////////////////////
// streams helpers

void CProtocol::OnDvFramePacketIn(CDvFramePacket *Frame)
{
    // find the stream
    CPacketStream *stream = GetStream(Frame->GetStreamId());
    if ( stream != NULL )
    {
        //std::cout << "DV frame" << std::endl;
        // and push
        stream->Lock();
        stream->Push(Frame);
        stream->Unlock();
    }
}

void CProtocol::OnDvLastFramePacketIn(CDvLastFramePacket *Frame)
{
    // find the stream
    CPacketStream *stream = GetStream(Frame->GetStreamId());
    if ( stream != NULL )
    {
        // push
        stream->Lock();
        stream->Push(Frame);
        stream->Unlock();
        
        // and close the stream
        g_Reflector.CloseStream(stream);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// stream handle helpers

CPacketStream *CProtocol::GetStream(uint16 uiStreamId)
{
    CPacketStream *stream = NULL;
    for ( int i = 0; (i < m_Streams.size()) && (stream == NULL); i++ )
    {
        if ( m_Streams[i]->GetStreamId() == uiStreamId )
        {
            stream = m_Streams[i];
        }
    }
    return stream;
}

void CProtocol::CheckStreamsTimeout(void)
{
    for ( int i = 0; i < m_Streams.size(); i++ )
    {
        // time out ?
        m_Streams[i]->Lock();
        if ( m_Streams[i]->IsExpired() )
        {
            // yes, close it
            m_Streams[i]->Unlock();
            g_Reflector.CloseStream(m_Streams[i]);
            // and remove it
            m_Streams.erase(m_Streams.begin()+i);
        }
        else
        {
            m_Streams[i]->Unlock();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// queue helper

void CProtocol::HandleQueue(void)
{
    // the default protocol just keep queue empty
    m_Queue.Lock();
    while ( !m_Queue.empty() )
    {
        delete m_Queue.front();
        m_Queue.pop();
    }
    m_Queue.Unlock();
}


