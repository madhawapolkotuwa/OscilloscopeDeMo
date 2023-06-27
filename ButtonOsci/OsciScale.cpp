// OsciScale.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonOsci.h"
#include "OsciScale.h"
#include "afxdialogex.h"


// COsciScale dialog

IMPLEMENT_DYNAMIC(COsciScale, CDialog)

COsciScale::COsciScale(float* fpMinY, float* fpMaxY, WORD* fpMaxX, CWnd* pParent /*=NULL*/ )
	: CDialog(IDD_USR_SCALE_DIALOG, pParent)
{
	m_fpMinY = fpMinY;		// Y -axis smallest value
	m_fpMaxY = fpMaxY;		// Y -axis maximum value
	m_wpMaxX = fpMaxX;		// X -axis maximum value
	m_fMinY = *m_fpMinY;	// Y -axis smallest value
	m_fMaxY = *m_fpMaxY;	// Y -axis maximum value

	switch (*m_wpMaxX) {
	case 2:
		m_wMaxX = 10;
		break;
	case 4:
		m_wMaxX = 8;
		break;
	case 6:
		m_wMaxX = 6;
		break;
	case 8:
		m_wMaxX = 4;
		break;
	case 10:
		m_wMaxX = 2;
		break;
	default:
		m_wMaxX = 2;
	}
}

COsciScale::~COsciScale()
{
}

void COsciScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COsciScale, CDialog)
	ON_BN_CLICKED(IDC_USR_OSCI_SCALE_OK_BUTTON, OnUsrScaleOkButton)
	ON_BN_CLICKED(IDC_USR_SCALE_CANCEL_BUTTON, OnUsrCancelButton)
END_MESSAGE_MAP()


// COsciScale message handlers

BOOL COsciScale::OnInitDialog() {

	// Subclass edit items
	m_staEdit[0].SubclassDlgItem((char *)&m_fMaxY, (L_EDIT_INFO *)&g_staOsciScaleInfo[0], this);
	m_staEdit[1].SubclassDlgItem((char *)&m_fMinY, (L_EDIT_INFO *)&g_staOsciScaleInfo[1], this);
	m_staEdit[2].SubclassDlgItem((char *)&m_wMaxX, (L_EDIT_INFO *)&g_staOsciScaleInfo[2], this);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Created and called for a command message
BOOL COsciScale::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// Search for spin control button to increase or decrease the set value
	for (int iIndex = 0; iIndex < _OSCI_SCALE_CTRL; iIndex++) {
		// Spin control " +"
		if (g_staOsciScaleInfo[iIndex].m_dwResourceUP == nID) {
			GetDlgItem(g_staOsciScaleInfo[iIndex].m_dwResourceID)->SendMessage(WM_USR_STEP_VALUE, (WPARAM)0, (LPARAM)TRUE);
			break;
		}
		// Spin control " -"
		else if (g_staOsciScaleInfo[iIndex].m_dwResourceDN == nID) {
			GetDlgItem(g_staOsciScaleInfo[iIndex].m_dwResourceID)->SendMessage(WM_USR_STEP_VALUE, (WPARAM)0, (LPARAM)FALSE);
			break;
		}
	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


/////////////////////////////////////////////////////////////////////////////
// When the OK button is pressed
void COsciScale::OnUsrScaleOkButton()
{
	// Get the configuration data
	*m_fpMinY = m_fMinY;
	*m_fpMaxY = m_fMaxY;
	switch (m_wMaxX) {
	case 2:
		*m_wpMaxX = 10;
		break;
	case 4:
		*m_wpMaxX = 8;
		break;
	case 6:
		*m_wpMaxX = 6;
		break;
	case 8:
		*m_wpMaxX = 4;
		break;
	case 10:
		*m_wpMaxX = 2;
		break;
	default:
		*m_wpMaxX = 2;
	}
	//*m_wpMaxX = m_wMaxX;
	// End the dialog
	EndDialog(TRUE);
}



/////////////////////////////////////////////////////////////////////////////
// When the cancellation button is pressed
void COsciScale::OnUsrCancelButton()
{
	EndDialog(FALSE);
	return;
}