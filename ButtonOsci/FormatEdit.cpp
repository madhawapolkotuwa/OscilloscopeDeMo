// FormatEdit.cpp : implementation file
//

#include "stdafx.h"
#include "FormatEdit.h"


// CFormatEdit

IMPLEMENT_DYNAMIC(CFormatEdit, CEdit)

CFormatEdit::CFormatEdit()
{

}

CFormatEdit::~CFormatEdit()
{
}


BEGIN_MESSAGE_MAP(CFormatEdit, CEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	/////////////////////////////////////////////////////////////////////////////
	//  Step value increase / decrease message
	//	argument	WPARAM	wParam
	//				LPARAM	lParam
	//	Return value TRUE
	ON_MESSAGE(WM_USR_STEP_VALUE, OnUsrStepValue)
END_MESSAGE_MAP()



// CFormatEdit message handlers

/////////////////////////////////////////////////////////////////////////////
//  Perform dynamic subclassing
//	argument	char*			cpConfig		Setting information pointer
//			    L_EDIT_INFO*	stpEditInfo 	Editing information
//			    CWnd*			stpParent		Parent window
//  No return value
void CFormatEdit::SubclassDlgItem( char* cpConfig, L_EDIT_INFO* stpEditInfo, CWnd* stpParent )
{
	// Save editing information
	m_cpConfig		= cpConfig ;
	m_stpEditInfo	= stpEditInfo ;
	m_stpParent	= stpParent ;

	// Subclass into a window
	CEdit::SubclassDlgItem( m_stpEditInfo->m_dwResourceID, stpParent ) ;

	// Display the configuration value
	ShowValue() ;
}

/////////////////////////////////////////////////////////////////////////////
// Display the set value
// No arguments
// No return value
void CFormatEdit::ShowValue()
{
	CString	strValue;	// Display string

						// Float type
	if (m_stpEditInfo->m_wForm == _FLOAT) {
		// Take out the corresponding data
		float* fpValue = (float *)(m_cpConfig + m_stpEditInfo->m_lOffset);

		// Determine the display format
		CString strForm;
		strForm.Format("%%.%df", m_stpEditInfo->m_wDecimal);
		strValue.Format(strForm, *fpValue);
	}
	// Double type
	else if (m_stpEditInfo->m_wForm == _DOUBLE) {
		// Take out the corresponding data
		double* dpValue = (double *)(m_cpConfig + m_stpEditInfo->m_lOffset);

		// Determine the display format
		CString strForm;
		strForm.Format("%%.%df", m_stpEditInfo->m_wDecimal);
		strValue.Format(strForm, *dpValue);
	}
	// WORD type
	else if (m_stpEditInfo->m_wForm == _WORD) {
		// Take out the corresponding data
		WORD* wpValue = (WORD *)(m_cpConfig + m_stpEditInfo->m_lOffset);

		// Determine the display format
		strValue.Format("%d", *wpValue);
	}
	// DWORD type
	else {
		// Take out the corresponding data
		DWORD* dwpValue = (DWORD *)(m_cpConfig + m_stpEditInfo->m_lOffset);

		// Determine the display format
		strValue.Format("%d", *dwpValue);
	}
	// Re -display
	SetWindowText(strValue);
}

/////////////////////////////////////////////////////////////////////////////
// It will be sent when the edit control loses the input focus
void CFormatEdit::OnKillfocus()
{
	// Get the input value
	CString	strValue	;
	GetWindowText( strValue ) ;

	// Convert the input value to numerical values
	double dInput = atof( strValue ) ;

	// Check the maximum input value
	if( dInput > m_stpEditInfo->m_dMaxValue ) {
		dInput = m_stpEditInfo->m_dMaxValue ;
	}
	// Check the minimum input value
	else if( dInput < m_stpEditInfo->m_dMinValue ) {
		dInput = m_stpEditInfo->m_dMinValue ;
	}
	// Float type
	if( m_stpEditInfo->m_wForm == _FLOAT ) {
		// Check the input range
		dInput = GetCheckValue( dInput ) ;

		// Take out the corresponding data
		float* fpValue = (float *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Register with the body
		*fpValue = (float)dInput ;
	}
	// double Type
	else if( m_stpEditInfo->m_wForm == _DOUBLE ) {
		// Check the input range
		dInput = GetCheckValue( dInput ) ;

		// Take out the corresponding data
		double* dpValue = (double *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Register with the body
		*dpValue = dInput ;
	}
	// WORD Type
	else if( m_stpEditInfo->m_wForm == _WORD ) {
		// Check the input range
		WORD wInput = GetCheckValue( (WORD)dInput ) ;

		// Take out the corresponding data
		WORD* wpValue = (WORD *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Register with the body
		*wpValue = wInput ;
	}

	// DWORD type
	else {
		// Check the input range
		DWORD dwInput = GetCheckValue( (DWORD)dInput ) ;

		// Take out the corresponding data
		DWORD* dwpValue = (DWORD *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Register with the body
		*dwpValue = dwInput ;
	}

}

/////////////////////////////////////////////////////////////////////////////
// Perform a range check
// Argument double dValue Current value
// Value after return value check
double CFormatEdit::GetCheckValue( double dValue )
{
	// Check the maximum input value
	if( dValue > m_stpEditInfo->m_dMaxValue ) {
		dValue = m_stpEditInfo->m_dMaxValue ;
	}
	// Check the minimum input value
	else if( dValue < m_stpEditInfo->m_dMinValue ) {
		dValue = m_stpEditInfo->m_dMinValue ;
	}
	return( dValue ) ;
}

/////////////////////////////////////////////////////////////////////////////
// Perform a range check
// Argument WORD wValue Current value
// Value after return value check
WORD CFormatEdit::GetCheckValue( WORD wValue )
{
	// Check the maximum input value
	if( wValue > (WORD)m_stpEditInfo->m_dMaxValue ) {
		wValue = (WORD)m_stpEditInfo->m_dMaxValue ;
	}
	// Check the minimum input value
	else if( wValue < (WORD)m_stpEditInfo->m_dMinValue ) {
		wValue = (WORD)m_stpEditInfo->m_dMinValue ;
	}
	return( wValue ) ;
}

/////////////////////////////////////////////////////////////////////////////
// Perform a range check
// Argument DWORD dwValue Current value
// Value after return value check
DWORD CFormatEdit::GetCheckValue( DWORD dwValue )
{
	// Check the maximum input value
	if( dwValue > (DWORD)m_stpEditInfo->m_dMaxValue ) {
		dwValue = (DWORD)m_stpEditInfo->m_dMaxValue ;
	}
	// Check the minimum input value
	else if( dwValue < (DWORD)m_stpEditInfo->m_dMinValue ) {
		dwValue = (DWORD)m_stpEditInfo->m_dMinValue ;
	}
	return( dwValue ) ;
}


/////////////////////////////////////////////////////////////////////////////
// Perform a range check
// Argument short sValue Current value
// Value after return value check
short CFormatEdit::GetCheckValue( short sValue )
{
	// Check the maximum input value
	if( sValue > (short)m_stpEditInfo->m_dMaxValue )
	{
		sValue = (short)m_stpEditInfo->m_dMaxValue;
	}
	// Check the minimum input value
	else if( sValue < (short)m_stpEditInfo->m_dMinValue )
	{
		sValue = (short)m_stpEditInfo->m_dMinValue;
	}
	return( sValue ) ;
}


/////////////////////////////////////////////////////////////////////////////
// Step value increase / decrease message
//	argument	WPARAM	wParam
//				LPARAM	lParam
//	Return value TRUE
long CFormatEdit::OnUsrStepValue( WPARAM /*wParam*/, LPARAM lParam )
{
	// Float type
	if( m_stpEditInfo->m_wForm == _FLOAT ) {
		// Take out the corresponding data
		float* fpValue = (float *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Add the step amount
		float fNewValue	;
		if( (BOOL)lParam == TRUE ) {
			fNewValue = *fpValue + (float)m_stpEditInfo->m_dStep ;
		}
		// Subtract the step amount
		else {
			fNewValue = *fpValue - (float)m_stpEditInfo->m_dStep ;
		}
		// Check the input range
		fNewValue = (float)GetCheckValue( fNewValue ) ;

		// Register with the body
		*fpValue = fNewValue ;
	}
	// Double type
	else if( m_stpEditInfo->m_wForm == _DOUBLE ) {
		// Take out the corresponding data
		double* dpValue = (double *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Add the step amount
		double dNewValue	;
		if( (BOOL)lParam == TRUE ) {
			dNewValue = *dpValue + m_stpEditInfo->m_dStep ;
		}
		// Subtract the step amount
		else {
			dNewValue = *dpValue - m_stpEditInfo->m_dStep ;
		}
		// Check the input range
		dNewValue = GetCheckValue( dNewValue ) ;

		// Register with the body
		*dpValue = dNewValue ;
	}

	// Word type
	else {
		// Take out the corresponding data
		WORD* wpValue = (WORD *)( m_cpConfig + m_stpEditInfo->m_lOffset ) ;

		// Add the step amount
		DWORD dwNewValue	;
		if( (BOOL)lParam == TRUE ) {
			dwNewValue = (WORD)*wpValue + (WORD)m_stpEditInfo->m_dStep ;
		}
		// Subtract the step amount
		else {
			if( 0 < ( *wpValue - (WORD)m_stpEditInfo->m_dStep ))
			{
				dwNewValue = *wpValue - (WORD)m_stpEditInfo->m_dStep ;
			}
			else
			{
				dwNewValue = 0;
			}

		}

		// Check the input range
		WORD wNewValue = GetCheckValue( (WORD)dwNewValue ) ;

		// Register with the body
		*wpValue = wNewValue ;
	}
	// Re -display
	ShowValue() ;

	// Set the focus
	SetFocus() ;
	SetSel( 0, -1 ) ;
	return( TRUE ) ;
}


/////////////////////////////////////////////////////////////////////////////
// Display the set value
// No arguments
// No return value
void CFormatEdit::ShowUpdate()
{
	// Re -display
	ShowValue();
}
