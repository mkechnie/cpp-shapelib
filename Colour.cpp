// Colour.cpp: implementation of the CColour class.
//
//////////////////////////////////////////////////////////////////////

#include "Colour.h"

std::ostream &operator << (std::ostream &os, const CColour &colour)
{
	os << colour.Str();
	return os;
}



std::istream &operator >> (std::istream &is, CColour &colour)
{
	char c;
	unsigned int red;
	unsigned int green;
	unsigned int blue;

	is >> c >> red >> c >> green >> c >> blue >> c;
	colour.m_red = red;
	colour.m_green = green;
	colour.m_blue = blue;

	return is;
}