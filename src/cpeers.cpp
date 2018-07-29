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
	bool found = false;
	for (auto pit=m_Peers.begin(); pit!=m_Peers.end(); pit++) {
		found = (*peer == *(*pit));
		// if found, just do nothing
		// so *peer keep pointing on a valid object
		// on function return
		if (found)
			delete peer;	// delete new one
	}

	// if not, append to the vector
	if (! found) {
		m_Peers.push_back(peer);
		std::cout << "New peer " << peer->GetCallsign() << " at " << peer->GetIp() << " added with protocol " << peer->GetProtocolName() << std::endl;
		// and append all peer's client to reflector client list
		// it is double lock safe to lock Clients list after Peers list
		CClients *clients = g_Reflector.GetClients();
		CClient *client;
		auto cit = peer->InitClientIterator();
		while (NULL != (client = peer->GetClient(cit))) {
			clients->AddClient(client);
			cit++;
		}
		g_Reflector.ReleaseClients();

		// notify
		g_Reflector.OnPeersChanged();
	}
}

void CPeers::RemovePeer(CPeer *peer)
{
	// look for the client
	for (auto pit=m_Peers.begin(); pit!=m_Peers.end(); pit++) {
		// compare object pointers
		if ((*pit) ==  peer) {
			// found it !
			if (! (*pit)->IsAMaster()) {
				// remove all clients from reflector client list
				// it is double lock safe to lock Clients list after Peers list
				CClients *clients = g_Reflector.GetClients();
				CClient *client;
				auto cit = peer->InitClientIterator();
				while (NULL != (client = peer->GetClient(cit))) {
					clients->RemoveClient(client);
					cit++;
				}
				g_Reflector.ReleaseClients();

				// remove it
				std::cout << "Peer " << (*pit)->GetCallsign() << " at " << (*pit)->GetIp() << " removed" << std::endl;
				delete *pit;
				m_Peers.erase(pit);
				g_Reflector.OnPeersChanged();	// notify
				return;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// find peers

CPeer *CPeers::FindPeer(const CIp &Ip, int Protocol)
{
	for (auto it=m_Peers.begin(); it!=m_Peers.end(); it++) {
		if ((*it)->GetIp()==Ip && (*it)->GetProtocol()==Protocol)
			return *it;
	}

	return NULL;
}

CPeer *CPeers::FindPeer(const CCallsign &Callsign, const CIp &Ip, int Protocol)
{
	for (auto it=m_Peers.begin(); it!=m_Peers.end(); it++) {
		if ((*it)->GetCallsign().HasSameCallsign(Callsign) && (*it)->GetIp()==Ip && (*it)->GetProtocol()==Protocol)
			return *it;
	}

	return NULL;
}

CPeer *CPeers::FindPeer(const CCallsign &Callsign, int Protocol)
{
	for (auto it=m_Peers.begin(); it!=m_Peers.end(); it++) {
		if ((*it)->GetProtocol()==Protocol && (*it)->GetCallsign().HasSameCallsign(Callsign))
			return *it;
	}

	return NULL;
}


////////////////////////////////////////////////////////////////////////////////////////
// iterate on peers

CPeer *CPeers::FindNextPeer(int Protocol, std::list<CPeer *>::iterator &it)
{
	while (it != m_Peers.end()) {
		if ((*it)->GetProtocol() == Protocol)
			return *it++;
		it++;
	}
	return NULL;
}

