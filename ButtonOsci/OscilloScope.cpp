// OscilloScope.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "OscilloScope.h"


// COscilloScope

IMPLEMENT_DYNAMIC(COscilloScope, CButton)

COscilloScope::COscilloScope()
{
	 // since plotting is based on a LineTo for each new point
  // we need a starting point (i.e. a "previous" point)
  // use 0.0 as the default first point.
  // these are public member variables, and can be changed outside
  // (after construction).  Therefore m_perviousPosition could be set to
  // a more appropriate value prior to the first call to SetPosition.
	
	for (int iCh = 0; iCh < FLL_CH; iCh++) {
		m_dPreviousPosition[iCh] = 0.0;
	}
  
  //m_dPreviousPosition[1] =   0.0 ;
  // public variable for the number of decimal places on the y axis
  m_nYDecimals = 3 ;

  // set some initial values for the scaling until "SetRange" is called.
  // these are protected varaibles and must be set with SetRange
  // in order to ensure that m_dRange is updated accordingly
  m_dLowerLimit = -10.0 ;
  m_dUpperLimit =  10.0 ;
  m_dRange      =  m_dUpperLimit - m_dLowerLimit ;   // protected member variable

  // m_nShiftPixels determines how much the plot shifts (in terms of pixels) 
  // with the addition of a new data point
  m_nShiftPixels     = 4 ;
  m_nHalfShiftPixels = m_nShiftPixels/2 ;                     // protected
  m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels ;  // protected

  // background, grid and data colors
  // these are public variables and can be set directly
  m_crBackColor  = RGB(  0,   0,   0) ;  // see also SetBackgroundColor
  m_crGridColor  = RGB(  0, 255, 255) ;  // see also SetGridColor
  m_crPlotColor  = RGB(255, 255, 255) ;  // see also SetPlotColor

  // protected variables
  m_penPlot[0].CreatePen(PS_SOLID, 1, m_crPlotColor) ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

  // public member variables, can be set directly 
  m_strXUnitsString.Format("Samples") ;  // can also be set with SetXUnits
  m_strYUnitsString.Format("Y units") ;  // can also be set with SetYUnits

  // protected bitmaps to restore the memory DC's
  m_pbitmapOldGrid = NULL ;
  m_pbitmapOldPlot = NULL ;
}

COscilloScope::~COscilloScope()
{
	// just to be picky restore the bitmaps for the two memory dc's
	// (these dc's are being destroyed so there shouldn't be any leaks)
	if (m_pbitmapOldGrid != NULL)
		m_dcGrid.SelectObject(m_pbitmapOldGrid);
	if (m_pbitmapOldPlot != NULL)
		m_dcPlot.SelectObject(m_pbitmapOldPlot);
}


BEGIN_MESSAGE_MAP(COscilloScope, CButton)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// COscilloScope message handlers


/////////////////////////////////////////////////////////////////////////////
BOOL COscilloScope::Create(DWORD dwStyle, const RECT& rect,
	CWnd* pParentWnd, UINT nID)
{
	BOOL result;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);

	result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		className, NULL, dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), (HMENU)nID);
	if (result != 0)
		InvalidateCtrl();

	return result;

} // Create

  /////////////////////////////////////////////////////////////////////////////
void COscilloScope::SetRange(double dLower, double dUpper, int nDecimalPlaces, int shifPx)
{
	ASSERT(dUpper > dLower);

	m_nShiftPixels = shifPx;
	m_nHalfShiftPixels = m_nShiftPixels / 2;                     // protected
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels;   // protected

	m_dLowerLimit = dLower;
	m_dUpperLimit = dUpper;
	m_nYDecimals = nDecimalPlaces;
	m_dRange = m_dUpperLimit - m_dLowerLimit;
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange;

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl();

}  // SetRange


   /////////////////////////////////////////////////////////////////////////////
void COscilloScope::SetXUnits(CString string)
{
	m_strXUnitsString = string;

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl();

}  // SetXUnits

   /////////////////////////////////////////////////////////////////////////////
void COscilloScope::SetYUnits(CString string)
{
	m_strYUnitsString = string;

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl();

}  // SetYUnits

   /////////////////////////////////////////////////////////////////////////////
void COscilloScope::SetGridColor(COLORREF color)
{
	m_crGridColor = color;

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl();

}  // SetGridColor


   /////////////////////////////////////////////////////////////////////////////
