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
#include "OgreRoot.h"

#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && !defined(__MINGW32__) && !defined(OGRE_STATIC_LIB)
#	ifdef OGRE_STEREOPLUGIN_EXPORTS
#		define _OgreStereoPluginExport __declspec(dllexport)
#	else
#       if defined( __MINGW32__ )
#           define _OgreStereoPluginExport
#       else
#    		define _OgreStereoPluginExport __declspec(dllimport)
#       endif
#   endif
#elif defined ( OGRE_GCC_VISIBILITY )
#   define _OgreStereoPluginExport  __attribute__ ((visibility("default")))
#else
#	define _OgreStereoPluginExport
#endif	// OGRE_WIN32

namespace Ogre {
    StereoPlugin *gStereoPlugin;
    
    //-----------------------------------------------------------------------
    extern "C" void _OgreStereoPluginExport dllStartPlugin(void)
    {
		// Create new plugin
		gStereoPlugin = OGRE_NEW StereoPlugin();

		// Register
		Root::getSingleton().installPlugin(gStereoPlugin);
	}

	extern "C" void _OgreStereoPluginExport dllStopPlugin(void)
	{
		Root::getSingleton().uninstallPlugin(gStereoPlugin);

		OGRE_DELETE gStereoPlugin;
	}
}
