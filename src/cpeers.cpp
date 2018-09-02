//
//  cpeers.cpp
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

#include "main.h"
#include "creflector.h"
#include "cpeers.h"


////////////////////////////////////////////////////////////////////////////////////////
// constructor


CPeers::CPeers()
{
}

////////////////////////////////////////////////////////////////////////////////////////
// destructors

CPeers::~CPeers()
{
	m_Mutex.lock();
	while (! m_Peers.empty()) {
		auto it = m_Peers.begin();
		delete *it;
		m_Peers.erase(it);
	}
	m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////////////
// manage peers

void CPeers::AddPeer(CPeer *peer)
{
	// first check if peer already exists
	for ( auto it=m_Peers.begin(); it!=m_Peers.end(); it++ ) {
		if (*peer == *(*it)) {
			// delete new one
			delete peer;
			//std::cout << "Adding existing peer " << peer->GetCallsign() << " at " << peer->GetIp() << std::endl;
			return;
		}
	}

	// if not, append to the vector
	m_Peers.push_back(peer);
	std::cout << "New peer " << peer->GetCallsign() << " at " << peer->GetIp()
			  << " added with protocol " << peer->GetProtocolName()  << std::endl;
	// and append all peer's client to reflector client list
	// it is double lock safe to lock Clients list after Peers list
	CClients *clients = g_Reflector.GetClients();
	auto it = peer->InitClientIterator();
	CClient *client;
	while (NULL != (client = peer->GetClient(it))) {
		clients->AddClient(client);
		it++;
	}
	g_Reflector.ReleaseClients();

	// notify
	g_Reflector.OnPeersChanged();
}

void CPeers::RemovePeer(CPeer *peer)
{
	// look for the client
	for ( auto it=m_Peers.begin(); it!=m_Peers.end(); it++ ) {
		// compare object pointers
		if ( *it ==  peer ) {
			// found it !
			if ( ! (*it)->IsAMaster() ) {
				// remove all clients from reflector client list
				// it is double lock safe to lock Clients list after Peers list
				CClients *clients = g_Reflector.GetClients();
				auto cit = peer->InitClientIterator();
				CClient *client;
				while (NULL != (client = peer->GetClient(cit))) {
					cit++;
					clients->RemoveClient(client);	// this also delete the client object
				}
				peer->ClearClients();	// just removed all the clients, so we just need to clear the list
				g_Reflector.ReleaseClients();

				// remove it
				std::cout << "Peer " << (*it)->GetCallsign() << " at " << (*it)->GetIp() << " removed" << std::endl;
				delete *it;
				m_Peers.erase(it);
				// notify
				g_Reflector.OnPeersChanged();
				return;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// find peers

CPeer *CPeers::FindPeer(const CIp &Ip, int Protocol)
{
	for ( auto it=m_Peers.begin(); it!=m_Peers.end(); it++ ) {
		if ( ((*it)->GetIp() == Ip)  && ((*it)->GetProtocol() == Protocol)) {
			return *it;
		}
	}

	// done
	return NULL;
}

CPeer *CPeers::FindPeer(const CCallsign &Callsign, const CIp &Ip, int Protocol)
{
	for ( auto it=m_Peers.begin(); it!=m_Peers.end(); it++ ) {
		if ( (*it)->GetCallsign().HasSameCallsign(Callsign) &&
				((*it)->GetIp() == Ip)  &&
				((*it)->GetProtocol() == Protocol) ) {
			return *it;
		}
	}

	// done
	return NULL;
}

CPeer *CPeers::FindPeer(const CCallsign &Callsign, int Protocol)
{
	for ( auto it=m_Peers.begin(); it!=m_Peers.end(); it++ ) {
		if ( ((*it)->GetProtocol() == Protocol) &&
				(*it)->GetCallsign().HasSameCallsign(Callsign) ) {
			return *it;
		}
	}

	// done
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////////////
// iterate on peers

CPeer *CPeers::FindNextPeer(int Protocol, std::list<CPeer *>::iterator &it)
{
	while ( it != m_Peers.end() ) {
		if ( (*it)->GetProtocol() == Protocol ) {
			return *it++;
		}
		it++;
	}
	return NULL;
}

