#include "Basic4.h"

Basic4::Basic4(void)
{
}

Basic4::~Basic4(void)
{
}

int main(int argc, char **argv)
{
	Basic4 app;
	try {
		app.go();
	}catch(Ogre::Exception e) {
		std::cerr << "An exception has occured:" << e.getDescription().c_str() << std::endl;
	}
	return 0;
}