void COscilloScope::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color;
	switch (FLL_CH) {
	case (3):
		m_penPlot[0].DeleteObject();
		m_penPlot[1].DeleteObject();
		m_penPlot[2].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[1].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[2].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		break;
	case (4):
		m_penPlot[0].DeleteObject();
		m_penPlot[1].DeleteObject();
		m_penPlot[2].DeleteObject();
		m_penPlot[3].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[1].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[2].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_penPlot[3].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		break;
	case(5):
		m_penPlot[0].DeleteObject();
		m_penPlot[1].DeleteObject();
		m_penPlot[2].DeleteObject();
		m_penPlot[3].DeleteObject();
		m_penPlot[4].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[1].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[2].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_penPlot[3].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		m_penPlot[4].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		break;

	case(6):
		m_penPlot[0].DeleteObject();
		m_penPlot[1].DeleteObject();
		m_penPlot[2].DeleteObject();
		m_penPlot[3].DeleteObject();
		m_penPlot[4].DeleteObject();
		m_penPlot[5].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[1].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[2].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_penPlot[3].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		m_penPlot[4].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		m_penPlot[5].CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
		break;
	case(7):
		m_penPlot[0].DeleteObject();
		m_penPlot[1].DeleteObject();
		m_penPlot[2].DeleteObject();
		m_penPlot[3].DeleteObject();
		m_penPlot[4].DeleteObject();
		m_penPlot[5].DeleteObject();
		m_penPlot[6].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[1].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[2].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_penPlot[3].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		m_penPlot[4].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		m_penPlot[5].CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
		m_penPlot[6].CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		break;
	case(8):
		m_penPlot[0].DeleteObject();
		m_penPlot[1].DeleteObject();
		m_penPlot[2].DeleteObject();
		m_penPlot[3].DeleteObject();
		m_penPlot[4].DeleteObject();
		m_penPlot[5].DeleteObject();
		m_penPlot[6].DeleteObject();
		m_penPlot[7].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
		m_penPlot[1].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		m_penPlot[2].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		m_penPlot[3].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		m_penPlot[4].CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		m_penPlot[5].CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
		m_penPlot[6].CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		m_penPlot[7].CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		break;
	default:
		m_penPlot[0].DeleteObject();
		m_penPlot[0].CreatePen(PS_SOLID, 1, m_crPlotColor);
	}

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl();

}  // SetPlotColor


   /////////////////////////////////////////////////////////////////////////////
void COscilloScope::SetBackgroundColor(COLORREF color)
{
	m_crBackColor = color;

	m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(m_crBackColor);

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl();

}  // SetBackgroundColor

   /////////////////////////////////////////////////////////////////////////////
