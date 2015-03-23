#ifndef CH2_H
#define CH2_H

#include "BaseApplication.h"

class CH2 : public BaseApplication {
public:
	CH2();
	virtual ~CH2();
protected:
	virtual void createScene();  //虚函数，可以被子类继承重实现
	virtual void createCamera();
	virtual void createViewports();
};
#endif
