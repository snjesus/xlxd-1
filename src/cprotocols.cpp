//
//  cprotocols.cpp
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 01/11/2015.
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

#include "main.h"
#include "cdextraprotocol.h"
#include "cdplusprotocol.h"
#include "cdcsprotocol.h"
#include "cxlxprotocol.h"
#include "cdmrplusprotocol.h"
#include "cdmrmmdvmprotocol.h"
#include "cprotocols.h"


////////////////////////////////////////////////////////////////////////////////////////
// constructor

CProtocols::CProtocols()
{
	for ( unsigned int i = 0; i < m_Protocol.size(); i++ ) {
		m_Protocol[i] = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// destructor

CProtocols::~CProtocols()
{
	m_Mutex.lock();
	{
		for ( unsigned int i = 0; i < m_Protocol.size(); i++ ) {
			delete m_Protocol[i];
		}
	}
	m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////////////
// initialization

bool CProtocols::Init(void)
{
	bool ok = true;

	m_Mutex.lock();
	{
		// create and initialize DEXTRA
		delete m_Protocol[0];
		m_Protocol[0] = new CDextraProtocol;
		ok &= m_Protocol[0]->Init();

		// create and initialize DPLUS
		delete m_Protocol[1];
		m_Protocol[1] = new CDplusProtocol;
		ok &= m_Protocol[1]->Init();

		// create and initialize DCS
		delete m_Protocol[2];
		m_Protocol[2] = new CDcsProtocol;
		ok &= m_Protocol[2]->Init();

		// create and initialize XLX - interlink
		delete m_Protocol[3];
		m_Protocol[3] = new CXlxProtocol;
		ok &= m_Protocol[3]->Init();

		// create and initialize DMRPLUS
		delete m_Protocol[4];
		m_Protocol[4] = new CDmrplusProtocol;
		ok &= m_Protocol[4]->Init();

		// create and initialize DMRMMDVM
		delete m_Protocol[5];
		m_Protocol[5] = new CDmrmmdvmProtocol;
		ok &= m_Protocol[5]->Init();
	}
	m_Mutex.unlock();

	// done
	return ok;
}

void CProtocols::Close(void)
{
	m_Mutex.lock();
	{
		for ( unsigned int i = 0; i < m_Protocol.size(); i++ ) {
			m_Protocol[i]->Close();
		}
	}
	m_Mutex.unlock();
}
