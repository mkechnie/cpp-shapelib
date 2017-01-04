// Colour.h: interface for the CColour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(COLOUR_H)
#define COLOUR_H

#include <iostream>
#include <sstream>
#include "ioXml.h"
#include "Xml.h"

class CColour : public IioXml  
{
public:
	CColour::CColour() 
		: m_red(0), m_green(0), m_blue(0)
	{ }

	CColour::CColour(unsigned char red, unsigned char green, unsigned char blue)
		: m_red(red), m_green(green), m_blue(blue)
	{ }
	virtual ~CColour() { }

	CColour(const CColour &old)
		: m_red(old.m_red), m_green(old.m_green), m_blue(old.m_blue)
	{ }

	CColour &operator = (const CColour &rhs)
	{
		if (*this != rhs)
		{
			this->m_red = rhs.m_red;
			this->m_green = rhs.m_green;
			this->m_blue = rhs.m_blue;
		}
		return *this;
	}

	CColour &operator = (const std::string &rhs)
	{
		char c;
		std::istringstream i_str(rhs);
		i_str >> c >> m_red >> c >> m_green >> c >> m_blue;
		return *this;
	}

	const CColour operator + (const CColour &rhs) const
	{
		CColour ret_val; 
		ret_val.m_red = this->m_red + rhs.m_red;
		ret_val.m_green = this->m_green + rhs.m_green;
		ret_val.m_blue = this->m_blue + rhs.m_blue;
		return ret_val;
	}

	const CColour operator - (const CColour &rhs) const
	{
		CColour ret_val; 
		ret_val.m_red = this->m_red - rhs.m_red;
		ret_val.m_green = this->m_green - rhs.m_green;
		ret_val.m_blue = this->m_blue - rhs.m_blue;
		return ret_val;
	}

	const CColour operator * (const CColour &rhs) const
	{
		CColour ret_val; 
		ret_val.m_red = this->m_red * rhs.m_red;
		ret_val.m_green = this->m_green * rhs.m_green;
		ret_val.m_blue = this->m_blue * rhs.m_blue;
		return ret_val;
	}

	bool operator == (const CColour &rhs)
	{ 
		if ((this->m_red == rhs.m_red) && (this->m_green == rhs.m_green) && (this->m_blue == rhs.m_blue))
			return true;
		else
			return false;
	}

	bool operator != (const CColour &rhs)
	{ 
		if ((this->m_red != rhs.m_red) || (this->m_green != rhs.m_green) || (this->m_blue != rhs.m_blue))
			return true;
		else
			return false;
	}

	bool WriteXml(CXml &element)
	{
		if (element.GetTag().empty())
			element.SetTag("COLOUR");
		element.AddAttribute("RED", m_red);
		element.AddAttribute("GREEN", m_green);
		element.AddAttribute("BLUE", m_blue);

		return true;
	}

	static IioXml *Create(CXml &element)
	{
		unsigned int red;
		unsigned int blue;
		unsigned int green;

		element.GetAttribute("RED", &red);
		element.GetAttribute("GREEN", &green);
		element.GetAttribute("BLUE", &blue);

		return new CColour(red, green, blue);
	}

	std::string Str() const
	{
		std::ostringstream o_str;
		o_str << "(" << (unsigned int)m_red << ", " << (unsigned int)m_green << ", " << (unsigned int)m_blue << ")";
		return o_str.str();
	}

	void SetRed(unsigned char red) { m_red = red; }
	void SetGreen(unsigned char green) { m_green = green; }
	void SetBlue(unsigned char blue) { m_blue = blue; }
	unsigned char Red() const { return m_red; }
	unsigned char Green() const	{ return m_green; }
	unsigned char Blue() const { return m_blue; }

private:
	unsigned char m_red;
	unsigned char m_green;
	unsigned char m_blue;

	friend std::ostream &operator << (std::ostream &os, const CColour &colour);
	friend std::istream &operator >> (std::istream &is, CColour &colour);
};

namespace ColourSpace
{
	const CColour RED(255, 0, 0);
	const CColour GREEN(255, 0, 0);
	const CColour BLUE(255, 0, 0);
	const CColour BLACK(0, 0, 0);
	const CColour WHITE = CColour(255, 255, 255);

}
#endif // !defined(COLOUR_H)