void COscilloScope::InvalidateCtrl()
{
	// There is a lot of drawing going on here - particularly in terms of 
	// drawing the grid.  Don't panic, this is all being drawn (only once)
	// to a bitmap.  The result is then BitBlt'd to the control whenever needed.
	int i;
	int nCharacters;
	int nTopGridPix, nMidGridPix, nBottomGridPix;

	CPen *oldPen;
	CPen solidPen(PS_SOLID, 0, m_crGridColor);
	CFont axisFont, yUnitFont, *oldFont;
	CString strTemp;

	// in case we haven't established the memory dc's
	CClientDC dc(this);

	// if we don't have one yet, set up a memory dc for the grid
	if (m_dcGrid.GetSafeHdc() == NULL)
	{
		m_dcGrid.CreateCompatibleDC(&dc);
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
		m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid);
	}

	m_dcGrid.SetBkColor(m_crBackColor);

	// fill the grid background
	m_dcGrid.FillRect(m_rectClient, &m_brushBack);

	// draw the plot rectangle:
	// determine how wide the y axis scaling values are
	nCharacters = abs((int)log10(fabs(m_dUpperLimit)));
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit))));

	// add the units digit, decimal point and a minus sign, and an extra space
	// as well as the number of decimal places to display
	nCharacters = nCharacters + 4 + m_nYDecimals;

	// adjust the plot rectangle dimensions
	// assume 6 pixels per character (this may need to be adjusted)
	m_rectPlot.left = m_rectClient.left + 6 * (nCharacters);
	m_nPlotWidth = m_rectPlot.Width();

	// draw the plot rectangle
	oldPen = m_dcGrid.SelectObject(&solidPen);
	m_dcGrid.MoveTo(m_rectPlot.left, m_rectPlot.top);
	m_dcGrid.LineTo(m_rectPlot.right + 1, m_rectPlot.top);
	m_dcGrid.LineTo(m_rectPlot.right + 1, m_rectPlot.bottom + 1);
	m_dcGrid.LineTo(m_rectPlot.left, m_rectPlot.bottom + 1);
	m_dcGrid.LineTo(m_rectPlot.left, m_rectPlot.top);
	m_dcGrid.SelectObject(oldPen);

	// draw the dotted lines, 
	// use SetPixel instead of a dotted pen - this allows for a 
	// finer dotted line and a more "technical" look
	nMidGridPix = (m_rectPlot.top + m_rectPlot.bottom) / 2;
	nTopGridPix = nMidGridPix - m_nPlotHeight / 4;
	nBottomGridPix = nMidGridPix + m_nPlotHeight / 4;

	for (i = m_rectPlot.left; i<m_rectPlot.right; i += 4)
	{
		m_dcGrid.SetPixel(i, nTopGridPix, m_crGridColor);
		m_dcGrid.SetPixel(i, nMidGridPix, m_crGridColor);
		m_dcGrid.SetPixel(i, nBottomGridPix, m_crGridColor);
	}

	// create some fonts (horizontal and vertical)
	// use a height of 14 pixels and 300 weight 
	// (these may need to be adjusted depending on the display)
	axisFont.CreateFont(14, 0, 0, 0, 300,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Arial");
	yUnitFont.CreateFont(14, 0, 900, 0, 300,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Arial");

	// grab the horizontal font
	oldFont = m_dcGrid.SelectObject(&axisFont);

	// y max
	m_dcGrid.SetTextColor(m_crGridColor);
	m_dcGrid.SetTextAlign(TA_RIGHT | TA_TOP);
	strTemp.Format("%.*lf", m_nYDecimals, m_dUpperLimit);
	m_dcGrid.TextOut(m_rectPlot.left - 4, m_rectPlot.top, strTemp);

	// y min
	m_dcGrid.SetTextAlign(TA_RIGHT | TA_BASELINE);
	strTemp.Format("%.*lf", m_nYDecimals, m_dLowerLimit);
	m_dcGrid.TextOut(m_rectPlot.left - 4, m_rectPlot.bottom, strTemp);

	// x min
	m_dcGrid.SetTextAlign(TA_LEFT | TA_TOP);
	m_dcGrid.TextOut(m_rectPlot.left, m_rectPlot.bottom + 4, "0");

	// x max
	m_dcGrid.SetTextAlign(TA_RIGHT | TA_TOP);
	strTemp.Format("%d", m_nPlotWidth / m_nShiftPixels);
	m_dcGrid.TextOut(m_rectPlot.right, m_rectPlot.bottom + 4, strTemp);

	// x units
	m_dcGrid.SetTextAlign(TA_CENTER | TA_TOP);
	m_dcGrid.TextOut((m_rectPlot.left + m_rectPlot.right) / 2,
		m_rectPlot.bottom + 4, m_strXUnitsString);

	// restore the font
	m_dcGrid.SelectObject(oldFont);

	// y units
	oldFont = m_dcGrid.SelectObject(&yUnitFont);
	m_dcGrid.SetTextAlign(TA_CENTER | TA_BASELINE);
	m_dcGrid.TextOut((m_rectClient.left + m_rectPlot.left) / 2,
		(m_rectPlot.bottom + m_rectPlot.top) / 2, m_strYUnitsString);
	m_dcGrid.SelectObject(oldFont);

	// at this point we are done filling the the grid bitmap, 
	// no more drawing to this bitmap is needed until the setting are changed

	// if we don't have one yet, set up a memory dc for the plot
	if (m_dcPlot.GetSafeHdc() == NULL)
	{
		m_dcPlot.CreateCompatibleDC(&dc);
		m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
		m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot);
	}

	// make sure the plot bitmap is cleared
	m_dcPlot.SetBkColor(m_crBackColor);
	m_dcPlot.FillRect(m_rectClient, &m_brushBack);

	// finally, force the plot area to redraw
	InvalidateRect(m_rectClient);

} // InvalidateCtrl


  /////////////////////////////////////////////////////////////////////////////
double COscilloScope::AppendPoint(double dNewPoint[])
{
	// append a data point to the plot
	// return the previous point

	double dPrevious;

	dPrevious = m_dCurrentPosition[0];
	for (int iCh = 0; iCh < FLL_CH; iCh++) {
		m_dCurrentPosition[iCh] = dNewPoint[iCh];
	}

	//m_dCurrentPosition[1] = dNewPoint[1];
	DrawPoint();

	Invalidate();

	return dPrevious;

} // AppendPoint

  ////////////////////////////////////////////////////////////////////////////
void COscilloScope::OnPaint()
{
	CPaintDC dc(this);  // device context for painting
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* oldBitmap; // bitmap originally found in CMemDC

						// no real plotting work is performed here, 
						// just putting the existing bitmaps on the client

						// to avoid flicker, establish a memory dc, draw to it 
						// and then BitBlt it to the client
	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);

	if (memDC.GetSafeHdc() != NULL)
	{
		// first drop the grid on the memory dc
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,
			&m_dcGrid, 0, 0, SRCCOPY);
		// now add the plot on top as a "pattern" via SRCPAINT.
		// works well with dark background and a light plot
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,
			&m_dcPlot, 0, 0, SRCPAINT);  //SRCPAINT
										 // finally send the result to the display
		dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight,
			&memDC, 0, 0, SRCCOPY);
	}

	memDC.SelectObject(oldBitmap);

} // OnPaint

  /////////////////////////////////////////////////////////////////////////////
