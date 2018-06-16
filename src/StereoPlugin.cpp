/*
-----------------------------------------------------------------------------
This source is a Stereoscopy manager for OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
-----------------------------------------------------------------------------
* Copyright (c) 2008, Mathieu Le Ber, AXYZ-IMAGES
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AXYZ-IMAGES nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mathieu Le Ber ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mathieu Le Ber BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
*/

#include "StereoPlugin.h"

using namespace Ogre;

StereoPlugin::StereoPlugin()
{
}
StereoPlugin::~StereoPlugin()
{
}

const String& StereoPlugin::getName() const
{
	static String name("StereoManager");
	return name;
}

void StereoPlugin::install()
{
	// init enable plug-in option
	mOptEnableStereo.name = "Enable stereo rendering";
	mOptEnableStereo.currentValue.clear();
	mOptEnableStereo.possibleValues.clear();
	mOptEnableStereo.immutable = false;
	mOptEnableStereo.possibleValues.push_back( "Yes" );
	mOptEnableStereo.possibleValues.push_back( "No" );
	mOptEnableStereo.currentValue = "Yes";

	//const RenderSystemList& renderSystemList = *Root::getSingleton().getAvailableRenderers(); // for Ogre 1.6
	const RenderSystemList& renderSystemList = Root::getSingleton().getAvailableRenderers(); // for Ogre 1.7
	for (size_t i = 0 ; i < renderSystemList.size() ; i++)
	{
		RenderSystem * renderSystem = renderSystemList[i];
		ConfigOptionMap& configOptionMap = renderSystem->getConfigOptions();
		configOptionMap[mOptEnableStereo.name] = mOptEnableStereo;
	}
}

void StereoPlugin::initialise()
{
	// check if stereo must be enabled
	ConfigOptionMap& configOptionMap = Root::getSingleton().getRenderSystem()->getConfigOptions();
	if (configOptionMap[mOptEnableStereo.name].currentValue == "No")
	{
		return;
	}

	// the stereo manager needs a camera and a viewport but the have not been created yet in the application
	// create a frame manager to delay the initialisation to the first frame
	Root::getSingleton().addFrameListener(this);
}

void StereoPlugin::shutdown()
{
	ConfigOptionMap& configOptionMap = Root::getSingleton().getRenderSystem()->getConfigOptions();
	if (configOptionMap[mOptEnableStereo.name].currentValue == "No")
	{
		return;
	}

	mStereoManager.shutdown();
}

void StereoPlugin::uninstall()
{

}

bool StereoPlugin::frameStarted(const FrameEvent& evt)
{
	// initialize the StereoManager at the first frame when the application is initialised

	RenderWindow *window = Root::getSingleton().getAutoCreatedWindow();
	if(window)
	{
		Viewport *viewport = window->getViewport(0);
		if(viewport)
		{
			std::list<RenderTarget*> renderTextureList;
			RenderSystem::RenderTargetIterator it = Root::getSingleton().getRenderSystem()->getRenderTargetIterator();
			while(it.hasMoreElements())
			{
				RenderTarget *rt = it.getNext();
				// store all the auto updated render textures that depend on the main camera in a list
				// (in fact any rendertarget execpt the main window since I did not found a way to tell if a rendertarget is a rendertexture or not)
				if(!rt->isPrimary() && rt->isAutoUpdated() && rt != window)
				{
					bool viewDependent = false;
					for(int i = 0; i < rt->getNumViewports(); i++)
					{
						if(rt->getViewport(i)->getCamera() == viewport->getCamera())
							viewDependent = true;
					}
					if(viewDependent)
						renderTextureList.push_back(rt);
				}
			}



			try
			{
				StereoManager::StereoMode mode = mStereoManager.loadConfig("stereo.cfg");
				RenderWindow *window2 = NULL;
				Viewport *viewport2 = NULL;

				if(mode ==StereoManager::SM_DUALOUTPUT)
				{
					NameValuePairList miscParams;
					miscParams["monitorIndex"] = "1";

					window2 = Root::getSingleton().createRenderWindow("StereoPlugin right window", 
						window->getWidth(), window->getHeight(), window->isFullScreen(), &miscParams);
					viewport2 = window2->addViewport(viewport->getCamera());

					// When there are two windows, only the first one has the focus. Ogre Disables the rendering
					// on the unfocused window by default.
					// these lines will tell the two windows to render event when unfocused
					window->setDeactivateOnFocusChange(false);
					window2->setDeactivateOnFocusChange(false);

				}
				// try to load the config file
				mStereoManager.init(viewport, viewport2, mode);
			}
			catch(const FileNotFoundException& e)
			{
				// init the manager with defaults parameters
				mStereoManager.init(viewport, NULL, StereoManager::SM_ANAGLYPH_RC);
				// and generate a default config file
				mStereoManager.saveConfig("stereo.cfg");
			}		

			while(!renderTextureList.empty())
			{
				// tell the stereo manager to render these rendertextures one time for each eye because
				// their contents depend on the main camera position
				// (for example the rendertexture used for water reflexions)
				mStereoManager.addRenderTargetDependency(renderTextureList.front());
				renderTextureList.pop_front();
			}
		}
	}
	// only need to run this once, remove the listener
	Root::getSingleton().removeFrameListener(this);

	return true;
}
