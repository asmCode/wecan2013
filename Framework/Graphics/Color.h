#pragma once

class Color
{
public:
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
	
	Color();
	Color(unsigned char R, unsigned char G, unsigned char B);
	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
	
	bool operator == (const Color &color) const;
};
