// Coordinate.cpp: implementation of the CCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#include "Coordinate.h"


void CCoordinate::Draw()
{

}

std::ostream &operator << (std::ostream &os, CCoordinate &coord)
{
	os << coord.Str();
	return os;
}



std::istream &operator >> (std::istream &is, CCoordinate &coord)
{
	char c;

	is >> c >> coord.m_x >> c >> coord.m_y >> c >> coord.m_z >> c;;
	return is;
}
