
// ButtonOsciDlg.cpp : implementation file
// Author: Madhawa Heshan Polkotuwa

#include "stdafx.h"
#include "math.h"
#include "ButtonOsci.h"
#include "ButtonOsciDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// CButtonOsciDlg dialog

float	g_fOsciNowVoltMax = +10.0				;	// Voltage ( + )
float	g_fOsciNowVoltMin = -10.0				;	// Voltage ( - )
WORD	g_wOsciNowMaxX = 2						;   // X Scale

CButtonOsciDlg::CButtonOsciDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BUTTONOSCI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bStartStop = FALSE;
	m_bRandom = TRUE;
	m_intfrequency = 1;
	srand((unsigned)time(NULL));
}

void CButtonOsciDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RANDOM_DATA_RADIO, m_clMethod[0]);
	DDX_Control(pDX, IDC_USR_FREQUENCY_SLIDER, m_sliderFrequency);
}

BEGIN_MESSAGE_MAP(CButtonOsciDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_USR_OSCI_BUTTON, OnBtnClicked)
	ON_BN_CLICKED(IDC_USR_RUN_STOP_BUTTON, OnRunstop)

	ON_BN_CLICKED(IDC_RANDOM_DATA_RADIO, OnUsrRandomRadio)
	ON_BN_CLICKED(IDC_SIN_DATA_RADIO, OnUsrTriRadio)

	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CButtonOsciDlg message handlers

BOOL CButtonOsciDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_clMethod[0].SetCheck(TRUE);

	// Draw a graph sample
	HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 38, 18, 0);
	CStatic* stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH1);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH2);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON3), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH3);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH4);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON5), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH5);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON6), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH6);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON7), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH7);
	stpWnd->SetIcon(hIcon);

	hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON8), IMAGE_ICON, 38, 18, 0);
	stpWnd = (CStatic *)GetDlgItem(IDC_STATIC_ICON_CH8);
	stpWnd->SetIcon(hIcon);

	m_sliderFrequency.SetRange(1, 20, FALSE);
	m_sliderFrequency.SetPos(1);

	// TODO: Add extra initialization here
	CRect rect;
	GetDlgItem(IDC_USR_OSCI_BUTTON)->GetWindowRect(rect);
	ScreenToClient(rect);

	// create the control
	m_OscilloSopeBtn.Create(WS_VISIBLE | WS_CHILD, rect, this);

	// customize the control
	m_OscilloSopeBtn.SetRange(-10.0, 10.0, 2,2);
	m_OscilloSopeBtn.SetYUnits("Volts");
	m_OscilloSopeBtn.SetXUnits("Samples (Windows Timer: 10 msec)");
	m_OscilloSopeBtn.SetBackgroundColor(RGB(0, 0, 0));
	m_OscilloSopeBtn.SetGridColor(RGB(192, 192, 255));
	m_OscilloSopeBtn.SetPlotColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CButtonOsciDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CButtonOsciDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CButtonOsciDlg::OnTimer(UINT nIDEvent)
{
	double nRandom[FLL_CH];

	if (m_bRandom == TRUE) {
		// generate a random number 
		for (int iCh = 0; iCh < FLL_CH; iCh++) {
			double randomPhase = 2 * M_PI * rand() / (double)RAND_MAX;
			nRandom[iCh] = 5 * sin(2 * M_PI * 1 * iCh / 100 + randomPhase);;
		}
	}
	else {
		m_phase += 2 * M_PI * m_intfrequency * 0.01;
		nRandom[0] = 2 * sin(m_phase);
		nRandom[1] = 2 * cos(m_phase);
		nRandom[2] = 0.5*(nRandom[0] + nRandom[1]);

		nRandom[3] = 2 + nRandom[0];
		nRandom[4] = 2 + nRandom[1];

		nRandom[5] = nRandom[0] - nRandom[1];
		nRandom[6] = 0.5 + 0.5*nRandom[0];
		nRandom[7] = 0.5 + 0.5*nRandom[1];
	}

	// append the new value to the plot
	m_OscilloSopeBtn.AppendPoint(nRandom);


	CDialog::OnTimer(nIDEvent);
}



void CButtonOsciDlg::OnBtnClicked() {
	COsciScale clDialog(&g_fOsciNowVoltMin, &g_fOsciNowVoltMax, &g_wOsciNowMaxX);
	if (clDialog.DoModal() == IDOK) {
		m_OscilloSopeBtn.SetRange((double)g_fOsciNowVoltMin, (double)g_fOsciNowVoltMax, 2, (int)g_wOsciNowMaxX);
	}
	else {
	}
}

void CButtonOsciDlg::OnRunstop()
{
	// TODO: Add your control notification handler code here
	m_bStartStop ^= TRUE;

	if (m_bStartStop)
		SetTimer(1, 10, NULL);
	else
		KillTimer(1);

}

void CButtonOsciDlg::OnCancel()
{
	if (!m_bStartStop)
		KillTimer(1);

	CDialog::OnCancel();
}

void CButtonOsciDlg::OnUsrRandomRadio() {
	m_bRandom = TRUE;
}

void CButtonOsciDlg::OnUsrTriRadio() {
	m_bRandom = FALSE;
}

void CButtonOsciDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == GetDlgItem(IDC_USR_FREQUENCY_SLIDER))
	{
		// Handle the slider control change
		m_intfrequency = m_sliderFrequency.GetPos();  // Get the current position of the slider
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}