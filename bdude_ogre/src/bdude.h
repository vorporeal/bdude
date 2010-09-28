/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include <Ogre.h>

#include "BaseApplication.h"

class Map;

class bdude : public BaseApplication
{
public:
    bdude(void);
    virtual ~bdude(void);

protected:
    virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	Ogre::Viewport *m_viewport;

	Map *m_map;
};

#endif // #ifndef __TutorialApplication_h_
