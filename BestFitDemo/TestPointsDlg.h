#pragma once
#include "afxcmn.h"


// CTestPointsDlg dialog

struct TestPointsSt
	{
	int type;
	int count;
	double params[5];
	int error;

	TestPointsSt() 
		: type(2)
		, count(20)
		, error(1)
		{
		memset(&params[0], 0, 5 * sizeof(double));
		params[0] = 0.0;
		params[1] = 100.0;
		params[2] = 50.0;
		params[3] = 25.0;
		params[4] = 0.5;
		}
	};

class CTestPointsDlg : public CDialog
{
	TestPointsSt &m_io;

	void UpdateControls(UINT labelID, UINT editID, CString label, BOOL enable);

	DECLARE_DYNAMIC(CTestPointsDlg)

public:
	CTestPointsDlg(TestPointsSt &io, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestPointsDlg();

// Dialog Data
	enum { IDD = IDD_TESTPOINTSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CSliderCtrl m_errorSlider;
	afx_msg void OnBnClickedTypeRadio();
	};
