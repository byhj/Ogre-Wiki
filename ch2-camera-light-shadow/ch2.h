#ifndef CH2_H
#define CH2_H

#include "BaseApplication.h"

class CH2 : public BaseApplication {
public:
	CH2();
	virtual ~CH2();
protected:
	virtual void createScene();  //�麯�������Ա�����̳���ʵ��
	virtual void createCamera();
	virtual void createViewports();
};
#endif
