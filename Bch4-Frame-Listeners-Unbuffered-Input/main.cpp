#include "Bch4.h"

BCH4::BCH4(void)
{
}

BCH4::~BCH4(void)
{
}

int main(int argc, char **argv)
{
	BCH4 app;
	try {
		app.go();
	}catch(Ogre::Exception e) {
		std::cerr << "An exception has occured:" << e.getDescription().c_str() << std::endl;
	}
	return 0;
}
