#pragma once


// CFormatEdit
/////////////////////////////////////////////////////////////////////////////
// Control type
#define _EDIT				(0)		// Edit box

/////////////////////////////////////////////////////////////////////////////
// Variable type
#define _FLOAT				(0)		// float type
#define _DOUBLE				(1)		// double type
#define _WORD				(2)		// WORD type
#define _DWORD				(3)		// DWORD type

#define WM_USR_STEP_VALUE   (550)
/////////////////////////////////////////////////////////////////////////////
// Editing information structure
typedef struct {
	DWORD		m_dwResourceID;	// Resource ID
	DWORD		m_dwResourceUP;	// Spin control button ID ( +)
	DWORD		m_dwResourceDN;	// Spin control button ID ( -)
	WORD		m_wForm;		// Variable type
	WORD		m_wCtrl;		// Control type
	long		m_lOffset;		// Setting information reference offset
	double		m_dMinValue;	// minimum value
	double		m_dMaxValue;	// Maximum value
	double		m_dStep;		// Step amount
	WORD		m_wDecimal;		// Number of decimal digits
	WORD		m_wRadioCheck;	// Radio button index / checkbox settings
	WORD		m_wCombo;		// Combo box display character type
}	L_EDIT_INFO;

class CFormatEdit : public CEdit
{
	DECLARE_DYNAMIC(CFormatEdit)

public:
	CFormatEdit();
	virtual ~CFormatEdit();
public:
	/////////////////////////////////////////////////////////////////////////////
	//  Dynamic subclassization
	//	argument	char*			cpConfig		Setting information pointer
	//				L_EDIT_INFO*	stpEditInfo	        Editing information
	//				CWnd*			stpParent		    Parent window
	//	No return value
	void	SubclassDlgItem(char* cpConfig, L_EDIT_INFO* stpEditInfo, CWnd* stpParent);

	void	ShowUpdate();	//Display the set value

private:
	/////////////////////////////////////////////////////////////////////////////
	// Display the set value
	// No arguments
	// No return value
	void	ShowValue()	;
	/////////////////////////////////////////////////////////////////////////////
	// Perform a range check
	// Argument double dValue Current value
	// Value after return value check
	double	GetCheckValue( double dValue )	;
	/////////////////////////////////////////////////////////////////////////////
	// Perform a range check
	// Argument WORD wValue Current value
	// Value after return value check
	WORD	GetCheckValue( WORD wValue )	;
	/////////////////////////////////////////////////////////////////////////////
	// Perform a range check
	// Argument DWORD dwValue Current value
	// Value after return value check
	DWORD	GetCheckValue( DWORD dwValue )	;

	/////////////////////////////////////////////////////////////////////////////
	// Perform a range check
	// Argument short sValue Current value
	// Value after return value check
	short GetCheckValue(short sValue);

	private:
	char*			m_cpConfig		;	// Setting information pointer
	L_EDIT_INFO*	m_stpEditInfo	;	// Editing information
	CWnd*			m_stpParent	;	// Parent window


protected:
	afx_msg void OnKillfocus();
	/////////////////////////////////////////////////////////////////////////////
	//  Step value increase / decrease message
	//	argument	WPARAM	wParam
	//			    LPARAM	lParam
	//	Return value TRUE
	afx_msg long OnUsrStepValue(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


