For installation of the StereoManager Plugin
--------------------------------------------

1. Run CMake and install the Plugin (Alternatively you can add the StereoManager as an external project in your CMakeLists.txt (See code below)
2. Add 
   `Plugin=Plugin_StereoManager`
   into the plugin.cfg. Enable Plugin_CgProgramManager for anaglyph and/ or interlaced rendering.

External Project 
----------------
```
include(ExternalProject)
ExternalProject_Add(StereoManager
	GIT_REPOSITORY https://bitbucket.org/baudenri/ogrestereoplugin.git
	CMAKE_ARGS 
		-DOGRE_INSTALL_PATH=${OGRE_INSTALL_PATH}
)
```
