
// ButtonOsciDlg.h : header file
//

#pragma once
#include "OscilloScope.h"
#include "OsciScale.h"
#include "afxcmn.h"

// CButtonOsciDlg dialog
class CButtonOsciDlg : public CDialog
{
// Construction
public:
	CButtonOsciDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTTONOSCI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	BOOL			m_bRandom;
	CButton			m_clMethod[2];
	int				m_intfrequency;

// Implementation
protected:
	HICON			m_hIcon;
	COscilloScope	m_OscilloSopeBtn;
	BOOL			m_bStartStop;

	double	  m_phase;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRunstop();
	afx_msg void OnTimer(UINT nIDEvent);
	//virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnClicked();
	afx_msg void OnUsrRandomRadio();
	afx_msg void OnUsrTriRadio();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderFrequency;
};
