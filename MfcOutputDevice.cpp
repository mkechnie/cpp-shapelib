// MfcOutputDevice.cpp: implementation of the CMfcOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "MfcOutputDevice.h"
#include "Coordinate.h"
#include "Shape.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMfcOutputDevice::CMfcOutputDevice()
: m_drawn_selected_border(false)
{

}

CMfcOutputDevice::~CMfcOutputDevice()
{

}

void CMfcOutputDevice::InitialiseDC(CDC *dc)
{
	m_font.erase();
	m_dc = dc;
	//m_dc->SelectStockObject(HOLLOW_BRUSH);
	//m_dc->SetBkMode(TRANSPARENT);
}

void CMfcOutputDevice::Line(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if (!m_dc)
		return;

	CBrush brush(RGB(m_background.Red(), m_background.Green(), m_background.Blue()));
	CBrush *old_brush = m_dc->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(m_foreground.Red(), m_foreground.Green(), m_foreground.Blue()));
	CPen *old_pen = m_dc->SelectObject(&pen);

	m_dc->MoveTo(top_left.X(), top_left.Y());
	m_dc->LineTo(bottom_right.X(), bottom_right.Y());
	m_dc->SelectObject(old_pen);
	m_dc->SelectObject(old_brush);
}

void CMfcOutputDevice::Rectangle(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if (!m_dc)
		return;

	CBrush brush(RGB(m_background.Red(), m_background.Green(), m_background.Blue()));
	CBrush *old_brush = m_dc->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(m_foreground.Red(), m_foreground.Green(), m_foreground.Blue()));
	CPen *old_pen = m_dc->SelectObject(&pen);

	m_dc->Rectangle(top_left.X(), top_left.Y(), bottom_right.X(), bottom_right.Y());
	m_dc->SelectObject(old_pen);
	m_dc->SelectObject(old_brush);
}

void CMfcOutputDevice::Ellipse(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if (!m_dc)
		return;

	CBrush brush(RGB(m_background.Red(), m_background.Green(), m_background.Blue()));
	CBrush *old_brush = m_dc->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(m_foreground.Red(), m_foreground.Green(), m_foreground.Blue()));
	CPen *old_pen = m_dc->SelectObject(&pen);

	m_dc->Ellipse(top_left.X(), top_left.Y(), bottom_right.X(), bottom_right.Y());
	m_dc->SelectObject(old_pen);
	m_dc->SelectObject(old_brush);
}

void CMfcOutputDevice::SetFont(const int &point_size, std::string &font)
{
	m_point_size = point_size;
	m_font = font;
}

void CMfcOutputDevice::TextAt(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text)
{
	CFont font;

	if (!m_dc)
		return;

	if ((m_point_size) && (!m_font.empty()))
	{
		font.CreatePointFont(m_point_size * 10, _T( m_font.c_str() ));
		m_dc->SelectObject(&font);
	}

	COLORREF text_clr = m_dc->GetTextColor();
	m_dc->SetTextColor(RGB(m_foreground.Red(), m_foreground.Green(), m_foreground.Blue()));
	CRect rect(top_left.X(), top_left.Y(), bottom_right.X(), bottom_right.Y());
	m_dc->DrawText(_T(text.c_str()), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	m_dc->SetTextColor(text_clr);
}

void CMfcOutputDevice::TextIn(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text)
{
	CFont font;
	CRect rect(top_left.X(), top_left.Y(), bottom_right.X(), bottom_right.Y());

	if (!m_dc)
		return;

	if ((m_point_size) && (!m_font.empty()))
	{
		font.CreatePointFont(m_point_size * 10, _T( m_font.c_str() ));
		m_dc->SelectObject(&font);
	}

	CBrush brush(RGB(m_background.Red(), m_background.Green(), m_background.Blue()));
	CBrush *old_brush = m_dc->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(m_foreground.Red(), m_foreground.Green(), m_foreground.Blue()));
	CPen *old_pen = m_dc->SelectObject(&pen);
	COLORREF text_clr = m_dc->GetTextColor();
	m_dc->SetTextColor(RGB(m_foreground.Red(), m_foreground.Green(), m_foreground.Blue()));
	m_dc->DrawText(_T(text.c_str()), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	m_dc->SetTextColor(text_clr);
	m_dc->SelectObject(old_pen);
	m_dc->SelectObject(old_brush);
}

void CMfcOutputDevice::Erase(CShape &shape)
{
	if (!m_dc)
		return;

	COLORREF text_clr = m_dc->GetTextColor();
	COLORREF back_clr = m_dc->GetBkColor();

	LOGPEN log_pen;
	CPen new_pen;
	CPen *old_pen = m_dc->GetCurrentPen();
	old_pen->GetLogPen(&log_pen);
	log_pen.lopnColor = back_clr;
	new_pen.CreatePenIndirect(&log_pen);
	m_dc->SelectObject(new_pen);

	m_dc->SetTextColor(back_clr);

	shape.Draw();

	m_dc->SetTextColor(text_clr);
	m_dc->SelectObject(old_pen);
}

void CMfcOutputDevice::DrawSelectedBorder(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if (!m_dc)
		return;

	CRect rect(top_left.X(), top_left.Y(), bottom_right.X(), bottom_right.Y());
	m_dc->DrawFocusRect(&rect);
	m_drawn_selected_border = true;
}

void CMfcOutputDevice::RemoveSelectedBorder(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if (!m_drawn_selected_border)
		DrawSelectedBorder(top_left, bottom_right);
	m_drawn_selected_border = false;
}
void CMfcOutputDevice::SetForeground(const CColour &colour)
{
	m_foreground = colour;
}

void CMfcOutputDevice::SetBackground(const CColour &colour)
{
	m_background = colour;
}

void CMfcOutputDevice::Group(CShape &shape)
{
	return;
}


