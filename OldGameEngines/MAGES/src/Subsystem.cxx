/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include "MAGES/Subsystem.hxx"
using namespace MAGES;

Subsystem::Subsystem() {
	_core = NULL;
	_db = NULL;
	_coreReady = false;
}


bool Subsystem::initialize() {
	_commands.resize(0);

	return true;
}

/*
const std::vector<Id>& Subsystem::getCommands() {
    return const& _commands;
}
*/

void Subsystem::dispatchCommand(Id command, String property) {
	_m.lock();
	_cmdQueue.push(std::pair<Id, String>(command, property));
	_m.unlock();
}


void Subsystem::run() {
	while(update());
}


void Subsystem::destroy() {
	return;
}
