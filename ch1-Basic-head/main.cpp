#include "ch1.h"
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	CH1 app;
	try {
		app.go(); //运行实例
	}catch(Ogre::Exception &e) { //遇到异常，捕获抛出
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
	}
	return 0;
}
