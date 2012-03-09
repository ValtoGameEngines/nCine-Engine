#ifndef CLASS_MYEVENTHANDLER
#define CLASS_MYEVENTHANDLER

#include "ncIAppEventHandler.h"
#include "ncIInputEventHandler.h"
#include "ncVector2f.h"
class ncTexture;
class ncParticleSystem;
class ncTimer;

/// My nCine application
class MyEventHandler
	: public ncIAppEventHandler,
	  public ncIInputEventHandler
{
private:
	ncTexture *m_pTexture;
	ncParticleSystem *m_pParticleSys;
	ncTimer *m_pTimer;
	unsigned long int m_ulEmitTime;
	ncVector2f m_emitVector;

public:
	virtual void OnInit();
	virtual void OnFrameStart();
	virtual void OnFrameEnd();
	virtual void OnShutdown();

	virtual void OnKeyPressed(const ncKeyboardEvent &event) { }
#ifdef __ANDROID__
	virtual void OnKeyReleased(const ncKeyboardEvent &event) { }
	virtual void OnTouchDown(const ncTouchEvent &event);
	virtual void OnTouchUp(const ncTouchEvent &event) { }
	virtual void OnTouchMove(const ncTouchEvent &event);
	virtual void OnSecondaryTouchDown(const ncTouchEvent &event) { }
	virtual void OnSecondaryTouchUp(const ncTouchEvent &event) { }
#else
	virtual void OnKeyReleased(const ncKeyboardEvent &event);
	virtual void OnMouseButtonPressed(const ncMouseEvent &event);
	virtual void OnMouseButtonReleased(const ncMouseEvent &event) { }
	virtual void OnMouseMoved(const ncMouseState &state);
#endif
};

#endif