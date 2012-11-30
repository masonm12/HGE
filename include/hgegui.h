/*
** Haaf's Game Engine 1.5
** Copyright (C) 2003-2004, Relish Games
** hge.relishgames.com
**
** hgeGUI helper classes header
*/


#ifndef HGEGUI_H
#define HGEGUI_H


#include "hge.h"
#include "hgesprite.h"
#include "hgerect.h"


#define HGEGUI_NONAVKEYS		0
#define HGEGUI_LEFTRIGHT		1
#define HGEGUI_UPDOWN			2
#define HGEGUI_CYCLED			4

class hgeGUI;

/*
** hgeGUIObject
*/
class hgeGUIObject
{
public:
	hgeGUIObject()	{ hge=hgeCreate(HGE_VERSION); }
	virtual			~hgeGUIObject() { hge->Release(); }

	virtual void	Render() = 0;
	virtual void	Update(float dt) {}

	virtual void	Enter() {}
	virtual void	Leave() {}
	virtual bool	IsDone() { return true; }
	virtual void	Focus(bool bFocused) {}
	virtual void	MouseOver(bool bOver) {}

	virtual bool	MouseMove(float x, float y) { return false; }
	virtual bool	MouseLButton(bool bDown) { return false; }
	virtual bool	MouseRButton(bool bDown) { return false; }
	virtual bool	MouseWheel(int nNotches) { return false; }
	virtual bool	KeyClick(int key, int chr) { return false; }

	int				id;
	bool			bStatic;
	bool			bVisible;
	bool			bEnabled;
	hgeRect			rect;

	hgeGUI			*gui;
	hgeGUIObject	*next;
	hgeGUIObject	*prev;

protected:
	hgeGUIObject(const hgeGUIObject &go);
	hgeGUIObject&	operator= (const hgeGUIObject &go);

	static HGE		*hge;
};


/*
** hgeGUI
*/
class hgeGUI
{
public:
	hgeGUI();
	~hgeGUI();

	void			AddCtrl(hgeGUIObject *ctrl);
	void			DelCtrl(int id);
	hgeGUIObject*	GetCtrl(int id) const;

	void			MoveCtrl(int id, float x, float y);
	void			ShowCtrl(int id, bool bVisible);
	void			EnableCtrl(int id, bool bEnabled);

	void			SetNavMode(int mode);
	void			SetCursor(hgeSprite *spr);
	void			SetFocus(int id);
	int				GetFocus() const;
	
	void			Enter();
	void			Leave();

	int				Update(float dt);
	void			Render();

private:
	hgeGUI(const hgeGUI &);
	hgeGUI&			operator= (const hgeGUI&);
	bool			ProcessCtrl(hgeGUIObject *ctrl);

	static HGE		*hge;

	hgeGUIObject	*ctrls;
	hgeGUIObject	*ctrlLock;
	hgeGUIObject	*ctrlFocus;
	hgeGUIObject	*ctrlOver;

	int				navmode;
	int				nEnterLeave;
	hgeSprite		*sprCursor;

	float			mx,my;
	int				nWheel;
	bool			bLPressed, bLLastPressed;
	bool			bRPressed, bRLastPressed;
};


#endif
