################################################
# For installation of the StereoManager Plugin #
################################################

1) Run CMake and install the Plugin (Alternatively you can add the StereoManager as an external project in your CMakeLists.txt (See code below)
2) Create a link in ${Ogre Installation folder}/lib/Ogre to Plugin_StereoManager.so.1.10.0 with name Plugin_StereoManager.so
3) Add 
	Plugin=Plugin_StereoManager
   into the plugin.cfg and enable Plugin_CgProgramManager


####################
# External Project #
####################

include(ExternalProject)
ExternalProject_Add(StereoManager
	GIT_REPOSITORY https://baudenri@bitbucket.org/baudenri/ogrestereoplugin.git
	CMAKE_CACHE_ARGS 
		-DOGRE_INSTALL_PATH:string=${OGRE_INSTALL_PATH}
)
