// Shape.h: interface for the CShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SHAPE_H)
#define SHAPE_H

#include "Coordinate.h"
#include "Colour.h"
#include "BaseOutputDevice.h"
#include <vector>
#include <string>
#include <algorithm>
#include "Xml.h"

class CShape
{
protected:
	CShape(const CCoordinate &origin, const CCoordinate &bottom_right) 
		: m_origin(origin), m_bottom_right(bottom_right), m_selected(false), m_visible(true),  
		m_output_device(0), m_parent_group(0), m_foreground(ColourSpace::BLACK), m_background(ColourSpace::WHITE),
		m_font("Times New Roman"), m_point_size(12)
	{ }
	CShape(const unsigned int top_x, const unsigned int top_y, const unsigned int bot_x, const unsigned int bot_y)
		: m_origin(top_x, top_y), m_bottom_right(bot_x, bot_y), m_selected(false), m_visible(true),
		m_output_device(0), m_parent_group(0), m_foreground(ColourSpace::BLACK), m_background(ColourSpace::WHITE),
		m_font("Times New Roman"), m_point_size(12)
	{ }
public:
	enum GroupAction {APPLY_TO_GROUP, NOT_APPLY_TO_GROUP};
	virtual ~CShape() 
	{ 
		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); iter != m_grouped_shapes.end(); ++iter)
			(*iter)->Ungroup();
		Ungroup();
	}

	CCoordinate TopLeft() const { return m_origin; }
	CCoordinate BottomRight() const { return m_bottom_right; }
	CColour Foreground() const { return m_foreground; }
	CColour Background() const { return m_background; }
	int Width() const { return m_bottom_right.X() - m_origin.X(); }
	int Height() const { return m_bottom_right.Y() - m_origin.Y(); }

	void SetTopLeft(const CCoordinate &top_left) 
	{
		Move(top_left); 
	}
	void SetBottomRight(const CCoordinate &bottom_right) 
	{
		Resize(bottom_right.X() - m_origin.X(), bottom_right.Y() - m_origin.Y()); 
	}

	void SetForeground(CColour &colour, GroupAction apply_to_groups=APPLY_TO_GROUP) 
	{ 
		m_foreground = colour; 

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->SetForeground(colour);
	}
	void SetBackground(CColour &colour, GroupAction apply_to_groups=APPLY_TO_GROUP) 
	{ 
		m_background = colour; 
		
		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->SetBackground(colour);
	}

	virtual void Draw(CBaseOutputDevice *output_device=0, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		if (!m_visible)
			return;

		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
		{ 
			m_output_device->SetForeground(m_foreground);
			m_output_device->SetBackground(m_background);
			if (m_selected)
				m_output_device->DrawSelectedBorder(m_origin, m_bottom_right);
		}
		
		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->Draw(output_device);
	}
	virtual void MoveRelative(const CCoordinate &new_pos, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		int height = Height();
		int width = Width();

		m_origin = m_origin + new_pos;
		m_bottom_right.SetX(m_origin.X() + width);
		m_bottom_right.SetY(m_origin.Y() + height);

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->MoveRelative(new_pos);
	}
	virtual void Move(const CCoordinate &new_pos, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		int height = Height();
		int width = Width();
		CCoordinate rel_pos = new_pos - m_origin;

		m_origin = new_pos;
		m_bottom_right.SetX(m_origin.X() + width);
		m_bottom_right.SetY(m_origin.Y() + height);

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->MoveRelative(rel_pos);
	}
	virtual void Resize(int width, int height, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		if ((width == Width()) && (height == Height()))
			return;

		double width_ratio = (double)width / (double)Width();
		double height_ratio = (double)height / (double)Height();

		m_bottom_right.SetX(m_origin.X() + width);
		m_bottom_right.SetY(m_origin.Y() + height);

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
		{
			// Move group object in proportion to the resize. DONT move sub grouped shapes as these will
			// be moved when they are resized.
			CCoordinate new_origin(((*iter)->TopLeft().X() - m_origin.X()) * width_ratio + m_origin.X(), 
								   ((*iter)->TopLeft().Y() - m_origin.Y()) * height_ratio+ m_origin.Y());
			int	new_width = (int)(width_ratio * (double)((*iter)->Width()));
			int	new_height = (int)(height_ratio * (double)((*iter)->Height()));
			(*iter)->Move(new_origin, NOT_APPLY_TO_GROUP);
			(*iter)->Resize(new_width, new_height);
		}
	}

	virtual void UseOutputDevice(CBaseOutputDevice *output_device, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		m_output_device = output_device;

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->UseOutputDevice(output_device);
	}

	void Select(CBaseOutputDevice *output_device=0) 
	{ 
		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
			m_output_device->DrawSelectedBorder(m_origin, m_bottom_right);
		m_selected = true; 
	}
	void Deselect(CBaseOutputDevice *output_device=0) 
	{ 
		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
			m_output_device->RemoveSelectedBorder(m_origin, m_bottom_right);
		m_selected = false; 
	}
	bool IsSelected() const { return m_selected; }

	void Show(GroupAction apply_to_groups=APPLY_TO_GROUP) 
	{ 
		m_visible = true;
		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->Show();
	}
	void Hide(GroupAction apply_to_groups=APPLY_TO_GROUP) 
	{ 
		m_visible = false; 
		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
				 (*iter)->Hide();
	}
	bool IsVisible() const { return m_visible; }

	virtual void Erase(CBaseOutputDevice *output_device=0, GroupAction apply_to_groups=APPLY_TO_GROUP)
	{
		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
			m_output_device->Erase(*this); 

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->Erase(output_device);
	}

	void Rectangle(CBaseOutputDevice *output_device=0)
	{ 
		if (!m_visible)
			return;
		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
			m_output_device->Rectangle(m_origin, m_bottom_right); 
	}
	void Line(CBaseOutputDevice *output_device=0)
	{
		if (!m_visible)
			return;
		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
			m_output_device->Line(m_origin, m_bottom_right); 
	}
	void Ellipse(CBaseOutputDevice *output_device=0)
	{ 
		if (!m_visible)
			return;
		if (output_device)
			UseOutputDevice(output_device);
		if (m_output_device)
			m_output_device->Ellipse(m_origin, m_bottom_right); 
	}

	bool InShapeArea(const CCoordinate &coord) const
	{
		if ((m_origin.X() <= coord.X() && coord.X() < m_bottom_right.X()) &&
			(m_origin.Y() <= coord.Y() && coord.Y() < m_bottom_right.Y()))
		{
			return true;
		} else {
			return false;
		}
	}

	bool OnTopEdge(const CCoordinate &coord) const
	{
		if ((m_origin.X() <= coord.X() && coord.X() < m_bottom_right.X()) &&
			(m_origin.Y() == coord.Y()))
		{
			return true;
		} else {
			return false;
		}
	}

	bool OnBottomEdge(const CCoordinate &coord) const
	{
		if ((m_origin.X() <= coord.X() && coord.X() < m_bottom_right.X()) &&
			(coord.Y() == m_bottom_right.Y() - 1))
		{
			return true;
		} else {
			return false;
		}
	}

	bool OnLeftEdge(const CCoordinate &coord) const
	{
		if ((m_origin.X() == coord.X()) &&
			(m_origin.Y() <= coord.Y() && coord.Y() < m_bottom_right.Y()))
		{
			return true;
		} else {
			return false;
		}
	}

	bool OnRightEdge(const CCoordinate &coord) const
	{
		if ((coord.X() == m_bottom_right.X() - 1) &&
			(m_origin.Y() <= coord.Y() && coord.Y() < m_bottom_right.Y()))
		{
			return true;
		} else {
			return false;
		}
	}

	void Group(CShape &shape)
	{
		shape.m_parent_group = this;
		m_grouped_shapes.push_back(&shape);
	}

	void Ungroup()
	{
		if (m_parent_group)
			std::remove(m_grouped_shapes.begin(), m_grouped_shapes.end(), this);
		m_parent_group = 0;
	}

	void SetFont(const int point_size, const std::string &font, GroupAction apply_to_groups=APPLY_TO_GROUP) 
	{ 
		m_font = font; 
		m_point_size = point_size;

		for (std::vector<CShape *>::iterator iter = m_grouped_shapes.begin(); 
			 ((apply_to_groups==APPLY_TO_GROUP) && (iter != m_grouped_shapes.end())); ++iter)
			(*iter)->SetFont(point_size, font);
	}
	const std::string &Font() { return m_font; }

	int PointSize() { return m_point_size; }

	virtual bool WriteXml(CXml &element)
	{
		element.AddAttribute("topleft", m_origin.Str());
		element.AddAttribute("width", Width());
		element.AddAttribute("height", Height());
		element.AddAttribute("foreground", m_foreground);
		element.AddAttribute("background", m_background);
		element.AddAttribute("font", m_font);
		element.AddAttribute("pointsize", m_point_size);
		element.AddAttribute("visible", m_visible);
		element.AddAttribute("selected", m_selected);

		return true;
	}
	virtual bool ReadXml(CXml &element)
	{
		bool visible(true);
		bool selected(false);
		CCoordinate top_left;
		int width(0);
		int height(0);
		element.GetAttribute("width", &width);
		element.GetAttribute("height", &height);
		element.GetAttribute("topleft", &top_left);
		element.GetAttribute("foreground", &m_foreground);
		element.GetAttribute("background", &m_background);
		element.GetAttribute("font", &m_font);
		element.GetAttribute("pointsize", &m_point_size);
		element.GetAttribute("visible", &visible);
		element.GetAttribute("selected", &selected);

		if ((width) && (height))
			Resize(width, height);
		Move(top_left);
		SetForeground(m_foreground);
		SetBackground(m_background);
		SetFont(m_point_size, m_font);
		if (visible)
			Show();
		else
			Hide();
		if (selected)
			Select();

		return true;
	}


protected:
	bool		m_visible;
	bool		m_selected;
	CCoordinate	m_origin;
	CCoordinate m_bottom_right;
	CBaseOutputDevice *m_output_device;
	CColour		m_foreground;
	CColour		m_background;
	std::string m_font;
	int m_point_size;

	std::vector<CShape *> m_grouped_shapes;
	CShape *m_parent_group;
};

#endif // !defined(SHAPE_H)
