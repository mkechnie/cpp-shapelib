// Rectangle.h: interface for the CRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(RECTANGLE_H)
#define RECTANGLE_H

#include "Shape.h"
#include "Coordinate.h"

class CRectangle : public CShape 
{
public:
	CRectangle(const CCoordinate &origin, const CCoordinate &bottom_right) 
		: CShape(origin, bottom_right)
	{ 	}
	CRectangle(const unsigned int top_x=0, const unsigned int top_y=0, const unsigned int bot_x=80, const unsigned int bot_y=40) 
		: CShape(top_x, top_y, bot_x, bot_y)
	{ }
	virtual ~CRectangle() { }
	void Draw(CBaseOutputDevice *output_device=0, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		CShape::Draw(output_device);
		Rectangle();		
	}
};

#endif // !defined(RECTANGLE_H)