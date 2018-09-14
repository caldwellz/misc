/*
 *	Overmind: A 3D RPG game.
 *	This file is part of Overmind.
 *
 *	Copyright (C) 2012 by authors.
 *
 * Overmind is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * Overmind is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *	along with Overmind.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OvermindEngine.hpp"
using namespace OvermindEngine;

template<> LuaManager* OvermindEngine::Singleton<LuaManager>::msSingleton = 0;

void LuaManager::hManagerStatus(const AutoPtr<nfManagerStatus>& stat) {
	if(stat->mManager == M_CEGUI) {
		if(stat->mManagerStatusIndicator == MSI_END_INITIALIZE)
			init();
		if(stat->mManagerStatusIndicator == MSI_BEGIN_DEINITIALIZE)
			deinit();
	}
}

void LuaManager::init() {
	if(!logger) logger = &logOvermind->get("LuaManager");
	nfCenter.postNotification(new nfManagerStatus(M_LUA, MSI_BEGIN_INITIALIZE));
	logger->information("LuaManager initializing...");

	mScriptModule = &CEGUI::LuaScriptModule::create();
	mScriptModule->createBindings();
	CEGUI::System::getSingleton().setScriptingModule(mScriptModule);

	mManagerReady = true;
	logger->notice("LuaManager initialized");
	nfCenter.postNotification(new nfManagerStatus(M_LUA, MSI_END_INITIALIZE));
}

void LuaManager::deinit() {
	nfCenter.postNotification(new nfManagerStatus(M_LUA, MSI_BEGIN_DEINITIALIZE));
	logger->information("LuaManager deinitializing...");

	//Apparently CEGUI's destroySystem call takes care of ScriptingModule destruction
	//CEGUI::System::getSingleton().setScriptingModule(NULL);
	//mScriptModule->destroyBindings();
	//CEGUI::LuaScriptModule::destroy(*mScriptModule);

	mManagerReady = false;
	logger->information("LuaManager deinitialized");
	nfCenter.postNotification(new nfManagerStatus(M_LUA, MSI_END_DEINITIALIZE));
}
