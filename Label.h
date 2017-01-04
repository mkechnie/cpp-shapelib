// Label.h: interface for the CLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LABEL_H)
#define LABEL_H

#include <string>
#include "Shape.h"
#include "Coordinate.h"

class CLabel  : public CShape
{
public:
	CLabel(const CCoordinate &origin, const CCoordinate &bottom_right) 
		: CShape(origin, bottom_right)
	{ 
		m_font.erase(); 
		m_label.erase(); 
	}
	CLabel(const unsigned int top_x=0, const unsigned int top_y=0, const unsigned int bot_x=80, const unsigned int bot_y=40) 
		: CShape(top_x, top_y, bot_x, bot_y)
	{ 
		m_font.erase(); 
		m_label.erase(); 
	}
	virtual ~CLabel() { }

	void SetLabel(const std::string &label) { m_label = label; }
	const std::string &Label() { return m_label; }

	void DrawAt(CBaseOutputDevice *output_device=0)
	{
		CShape::Draw(output_device);
		if (m_output_device)
		{
			if (!m_font.empty())
				m_output_device->SetFont(m_point_size, m_font);
			m_output_device->TextAt(m_origin, m_bottom_right, m_label);
		}
	}

	void DrawIn(CBaseOutputDevice *output_device=0)
	{
		CShape::Draw(output_device);
		if (m_output_device)
		{
			if (!m_font.empty())
				m_output_device->SetFont(m_point_size, m_font);
			m_output_device->TextIn(m_origin, m_bottom_right, m_label);
		}
	}

	void Draw(CBaseOutputDevice *output_device=0, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		DrawIn(output_device);
	}

private:
	std::string m_label;
};

#endif // !defined(LABEL_H)
