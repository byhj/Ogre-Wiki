#include "Bch1.h"

int main(int argc, char **argv)
{
	BCH1 app;
	try {
		app.go();
	}catch(Ogre::Exception &e) {
		std::cout << "Ogre Exception Occured: " << e.getFullDescription().c_str() << std::endl;
	    return -1;
	}
	return 0;
}