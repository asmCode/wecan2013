#include "Color.h"

const Color Color::White	(255, 255, 255, 255);
const Color Color::Black	(  0,   0,   0, 255);
const Color Color::Red		(255,   0,   0, 255);
const Color Color::Green	(  0, 255,   0, 255);
const Color Color::Blue		(  0,   0, 255, 255);

Color::Color()
{
	this ->R = 0;
	this ->G = 0;
	this ->B = 0;
	this ->A = 255;
}

Color::Color(unsigned char R, unsigned char G, unsigned char B)
{
	this ->R = R;
	this ->G = G;
	this ->B = B;
	this ->A = 255;
}

Color::Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
	this ->R = R;
	this ->G = G;
	this ->B = B;
	this ->A = A;
}

bool Color::operator == (const Color &color) const
{
	return
		R == color.R &&
		G == color.G &&
		B == color.B &&
		A == color.A;
}