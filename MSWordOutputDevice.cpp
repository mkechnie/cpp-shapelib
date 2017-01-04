// MSWordOutputDevice.cpp: implementation of the CMSWordOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "MSWordOutputDevice.h"
#include "Coordinate.h"
#include "Shape.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleVariant vTrue((short)TRUE);
COleVariant vFalse((short)FALSE);
COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

CMSWordOutputDevice::CMSWordOutputDevice() 
	: m_point_size(0), m_application_created(FALSE), m_top_margin(0), m_left_margin(0)
{

}

CMSWordOutputDevice::~CMSWordOutputDevice()
{

}

void CMSWordOutputDevice::CreateDocument()
{
	m_application_created = m_app.CreateDispatch("Word.Application");
	if(!m_application_created)
	{
		AfxMessageBox("Couldn't start Word and get Application object.");
		return;
	}

	// Size word
	m_app.Resize(700, 500);
	m_app.SetVisible(TRUE);

	// Create A3 landscape document
	m_docs = m_app.GetDocuments();
	m_doc = m_docs.Add(vOpt, vOpt,vOpt, vOpt);
	PageSetup page_setup = m_doc.GetPageSetup();
	page_setup.SetPageHeight(841.95F);
	page_setup.SetPageWidth(1190.7F);
	m_top_margin = page_setup.GetTopMargin();
	m_left_margin = page_setup.GetLeftMargin();

	Window window = m_app.GetActiveWindow();
	Pane pane = window.GetActivePane();
	View view = pane.GetView();
	Zoom zoom = view.GetZoom();
	zoom.SetPercentage(75);

	m_shapes = m_doc.GetShapes();
}


void CMSWordOutputDevice::Line(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if(!m_application_created)
		return;

	m_shapes.AddLine(top_left.X() + m_left_margin, top_left.Y() + m_top_margin, bottom_right.X() + m_left_margin, bottom_right.Y() + m_top_margin, vOpt);
}

void CMSWordOutputDevice::Rectangle(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if(!m_application_created)
		return;

	int width = bottom_right.X() - top_left.X();
	int height = bottom_right.Y() - top_left.Y();

	Shape ellipse = m_shapes.AddShape(1, top_left.X() + m_left_margin, top_left.Y() + m_top_margin, width, height, vOpt);
	FillFormat fill = ellipse.GetFill();
	fill.SetVisible(0);
}

void CMSWordOutputDevice::Ellipse(const CCoordinate &top_left, const CCoordinate &bottom_right)
{
	if(!m_application_created)
		return;

	int width = bottom_right.X() - top_left.X();
	int height = bottom_right.Y() - top_left.Y();

	Shape ellipse = m_shapes.AddShape(9, top_left.X() + m_left_margin, top_left.Y() + m_top_margin, width, height, vOpt);
	FillFormat fill = ellipse.GetFill();
	CString str = ellipse.GetName();
	TRACE("\n");
	TRACE(str);
	fill.SetVisible(0);
}

void CMSWordOutputDevice::SetFont(const int &point_size, std::string &font)
{
	if(!m_application_created)
		return;

	m_point_size = point_size;
	m_font = font;
}

void CMSWordOutputDevice::TextAt(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text)
{
	// create a textbox with no fill line or margin
	int width = bottom_right.X() - top_left.X();
	int height = bottom_right.Y() - top_left.Y();
	Shape text_box = m_shapes.AddTextbox(1, top_left.X() + m_left_margin, top_left.Y() + m_top_margin, width, height, vOpt);
	FillFormat fill = text_box.GetFill();
	fill.SetVisible(0);
	LineFormat line = text_box.GetLine();
	line.SetVisible(0);
	TextFrame text_frame = text_box.GetTextFrame();
	text_frame.SetMarginBottom(0.0);
	text_frame.SetMarginTop(0.0);
	text_frame.SetMarginLeft(0.0);
	text_frame.SetMarginRight(0.0);
	Range range = text_frame.GetTextRange();

	// Create a font
	if ((m_point_size) && (!m_font.empty()))
	{
		_Font font = range.GetFont();
		font.SetNameAscii(m_font.c_str());
		font.SetSize(m_point_size);
	}

	range.SetText(_T(text.c_str()));
}

