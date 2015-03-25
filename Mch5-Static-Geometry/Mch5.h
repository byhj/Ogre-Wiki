#ifndef MCH5_H
#define MCH5_H

#include "BaseApplication.h"

class Mch5 : public BaseApplication
{
public:
	Mch5(void);
	virtual ~Mch5(void);
protected:
	virtual void createScene(void);
	virtual void createGrassMesh(void);

};
#endif // !MCH5_H
