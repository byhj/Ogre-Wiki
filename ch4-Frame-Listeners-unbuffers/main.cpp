#include "ch4.h"

CH4::CH4(void)
{
}

CH4::~CH4(void)
{
}

int main(int argc, char **argv)
{
	CH4 app;
	try {
		app.go();
	}catch(Ogre::Exception e) {
		std::cerr << "An exception has occured:" << e.getDescription().c_str() << std::endl;
	}
	return 0;
}
