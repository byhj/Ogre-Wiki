#include "ch1.h"
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	CH1 app;
	try {
		app.go(); //����ʵ��
	}catch(Ogre::Exception &e) { //�����쳣�������׳�
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
	}
	return 0;
}