void CMSWordOutputDevice::TextIn(const CCoordinate &top_left, const CCoordinate &bottom_right, const std::string &text)
{
	if(!m_application_created)
		return;

	// create a textbox with no fill line or margin
	int width = bottom_right.X() - top_left.X();
	int height = bottom_right.Y() - top_left.Y();
	Shape text_box = m_shapes.AddTextbox(1, top_left.X() + m_left_margin, top_left.Y() + m_top_margin, width, height, vOpt);
	FillFormat fill = text_box.GetFill();
	fill.SetVisible(0);
	LineFormat line = text_box.GetLine();
	line.SetVisible(0);
	TextFrame text_frame = text_box.GetTextFrame();
	text_frame.SetMarginBottom(0.0);
	text_frame.SetMarginTop(0.0);
	text_frame.SetMarginLeft(0.0);
	text_frame.SetMarginRight(0.0);
	Range range = text_frame.GetTextRange();

	// Create a font
	if ((m_point_size) && (!m_font.empty()))
	{
		_Font font = range.GetFont();
		font.SetNameAscii(m_font.c_str());
		font.SetSize(m_point_size);
	}

	// Create a table in the text box
	Tables tables = range.GetTables();
	Table table = tables.Add(range, 1, 1, vOpt, vOpt);
	table.SetPreferredWidth(80);
	Borders borders = table.GetBorders();

	// Hide the lines of the table
	borders.SetEnable(0);
	Window window = m_app.GetActiveWindow();
	View view = window.GetView();
	view.SetTableGridlines(0);

	// Get the cell of the table
	Range tbl_range = table.GetRange();
	Cells cells = tbl_range.GetCells();
	Cell cell = cells.Item(1);

	// set the height and width of the cell
	cell.SetHeight(height);
	cell.SetWidth(width);

	// align the text of the cell in the middle
	cell.SetVerticalAlignment(1);
	Range cell_range = cell.GetRange();
	Paragraph paragraph = cell_range.GetParagraphFormat();
	paragraph.SetAlignment(1);

	// Set the text of the cell
	cell_range.SetText(_T(text.c_str()));
}
/*
void CMSWordOutputDevice::GroupMostRecentShapes(int count)
{

	Template cur_template = m_app.GetNormalTemplate();
	OLECHAR FAR* szMember = OLESTR("GroupShapes");
      DISPID dispid;
      HRESULT res = cur_template.m_lpDispatch->GetIDsOfNames ( IID_NULL,
                               &szMember,  //Function name.
                               1,          //Number of functions.
                               LOCALE_SYSTEM_DEFAULT,
                               &dispid);

							   if (res == DISP_E_UNKNOWNNAME)
      {
       AfxMessageBox("Unable to get dispID for GroupShapes");
      }

      COleVariant result;
      UCHAR *parmStr = (BYTE *)( VTS_VARIANT );
      COleVariant x(3L);
      //x = COleVariant(3);
      cur_template.InvokeHelper(dispid,
                        DISPATCH_METHOD | DISPATCH_PROPERTYGET,
                        VT_VARIANT,
                        (void *)&result,
                        parmStr,
                        &x);
						*/



	
/*
	VARIANT varColInfo[3];

	//initialize VARIANTs
	for (int i = 0; i < 3; i++)
	   VariantInit(&varColInfo[i]);

	// Column Name
	varColInfo[0].vt = VT_BSTR;
	varColInfo[0].bstrVal = ::SysAllocString(L"Oval 2");

	// Column Type
	varColInfo[1].vt = VT_BSTR;
	varColInfo[1].bstrVal = ::SysAllocString(L"Oval 10");
	//varColInfo[0].vt = VT_UI4;
	//varColInfo[0].lVal = 1;
	//varColInfo[1].vt = VT_UI4;
	//varColInfo[1].lVal = 2;

	COleSafeArray sa;
	//create a 1 dimensional safearray of VARIANTs
	//& initialize it with varColInfo VARIANT array
//	sa.CreateOneDim(VT_VARIANT, 2, varColInfo);

	COleSafeArray saRet;
	long values[] = {1, 2, 3, 4};
	saRet.CreateOneDim(VT_I4, 4, values);

	VARIANT variant;
	VariantInit (&variant);
 variant.vt = VT_ARRAY | VT_INT; // Array of integers
 
 SAFEARRAY* psa;
 // 10 elements numbered 0-9
 SAFEARRAYBOUND bound = { 2, 1 }; 
 psa = SafeArrayCreate (VT_I4, 1, &bound);

 for (long i=1; i<3; i++)
	{
	 int val = i + 1;
	 SafeArrayPutElement (psa, &i, &val);

//     psa->PutElement(&i, &i);
	}

 long i = 1;
 long val = 2;
	 SafeArrayPutElement (psa, &i, &val);
	 i = 2;
  val = 10;
	 SafeArrayPutElement (psa, &i, &val);


 variant.parray = psa;
 */

