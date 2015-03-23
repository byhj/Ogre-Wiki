#include "ch6.h"
#include <OgreException.h>

int main(int argc, char **argv)
{
	CH6 app;
	try {
		app.go();
	}catch(Ogre::Exception &e) {
		std::cerr << "An exception has occured:"
			<< e.getFullDescription().c_str() << std::endl;
	}
	return 0;
}