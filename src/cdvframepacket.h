#pragma once

//
//  cdvframepacket.h
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

#include "cpacket.h"

////////////////////////////////////////////////////////////////////////////////////////
// defines

#define AMBE_SIZE       9
#define DVDATA_SIZE     3

#ifdef IS_XLX
#define AMBEPLUS_SIZE   9
#define DVSYNC_SIZE     7
#endif

// typedef & structures

struct __attribute__ ((__packed__))dstar_dvframe
{
	uint8	AMBE[AMBE_SIZE];
	uint8	DVDATA[DVDATA_SIZE];
};

////////////////////////////////////////////////////////////////////////////////////////
// class

class CDvFramePacket : public CPacket
{
	friend class CCodecStream;
public:
	// constructor
	CDvFramePacket();
	CDvFramePacket(const struct dstar_dvframe *, uint16, uint8);
#ifdef IS_XLX
	CDvFramePacket(const uint8 *, const uint8 *, uint16, uint8, uint8);
	CDvFramePacket(uint16, uint8, const uint8 *, const uint8 *, uint8, uint8, const uint8 *, const uint8 *);
#endif
	CDvFramePacket(const CDvFramePacket &);

	// destructor
	virtual ~CDvFramePacket() {}

	// virtual duplication
	CPacket *Duplicate(void) const;

	// identity
	bool IsDvFrame(void) const              { return true; }
	bool HaveTranscodableAmbe(void) const   { return true; }

	// get
	const uint8 *GetAmbe(void) const        { return m_uiAmbe; }
	const uint8 *GetDvData(void) const      { return m_uiDvData; }
#ifdef IS_XLX
	const uint8 *GetAmbe(uint8) const;
	const uint8 *GetAmbePlus(void) const    { return m_uiAmbePlus; }
	const uint8 *GetDvSync(void) const      { return m_uiDvSync; }
#endif

	// set
	void SetDvData(uint8 *);
#ifdef IS_XLX
	void SetAmbe(uint8, uint8 *);
#else
	void SetAmbe(uint8 *);
#endif

	// operators
	bool operator ==(const CDvFramePacket &) const;

protected:
	// get
	uint8 *GetAmbeData(void)                { return m_uiAmbe; }
#ifdef IS_XLX
	uint8 *GetAmbePlusData(void)            { return m_uiAmbePlus; }
#endif

protected:
	// data (dstar)
	uint8       m_uiAmbe[AMBE_SIZE];
	uint8       m_uiDvData[DVDATA_SIZE];
#ifdef IS_XLX
	// data (dmr)
	uint8       m_uiAmbePlus[AMBEPLUS_SIZE];
	uint8       m_uiDvSync[DVSYNC_SIZE];
#endif
};


////////////////////////////////////////////////////////////////////////////////////////
