#include "Basic2.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

//If define cplusplus, Use C code in Cpp
#ifdef __cplusplus
extern "C"
{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
#else
	int main(int argc, char *argv[])
#endif
   {
	   Basic2 app;
	   try
	   {
		   app.go();
	   }
	   catch(Ogre::Exception &e) 
	   {
		   //Output the Exception information
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		 MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		 std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
#endif
	   }
	   return 0;
   }
#ifdef __cplusplus
}
#endif