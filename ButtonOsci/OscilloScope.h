#pragma once


// COscilloScope

class COscilloScope : public CButton
{
	DECLARE_DYNAMIC(COscilloScope)

public:
	COscilloScope();
	virtual ~COscilloScope();

public:

	void SetRange(double dLower, double dUpper, int nDecimalPlaces, int ShifPx);
	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);

	double AppendPoint(double dNewPoint[]);
	void SetBackgroundColor(COLORREF color);
	void InvalidateCtrl();
	void DrawPoint();
	void Reset();

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = NULL);

// Implementation
public:
	int m_nShiftPixels;          // amount to shift with each new point 
	int m_nYDecimals;

	CString m_strXUnitsString;
	CString m_strYUnitsString;

	COLORREF m_crBackColor;        // background color
	COLORREF m_crGridColor;        // grid color
	COLORREF m_crPlotColor;        // data color  

	double m_dCurrentPosition[FLL_CH];   // current position
	double m_dPreviousPosition[FLL_CH];  // previous position


protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

	int m_nHalfShiftPixels;
	int m_nPlotShiftPixels;
	int m_nClientHeight;
	int m_nClientWidth;
	int m_nPlotHeight;
	int m_nPlotWidth;

	double m_dLowerLimit;        // lower bounds
	double m_dUpperLimit;        // upper bounds
	double m_dRange;
	double m_dVerticalFactor;

	CRect  m_rectClient;
	CRect  m_rectPlot;
	CPen   m_penPlot[FLL_CH];
	CBrush m_brushBack;

	CDC     m_dcGrid;
	CDC     m_dcPlot;
	CBitmap *m_pbitmapOldGrid;
	CBitmap *m_pbitmapOldPlot;
	CBitmap m_bitmapGrid;
	CBitmap m_bitmapPlot;
};


