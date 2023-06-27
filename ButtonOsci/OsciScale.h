#pragma once
#include "FormatEdit.h"

// COsciScale dialog

// COsciScale dialog
#define _OSCI_SCALE_CTRL		3

const L_EDIT_INFO	g_staOsciScaleInfo[_OSCI_SCALE_CTRL] = {
	{ IDC_USR_Y_MAX_EDIT,	IDC_USR_Y_MAX_UP_BUTTON,	IDC_USR_Y_MAX_DOWN_BUTTON,	0,	0,	0,	-10.0,	  10.0,	  1.00,	 2,	0 },
	{ IDC_USR_Y_MIN_EDIT,	IDC_USR_Y_MIN_UP_BUTTON,	IDC_USR_Y_MIN_DOWN_BUTTON,	0,	0,	0,	-10.0,	  10.0,	  1.00,	 2,	0 },
	{ IDC_USR_X_EDIT,		IDC_USR_X_UP_BUTTON,		IDC_USR_X_DOWN_BUTTON,		2,	0,	0,	    2,	    10,	  2,	 0,	0 },
};

class COsciScale : public CDialog
{
	DECLARE_DYNAMIC(COsciScale)

public:
	COsciScale(float* fpMinY, float* fpMaxY, WORD* fpMaxX, CWnd* pParent = NULL);   // standard constructor
	virtual ~COsciScale();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USR_SCALE_DIALOG };
#endif
private:
	float*		m_fpMinY;	// Y -axis smallest value
	float*		m_fpMaxY;	// Y -axis maximum value
	WORD*		m_wpMaxX;	// X -axis maximum value
	float		m_fMinY;	// Y -axis smallest value
	float		m_fMaxY;	// Y -axis maximum value
	WORD		m_wMaxX;	// X -axis maximum value

	CFormatEdit	m_staEdit[_OSCI_SCALE_CTRL];

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnUsrScaleOkButton();
	afx_msg void OnUsrCancelButton();
	DECLARE_MESSAGE_MAP()
};
