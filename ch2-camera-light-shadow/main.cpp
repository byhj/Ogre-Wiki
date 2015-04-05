#include "ch2.h"

int main(int argc, char **argv)
{
	CH2 app;
	try {
		app.go();
	}catch(Ogre::Exception e) {
		std::cerr << "An exception has occured:" << e.getDescription().c_str() << std::endl;
	}
	return 0;
}
