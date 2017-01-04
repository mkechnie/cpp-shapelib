// MfcOutputDevice.h: interface for the CMfcOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MFC_OUTPUT_DEVICE_H)
#define MFC_OUTPUT_DEVICE_H

#include "AfxWin.h"
#include "BaseOutputDevice.h"
#include "Colour.h"

class CMfcOutputDevice : public CBaseOutputDevice  
{
public:
	CMfcOutputDevice();
	virtual ~CMfcOutputDevice();

	void InitialiseDC(CDC *dc);
	void Line(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void Rectangle(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void Ellipse(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void TextAt(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text);
	void SetFont(const int &point_size, std::string &font);
	void TextIn(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text);
	void Erase(CShape &shape);
	void DrawSelectedBorder(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void RemoveSelectedBorder(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void Group(CShape &shape);
	void SetForeground(const CColour &colour);
	void SetBackground(const CColour &colour);

private:
	CDC *m_dc;
	std::string m_font;
	int m_point_size;

	CColour	m_foreground;
	CColour m_background;

	bool m_drawn_selected_border;
};

#endif // !defined(MFC_OUTPUT_DEVICE_H)
