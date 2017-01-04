// MSWordOutputDevice.h: interface for the CMSWordOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MSWORD_OUTPUT_DEVICE_H)
#define MSWORD_OUTPUT_DEVICE_H

#include <afxdisp.h>
#include "msword9.h"
#include "BaseOutputDevice.h"

class CMSWordOutputDevice : public CBaseOutputDevice  
{
public:
	CMSWordOutputDevice();
	virtual ~CMSWordOutputDevice();

	void CreateDocument();
	void Line(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void Rectangle(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void Ellipse(const CCoordinate &top_left, const CCoordinate &bottom_right);
	void TextAt(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text);
	void SetFont(const int &point_size, std::string &font);
	void TextIn(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text);
	void Erase(CShape &shape);
	void DrawSelectedBorder(const CCoordinate &, const CCoordinate &);
	void RemoveSelectedBorder(const CCoordinate &, const CCoordinate &);
	void Group(CShape &shape);
	void SetForeground(const CColour &colour);
	void SetBackground(const CColour &colour);

private:
	BOOL m_application_created;
	_Application m_app;
	Documents m_docs;
	_Document m_doc;
	Shapes m_shapes;
	std::string m_font;
	int m_point_size;
	int m_top_margin;
	int m_left_margin;
};

#endif // !defined(MSWORD_OUTPUT_DEVICE_H)