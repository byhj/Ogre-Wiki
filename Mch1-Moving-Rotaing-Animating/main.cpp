#include "Mch1.h"

#ifdef __cplusplus
extern "C" {
#endif
	int main(int argc, char **argv)
	{
		Mch1 app;
		try {
			app.go();
		} catch(Ogre::Exception &e) {
			std::cerr << "An exception has occured:" << e.getFullDescription().c_str() << std::endl;
		}
		return 0;
	}
#ifdef __cplusplus
}
#endif
