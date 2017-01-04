// Ellipse.h: interface for the CEllipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ELLIPSE_H)
#define ELLIPSE_H

#include "Shape.h"
#include "Coordinate.h"


class CEllipse : public CShape 
{
public:
	CEllipse(const CCoordinate &origin, const CCoordinate &bottom_right) 
		: CShape(origin, bottom_right)
	{ 	}
	CEllipse(const unsigned int top_x=0, const unsigned int top_y=0, const unsigned int bot_x=80, const unsigned int bot_y=40) 
		: CShape(top_x, top_y, bot_x, bot_y)
	{ }
	virtual ~CEllipse() { }
	void Draw(CBaseOutputDevice *output_device=0, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		CShape::Draw(output_device);
		Ellipse();		
	}
};

#endif // !defined(ELLIPSE_H)