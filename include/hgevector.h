/*
** Haaf's Game Engine 1.5
** Copyright (C) 2003-2004, Relish Games
** hge.relishgames.com
**
** hgeVector helper class
*/


#ifndef HGEVECTOR_H
#define HGEVECTOR_H


#include "hge.h"
#include <math.h>


/*
** Fast 1.0/sqrtf(float) routine
*/
float InvSqrt(float x);

class hgeVector
{
public:
	float	x,y;

	hgeVector(float _x, float _y)	{ x=_x; y=_y; }
	hgeVector()						{ x=0; y=0; }

	hgeVector	operator- (const hgeVector &v) { return hgeVector(x-v.x, y-v.y); }
	hgeVector	operator+ (const hgeVector &v) { return hgeVector(x+v.x, y+v.y); }
	hgeVector	operator* (float scalar) { return hgeVector(x*scalar, y*scalar); }
	hgeVector&	operator-= (const hgeVector &v) { x-=v.x; y-=v.y; return *this; }
	hgeVector&	operator+= (const hgeVector &v) { x+=v.x; y+=v.y; return *this; }
	hgeVector&	operator*= (float scalar) { x*=scalar; y*=scalar; return *this; }
	hgeVector	operator- () { return hgeVector(-x, -y); }
	bool		operator== (const hgeVector &v) { return (x==v.x && y==v.y); }
	bool		operator!= (const hgeVector &v) { return (x!=v.x || y!=v.y); }

	float		Dot(const hgeVector *v) const { return x*v->x + y*v->y; }
	hgeVector*	Normalize() { float rc=InvSqrt(Dot(this)); x*=rc; y*=rc; return this; }
	float		Length() const { return sqrtf(Dot(this)); }
	float		Angle(const hgeVector *v = 0) const;
	hgeVector*	Rotate(float a);
};

inline hgeVector operator* (const hgeVector &v, float s) { return hgeVector(s*v.x, s*v.y); }
inline hgeVector operator* (float s, const hgeVector &v) { return hgeVector(s*v.x, s*v.y); }
inline float operator^ (const hgeVector &v, const hgeVector &u) { return v.Angle(&u); }
inline float operator% (const hgeVector &v, const hgeVector &u) { return v.Dot(&u); }


#endif
