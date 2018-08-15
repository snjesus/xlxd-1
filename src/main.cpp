//
//  main.cpp
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 31/10/2015.
//  Copyright © 2015 Jean-Luc Deltombe (LX3JL). All rights reserved.
//  Copyright © 2018 by Thomas A. Early, N7TAE
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

#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>


////////////////////////////////////////////////////////////////////////////////////////
// global objects

CReflector  g_Reflector;

////////////////////////////////////////////////////////////////////////////////////////
// function declaration

// signal catching function
static void sigCatch(int signum)
{
	/* do NOT do any serious work here */
	if ((signum == SIGTERM) || (signum == SIGINT))
		std::cout << "Signal caught, hutting down reflector..." << std::endl;
	return;
}

int main(int argc, const char * argv[])
{
	struct sigaction act;

	act.sa_handler = sigCatch;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;

	if (sigaction(SIGTERM, &act, 0) != 0) {
		std::cout << "sigaction-TERM failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGINT, &act, 0) != 0) {
		std::cout << "sigaction-INT failed" << std::endl;
		exit(EXIT_FAILURE);
	}

    // check arguments
    if ( argc != 4 )
    {
        std::cout << "Usage: xlxd callsign xlxdip ambedip" << std::endl;
        std::cout << "example: xlxd XLX999 192.168.178.212 127.0.0.1" << std::endl;
        exit(EXIT_FAILURE);
    }

    // splash
    std::cout << "Starting xlxd " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_REVISION << std::endl << std::endl;

    // initialize reflector
    g_Reflector.SetCallsign(argv[1]);
    g_Reflector.SetListenIp(CIp(argv[2]));
    g_Reflector.SetTranscoderIp(CIp(CIp(argv[3])));

    // and let it run
    if ( !g_Reflector.Start() )
    {
        std::cout << "Error starting reflector" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Reflector " << g_Reflector.GetCallsign() << "started and listening on " << g_Reflector.GetListenIp() << std::endl;

    pause(); // wait on a signal

    g_Reflector.Stop();	// clean-up!

    exit(EXIT_SUCCESS);
}
