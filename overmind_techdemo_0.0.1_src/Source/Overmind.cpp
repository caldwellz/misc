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

//Temporary main for testing the engine

#include <iostream>
using namespace std;

#include "Engine/OvermindEngine.hpp"
using namespace OvermindEngine;

int main() {
	cout << "[Program started - techdemo " << VERSION_STRING << "]" << endl;
	cout << "[Running Init...]" << endl;
	OvermindEngine::init();

	cout << "[Changing video mode...]" << endl;
	RenderWindowConfig newrenderconf;
	newrenderconf.Name = "Overmind"; newrenderconf.Width = 1024; newrenderconf.Height = 768;
	OgreManager::getSingleton().setRenderWindowConfig(newrenderconf);

	cout << "[Loading GUI...]" << endl;
	//Add resource groups
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("GUI/Fonts");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/GUI/Fonts", "FileSystem", "GUI/Fonts", false);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("GUI/Imagesets");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/GUI/Imagesets", "FileSystem", "GUI/Imagesets", false);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("GUI/Layouts");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/GUI/Layouts", "FileSystem", "GUI/Layouts", false);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("GUI/LookNFeel");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/GUI/LookNFeel", "FileSystem", "GUI/LookNFeel", false);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("GUI/Schemes");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/GUI/Schemes", "FileSystem", "GUI/Schemes", false);
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("GUI/Scripts");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/GUI/Scripts", "FileSystem", "GUI/Scripts", false);
	//Set up the scheme
	CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );
	CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );
	CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );
	//Set up the layout
	CEGUI::System::getSingleton().setGUISheet(CEGUI::WindowManager::getSingleton().loadWindowLayout("Demo8.layout"));
	LuaManager::getSingletonPtr()->mScriptModule->executeScriptFile("Demo8.lua", "GUI/Scripts");

	cout << "[Configuring audio...]" << endl;
	//Set a new config
	AudioConfig newaudioconf;
	newaudioconf.mMaxSources = 95;
	OgreAudioManager::getSingleton().setAudioConfig(newaudioconf);
	//Add resource group
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Audio");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Data/Audio", "FileSystem", "Audio", false);
	//Attach listener to a scene node, and move
	Ogre::SceneNode* listenerNode = OgreManager::getSingleton().mRoot->getSceneManager("mRootScene")->getRootSceneNode()->createChildSceneNode("Listener");
	listenerNode->attachObject(OgreAudioManager::getSingleton().mSoundManager->getListener());
	listenerNode->setPosition(500, 1, 1);
	//Add a sound, attach to a scene node, and play
	OgreOggISound* clock = OgreAudioManager::getSingleton().mSoundManager->createSound("clock", "clock.ogg", false, true, true);
	Ogre::SceneNode* clockNode = OgreManager::getSingleton().mRoot->getSceneManager("mRootScene")->getRootSceneNode()->createChildSceneNode("Clock");
	clockNode->attachObject(clock);
    clock->setReferenceDistance(64.f);
	clock->play();

	cout << "[Doing Run loop...]" << endl;
	float i = 2.f, n = 0.5;
	while(overmindReady && !OISManager::getSingleton().getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		//Change clock sound position
		if((i == 1000.f) || (i == 1.f)) n *= -1;
		clockNode->translate(n, 0, 0);
		i += n;

		OvermindEngine::run();
	}

	cout << "[Running Deinit...]" << endl;
	OvermindEngine::deinit();

	cout << "[Program reached end]" << endl;
	return 0;
}