void COscilloScope::DrawPoint()
{
	// this does the work of "scrolling" the plot to the left
	// and appending a new data point all of the plotting is 
	// directed to the memory based bitmap associated with m_dcPlot
	// the will subsequently be BitBlt'd to the client in OnPaint

	int currX, prevX, currY[FLL_CH], prevY[FLL_CH];

	CPen *oldPen[FLL_CH];
	CRect rectCleanUp;

	if (m_dcPlot.GetSafeHdc() != NULL)
	{
		// shift the plot by BitBlt'ing it to itself 
		// note: the m_dcPlot covers the entire client
		//       but we only shift bitmap that is the size 
		//       of the plot rectangle
		// grab the right side of the plot (exluding m_nShiftPixels on the left)
		// move this grabbed bitmap to the left by m_nShiftPixels

		m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top + 1,
			m_nPlotWidth, m_nPlotHeight, &m_dcPlot,
			m_rectPlot.left + m_nShiftPixels, m_rectPlot.top + 1,
			SRCCOPY);

		// establish a rectangle over the right side of plot
		// which now needs to be cleaned up proir to adding the new point
		rectCleanUp = m_rectPlot;
		rectCleanUp.left = rectCleanUp.right - m_nShiftPixels;

		// fill the cleanup area with the background
		m_dcPlot.FillRect(rectCleanUp, &m_brushBack);

		// draw the next line segement

		// grab the plotting pen
		//oldPen[0] = m_dcPlot.SelectObject(&m_penPlot[0]) ;
		//oldPen[1] = m_dcPlot.SelectObject(&m_penPlot[1]);

		// move to the previous point
		prevX = m_rectPlot.right - m_nPlotShiftPixels;

		for (int iCh = 0; iCh < FLL_CH; iCh++) {
			oldPen[iCh] = m_dcPlot.SelectObject(&m_penPlot[iCh]);
			prevY[iCh] = m_rectPlot.bottom -
				(long)((m_dPreviousPosition[iCh] - m_dLowerLimit) * m_dVerticalFactor);
			m_dcPlot.MoveTo(prevX, prevY[iCh]);

			// draw to the current point
			currX = m_rectPlot.right - m_nHalfShiftPixels;
			currY[iCh] = m_rectPlot.bottom -
				(long)((m_dCurrentPosition[iCh] - m_dLowerLimit) * m_dVerticalFactor);
			m_dcPlot.LineTo(currX, currY[iCh]);

			// if the data leaks over the upper or lower plot boundaries
			// fill the upper and lower leakage with the background
			// this will facilitate clipping on an as needed basis
			// as opposed to always calling IntersectClipRect
			if ((prevY[iCh] <= m_rectPlot.top) || (currY[iCh] <= m_rectPlot.top))
				m_dcPlot.FillRect(CRect(prevX, m_rectClient.top, currX + 1, m_rectPlot.top + 1), &m_brushBack);
			if ((prevY[iCh] >= m_rectPlot.bottom) || (currY[iCh] >= m_rectPlot.bottom))
				m_dcPlot.FillRect(CRect(prevX, m_rectPlot.bottom + 1, currX + 1, m_rectClient.bottom + 1), &m_brushBack);

		}


		// restore the pen 
		m_dcPlot.SelectObject(oldPen[0]);
		
		for (int iCh = 0; iCh < FLL_CH; iCh++) {
			// store the current point for connection to the next point
			m_dPreviousPosition[iCh] = m_dCurrentPosition[iCh];
		}

		//m_dPreviousPosition[1] = m_dCurrentPosition[1];
	}

} // end DrawPoint

  /////////////////////////////////////////////////////////////////////////////
void COscilloScope::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// NOTE: OnSize automatically gets called during the setup of the control

	GetClientRect(m_rectClient);

	// set some member variables to avoid multiple function calls
	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth = m_rectClient.Width();

	// the "left" coordinate and "width" will be modified in 
	// InvalidateCtrl to be based on the width of the y axis scaling
	m_rectPlot.left = 20;
	m_rectPlot.top = 10;
	m_rectPlot.right = m_rectClient.right - 10;
	m_rectPlot.bottom = m_rectClient.bottom - 25;

	// set some member variables to avoid multiple function calls
	m_nPlotHeight = m_rectPlot.Height();
	m_nPlotWidth = m_rectPlot.Width();

	// set the scaling factor for now, this can be adjusted 
	// in the SetRange functions
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange;

} // OnSize


  /////////////////////////////////////////////////////////////////////////////
void COscilloScope::Reset()
{
	// to clear the existing data (in the form of a bitmap)
	// simply invalidate the entire control
	InvalidateCtrl();
}
