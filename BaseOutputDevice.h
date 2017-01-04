// BaseOutputDevice.h: interface for the CLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(BASE_OUTPUT_DEVICE_H)
#define BASE_OUTPUT_DEVICE_H

#include <string>

class CCoordinate;
class CShape;
class CColour;

class CBaseOutputDevice
{
public:
	CBaseOutputDevice()	{	}
	virtual ~CBaseOutputDevice() { }

	//void Initialise(CDC *dc);
	virtual void Line(const CCoordinate &, const CCoordinate &) = 0; 
	virtual void Rectangle(const CCoordinate &, const CCoordinate &) = 0;
	virtual void Ellipse(const CCoordinate &, const CCoordinate &) = 0;
	virtual void TextAt(const CCoordinate &, const CCoordinate &, const std::string &) = 0;
	virtual void SetFont(const int &, std::string &) = 0;
	virtual void TextIn(const CCoordinate &, const CCoordinate &, const std::string &) = 0;
	virtual void Erase(CShape &) = 0;
	virtual void DrawSelectedBorder(const CCoordinate &, const CCoordinate &) = 0;
	virtual void RemoveSelectedBorder(const CCoordinate &, const CCoordinate &) = 0;
	virtual void Group(CShape &) = 0;
	virtual void SetForeground(const CColour &colour) = 0;
	virtual void SetBackground(const CColour &colour) = 0;
};

#endif // !defined(BASE_OUTPUT_DEVICE_H)
