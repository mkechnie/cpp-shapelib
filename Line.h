// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LINE_H)
#define LINE_H

#include "Shape.h"
#include "Coordinate.h"

class CLine : public CShape 
{
public:
	CLine(const CCoordinate &origin, const CCoordinate &bottom_right) 
		: CShape(origin, bottom_right)
	{ 	}
	CLine(const unsigned int top_x=0, const unsigned int top_y=0, const unsigned int bot_x=80, const unsigned int bot_y=40) 
		: CShape(top_x, top_y, bot_x, bot_y)
	{ }
	virtual ~CLine() { }

	void Draw(CBaseOutputDevice *output_device=0, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		CShape::Draw(output_device);
		Line();	
	}
};

#endif // !defined(LINE_H)