#include "Mch5.h"

int main(int argc, char **argv)
{
	Mch5 app;
    try {
		app.go();
	}catch(Ogre::Exception &e) {
		std::cerr << "An exception has occured:"<< 
			       e.getFullDescription().c_str() << std::endl;
	}
	return 0;
}