#ifndef CH1_H
#define CH1_H
#include "BaseApplication.h"

class CH1 :public BaseApplication {
public:
	CH1(void);  //构造与析构函数
	~CH1(void);
protected:
	void createScene(void); //创建场景
};
#endif