// Create a new 1-D array of Integers.
	  /*
SAFEARRAY * sa; 
SAFEARRAYBOUND aDim[1]; 
aDim[0].lLbound = 0; 
aDim[0].cElements = 8; 
// Equivalent to: Dim aiNew(1 To 8) As Integer.
sa = SafeArrayCreate(VT_I4, 1, aDim);
if (sa == NULL) throw ERROR_NOT_ENOUGH_MEMORY;

VARIANT vt;
long index;
long value;

index = 0;
//SafeArrayGetElement(sa, &index, &vt);
//VariantInit(&vt);
	//vt.vt = VT_UI4;
	//vt.lVal = 1;
	//vt.vt = VT_BSTR;
	//vt.bstrVal = ::SysAllocString(L"Oval 2");
	//SafeArrayPutElement(sa, &index, &vt);

	index = 1;
//SafeArrayGetElement(sa, &index, &vt);
//VariantInit(&vt);
	//vt.vt = VT_UI4;
	//vt.lVal = 2;
	//	vt.vt = VT_BSTR;
	//vt.bstrVal = ::SysAllocString(L"Oval 10");
	//SafeArrayPutElement(sa, &index, &vt);

	index = 0;
SafeArrayGetElement(sa, &index, &value);
//VariantInit(&vt);
	//vt.vt = VT_UI4;
	//vt.lVal = 1;
	//vt.vt = VT_BSTR;
	//vt.bstrVal = ::SysAllocString(L"Oval 2");
value = 1L;
	SafeArrayPutElement(sa, &index, &value);

	index = 1;
SafeArrayGetElement(sa, &index, &value);
//VariantInit(&vt);
	//vt.vt = VT_UI4;
	//vt.lVal = 2;
	//	vt.vt = VT_BSTR;
	//vt.bstrVal = ::SysAllocString(L"Oval 10");
value = 2L;
	SafeArrayPutElement(sa, &index, &value);

	//ShapeRange shape_range = m_shapes.Range(&sa);
	ShapeRange shape_range = m_shapes.Range((LPVARIANT)sa);
	long num = shape_range.GetCount();
	char txt[512];
	sprintf(txt, "<%ld>\n", num);
	TRACE(txt);

	
//	ShapeRange shape_range = m_shapes.Range(COleVariant("Oval 2"));

//	shape_range.Group();
 Shape shape = m_shapes.Item(COleVariant(2L));
 TRACE("\n");
 	CString str = shape.GetName();
	TRACE("\n");
	TRACE(str);

	
/*
	VARIANT shapes[4];
	for (int i=0; i < 4; i++)
	{
		//::VariantInit(&shapes[i]);
		shapes[i].vt = VT_INT;
		shapes[i].intVal = i + 1;
	}

	ShapeRange shape_range = m_shapes.Range(shapes);
	shape_range.Group();
	*/
/*	
}
*/

void CMSWordOutputDevice::SetForeground(const CColour &colour)
{
	return;
}

void CMSWordOutputDevice::SetBackground(const CColour &colour)
{
	return;
}

void CMSWordOutputDevice::Erase(CShape &shape)
{
	return;
}

void CMSWordOutputDevice::DrawSelectedBorder(const CCoordinate &top_left, const CCoordinate &bot_right)
{
	return;
}

void CMSWordOutputDevice::RemoveSelectedBorder(const CCoordinate &top_left, const CCoordinate &bot_right)
{
	return;
}

void CMSWordOutputDevice::Group(CShape &shape)
{
	return;
}


/*
      // Instantiate Excel and start a new workbook.
      objApp.CreateDispatch("Excel.Application");
      objBooks = objApp.GetWorkbooks();
      objBook = objBooks.Add(VOptional);
      objSheets = objBook.GetWorksheets();
      objSheet = objSheets.GetItem(COleVariant((short)1));

      //Get the range where the starting cell has the address
      //m_sStartingCell and it's dimensions are m_iNumRows x m_iNumCols.
      range = objSheet.GetRange(COleVariant(m_sStartingCell),
                                COleVariant(m_sStartingCell));
      range = range.GetResize(COleVariant(m_iNumRows),
                              COleVariant(m_iNumCols));

      //*** Fill the range with an array of values.

      //Create the SAFEARRAY.
      COleSafeArray saRet;
      DWORD numElements[2];
      numElements[0]= m_iNumRows;   //Number of rows in the range.
      numElements[1]= m_iNumCols;   //Number of columns in the range.

      if(m_bFillWithStrings)
      {
         saRet.Create(VT_BSTR, 2, numElements);
      }
      else
      {
         saRet.Create(VT_R8, 2, numElements);
      }

      //Fill the SAFEARRAY.
      long index[2];
      long iRow;
      long iCol;

      for(iRow=0;iRow<=m_iNumRows-1;iRow++)
      {
         for(iCol=0;iCol<=m_iNumCols-1;iCol++)
         {
            index[0] = iRow;
            index[1] = iCol;
            if(m_bFillWithStrings)      //Fill with Strings.
            {
               VARIANT v;
               CString s;
               VariantInit(&v);
               v.vt = VT_BSTR;
               s.Format("r%dc%d", iRow, iCol);
               v.bstrVal = s.AllocSysString();
               saRet.PutElement(index, v.bstrVal);
               SysFreeString(v.bstrVal);
               VariantClear(&v);
            }
            else                     //Fill with Numbers.
            {
               double d;
               d = (iRow*1000) + iCol;
               saRet.PutElement(index, &d);
            }
         }
      }

      //Set the range value to the SAFEARRAY.
      range.SetValue(COleVariant(saRet));
      saRet.Detach();

      //Return control of Excel to the user.
      objApp.SetVisible(TRUE);
      objApp.SetUserControl(TRUE);
*/
