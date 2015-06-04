#ifndef SIMPLEOGRE_H
#define SIMPLEOGRE_H

#include <memory>

//forward declaration
namespace Ogre{
	class RenderWindow;
	class Root;
}

namespace byhj
{
	class SimpleOgre 
	{
	private: 
		SimpleOgre(const SimpleOgre &s);
		SimpleOgre & operator = (const SimpleOgre &s);
	public:
		SimpleOgre();
		~SimpleOgre();
		bool initOgre();
	public:
		std::auto_ptr<Ogre::Root> mRoot;   //scene control
		Ogre::RenderWindow *mWindow;
	};
}

#endif