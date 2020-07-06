/****************************************************
*  MAGES: Modular Application / Game Engine System  *
* Copyright (C) 2016 the MAGES project contributors *
*****************************************************
* This Source Code Form is subject to the terms of  *
* the Mozilla Public License, v. 2.0. If a copy of  *
* the MPL was not distributed with this file, You   *
* can obtain one at http://mozilla.org/MPL/2.0/.    *
****************************************************/

/** \file Commands.hxx
*   \brief Contains the list of 'standard' commands. */

#ifndef _MAGES_COMMANDS_HXX_
#define _MAGES_COMMANDS_HXX_

#include "Globals.hxx"

namespace MAGES {
	/// Will eventually replace the subsystem coreReady mess
	static const Id STDCMD_SHUTDOWN = 0;

	// ...

	/// Reserved for testing and capping off the range
	static const Id STDCMD_TEST = ID_MAX;
}
#endif //_MAGES_COMMANDS_HXX_
