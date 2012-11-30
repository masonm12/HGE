/*
** Haaf's Game Engine 1.5
** Copyright (C) 2003-2004, Relish Games
** hge.relishgames.com
**
** hgeColor helper class
*/


#ifndef HGECOLOR_H
#define HGECOLOR_H


#include "hge.h"


inline void ColorClamp(float &x) { if(x<0.0f) x=0.0f; if(x>1.0f) x=1.0f; }

class hgeColor
{
public:
	float		r,g,b,a;

	hgeColor(float _r, float _g, float _b, float _a) { r=_r; g=_g; b=_b; a=_a; }
	hgeColor(DWORD col) { SetHWColor(col); }
	hgeColor() { r=g=b=a=0; }

	hgeColor	operator- (const hgeColor &c) { return hgeColor(r-c.r, g-c.g, b-c.b, a-c.a); }
	hgeColor	operator+ (const hgeColor &c) { return hgeColor(r+c.r, g+c.g, b+c.b, a+c.a); }
	hgeColor	operator* (float scalar) { return hgeColor(r*scalar, g*scalar, b*scalar, a*scalar); }
	hgeColor&	operator-= (const hgeColor &c) { r-=c.r; g-=c.g; b-=c.b; a-=c.a; return *this; }
	hgeColor&	operator+= (const hgeColor &c) { r+=c.r; g+=c.g; b+=c.b; a+=c.a; return *this; }
	hgeColor&	operator*= (float scalar) { r*=scalar; g*=scalar; b*=scalar; a*=scalar; return *this; }
	bool		operator== (const hgeColor &c) { return (r==c.r && g==c.g && b==c.b && a==c.a); }
	bool		operator!= (const hgeColor &c) { return (r!=c.r || g!=c.g || b!=c.b || a!=c.a); }

	void		Clamp() { ColorClamp(r); ColorClamp(g); ColorClamp(b); ColorClamp(a); }
	void		SetHWColor(DWORD col) { a=(col>>24)/255.0f; r=((col>>16) & 0xFF)/255.0f; g=((col>>8) & 0xFF)/255.0f; b=(col & 0xFF)/255.0f; }
	DWORD		GetHWColor() const { return (DWORD(a*255.0f)<<24) + (DWORD(r*255.0f)<<16) + (DWORD(g*255.0f)<<8) + DWORD(b*255.0f); }
};

inline hgeColor operator* (const hgeColor &c, float s) { return hgeColor(s*c.r, s*c.g, s*c.b, s*c.a); }
inline hgeColor operator* (float s, const hgeColor &c) { return hgeColor(s*c.r, s*c.g, s*c.b, s*c.a); }

#endif
