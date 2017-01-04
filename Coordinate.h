// Coordinate.h: interface for the CCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(COORDINATE_H)
#define COORDINATE_H

#include <iostream>
#include <sstream>
#include "ioXml.h"
#include "Xml.h"

class CCoordinate  : public IioXml
{
public:
	
	CCoordinate::CCoordinate() 
		: m_x(0), m_y(0), m_z(0)
	{ }

	CCoordinate::CCoordinate(unsigned int x, unsigned int y, unsigned int z=0)
		: m_x(x), m_y(y), m_z(z)
	{ }
	virtual ~CCoordinate()
	{ }

	CCoordinate(const CCoordinate &old)
	{
		this->m_x = old.m_x;
		this->m_y = old.m_y;
		this->m_z = old.m_z;
	}
		
	void Draw();

	unsigned int X() const {return m_x;}
	unsigned int Y() const {return m_y;}
	unsigned int Z() const {return m_z;}
	void SetX(const unsigned int x) {m_x = x;}
	void SetY(const unsigned int y) {m_y = y;}
	void SetZ(const unsigned int z) {m_z = z;}

	CCoordinate &operator = (const CCoordinate &rhs)
	{
		if (*this != rhs)
		{
			this->m_x = rhs.m_x;
			this->m_y = rhs.m_y;
			this->m_z = rhs.m_z;
		}
		return *this;
	}

	CCoordinate &operator = (const std::string &rhs)
	{
		char c;
		std::istringstream i_str(rhs);
		i_str >> c >> m_x >> c >> m_y >> c >> m_z;
		return *this;
	}

	CCoordinate operator + (const CCoordinate &rhs) const
	{
		CCoordinate ret_val;

		ret_val.m_x = this->m_x + rhs.m_x;
		ret_val.m_y = this->m_y + rhs.m_y;
		ret_val.m_z = this->m_z + rhs.m_z;
		return ret_val;
	}

	CCoordinate operator - (const CCoordinate &rhs) const
	{
		CCoordinate ret_val;

		ret_val.m_x = this->m_x - rhs.m_x;
		ret_val.m_y = this->m_y - rhs.m_y;
		ret_val.m_z = this->m_z - rhs.m_z;
		return ret_val;
	}

	CCoordinate operator * (const CCoordinate &rhs) const
	{
		CCoordinate ret_val;

		ret_val.m_x = this->m_x * rhs.m_x;
		ret_val.m_y = this->m_y * rhs.m_y;
		ret_val.m_z = this->m_z * rhs.m_z;
		return ret_val;
	}

	bool operator == (const CCoordinate &rhs)
	{ 
		if ((this->m_x == rhs.m_x) && (this->m_y == rhs.m_y) && (this->m_z == rhs.m_z))
			return true;
		else
			return false;
	}

	bool operator != (const CCoordinate &rhs)
	{ 
		if ((this->m_x != rhs.m_x) || (this->m_y != rhs.m_y) || (this->m_z != rhs.m_z))
			return true;
		else
			return false;
	}

	bool WriteXml(CXml &element)
	{
		if (element.GetTag().empty())
			element.SetTag("COORDINATE");
		element.AddAttribute("X", m_x);
		element.AddAttribute("Y", m_y);
		element.AddAttribute("Z", m_z);

		return true;
	}

	std::string Str()
	{
		std::ostringstream o_str;
		o_str << "(" << m_x << ", " << m_y << ", " << m_z << ")";
		return o_str.str();
	}

	static IioXml *Create(CXml &element)
	{
		unsigned int	x;
		unsigned int	y;
		unsigned int	z;

		element.GetAttribute("X", &x);
		element.GetAttribute("Y", &y);
		element.GetAttribute("Z", &z);

		return  new CCoordinate(x, y, z);
	}
private:
	unsigned int	m_x;
	unsigned int	m_y;
	unsigned int	m_z;

	friend std::ostream &operator << (std::ostream &os, CCoordinate &coord);
	friend std::istream &operator >> (std::istream &is, CCoordinate &coord);
};

#endif // !defined(COORDINATE_H)
