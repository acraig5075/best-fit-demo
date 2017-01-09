// TestPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BestFitDemo.h"
#include "TestPointsDlg.h"
#include <sstream>

// CTestPointsDlg dialog

IMPLEMENT_DYNAMIC(CTestPointsDlg, CDialog)

CTestPointsDlg::CTestPointsDlg(TestPointsSt &io, CWnd* pParent /*=NULL*/)
	: CDialog(CTestPointsDlg::IDD, pParent)
	, m_io(io)
{

}

CTestPointsDlg::~CTestPointsDlg()
{
}

void CTestPointsDlg::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Control(pDX, IDC_SLIDER, m_errorSlider);
	}


BEGIN_MESSAGE_MAP(CTestPointsDlg, CDialog)
	ON_BN_CLICKED(IDC_GENRADIO1, &CTestPointsDlg::OnBnClickedTypeRadio)
	ON_BN_CLICKED(IDC_GENRADIO2, &CTestPointsDlg::OnBnClickedTypeRadio)
	ON_BN_CLICKED(IDC_GENRADIO3, &CTestPointsDlg::OnBnClickedTypeRadio)
END_MESSAGE_MAP()


// CTestPointsDlg message handlers

void CTestPointsDlg::OnOK()
	{
	BOOL ok = FALSE;
	int count = (int)GetDlgItemInt(IDC_NUMPOINTSEDIT, &ok);
	if (!ok)
		return;

	CString param1, param2, param3, param4, param5;
	GetDlgItemText(IDC_PARAMETEREDIT1, param1);
	GetDlgItemText(IDC_PARAMETEREDIT2, param2);
	GetDlgItemText(IDC_PARAMETEREDIT3, param3);
	GetDlgItemText(IDC_PARAMETEREDIT4, param4);
	GetDlgItemText(IDC_PARAMETEREDIT5, param5);

	std::stringstream ss1(param1.GetString());
	std::stringstream ss2(param2.GetString());
	std::stringstream ss3(param3.GetString());
	std::stringstream ss4(param4.GetString());
	std::stringstream ss5(param5.GetString());

	double v1, v2, v3, v4, v5;
	if (!(ss1 >> v1))
		return;
	if (!(ss2 >> v2))
		return;
	if (!(ss3 >> v3))
		return;
	if (!(ss4 >> v4))
		return;
	if (!(ss5 >> v5))
		return;

	m_io.type = GetCheckedRadioButton(IDC_GENRADIO1, IDC_GENRADIO3) - IDC_GENRADIO1;
	m_io.count = count;
	m_io.params[0] = v1;
	m_io.params[1] = v2;
	m_io.params[2] = v3;
	m_io.params[3] = v4;
	m_io.params[4] = v5;

	CDialog::OnOK();
	}

BOOL CTestPointsDlg::OnInitDialog()
	{
	CDialog::OnInitDialog();

	CheckRadioButton(IDC_GENRADIO1, IDC_GENRADIO3, IDC_GENRADIO1 + m_io.type);
	SetDlgItemInt(IDC_NUMPOINTSEDIT, m_io.count);

	UINT nIDs[] = {
		IDC_PARAMETEREDIT1,
		IDC_PARAMETEREDIT2,
		IDC_PARAMETEREDIT3,
		IDC_PARAMETEREDIT4,
		IDC_PARAMETEREDIT5,
		0};

	for (int i = 0; nIDs[i]; ++i)
		{
		CString val;
		val.Format("%.6f", m_io.params[i]);
		SetDlgItemText(nIDs[i], val);
		}

	m_errorSlider.SetRange(0, 10);
	m_errorSlider.SetPos(m_io.error);

	OnBnClickedTypeRadio();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
	}

void CTestPointsDlg::OnBnClickedTypeRadio()
	{
	switch (GetCheckedRadioButton(IDC_GENRADIO1, IDC_GENRADIO3))
		{
		case IDC_GENRADIO1:
			UpdateControls(IDC_PARAMETERLABEL1, IDC_PARAMETEREDIT1, "Line gradient", TRUE);
			UpdateControls(IDC_PARAMETERLABEL2, IDC_PARAMETEREDIT2, "Line Y-intercept", TRUE);
			UpdateControls(IDC_PARAMETERLABEL3, IDC_PARAMETEREDIT3, "n/a", FALSE);
			UpdateControls(IDC_PARAMETERLABEL4, IDC_PARAMETEREDIT4, "n/a", FALSE);
			UpdateControls(IDC_PARAMETERLABEL5, IDC_PARAMETEREDIT5, "n/a", FALSE);
			break;
		case IDC_GENRADIO2:
			UpdateControls(IDC_PARAMETERLABEL1, IDC_PARAMETEREDIT1, "Circle centre X", TRUE);
			UpdateControls(IDC_PARAMETERLABEL2, IDC_PARAMETEREDIT2, "Circle centre Y", TRUE);
			UpdateControls(IDC_PARAMETERLABEL3, IDC_PARAMETEREDIT3, "Circle radius", TRUE);
			UpdateControls(IDC_PARAMETERLABEL4, IDC_PARAMETEREDIT4, "n/a", FALSE);
			UpdateControls(IDC_PARAMETERLABEL5, IDC_PARAMETEREDIT5, "n/a", FALSE);
			break;
		case IDC_GENRADIO3:
			UpdateControls(IDC_PARAMETERLABEL1, IDC_PARAMETEREDIT1, "Ellipse centre X", TRUE);
			UpdateControls(IDC_PARAMETERLABEL2, IDC_PARAMETEREDIT2, "Ellipse centre Y", TRUE);
			UpdateControls(IDC_PARAMETERLABEL3, IDC_PARAMETEREDIT3, "Ellipse major axis", TRUE);
			UpdateControls(IDC_PARAMETERLABEL4, IDC_PARAMETEREDIT4, "Ellipse minor axis", TRUE);
			UpdateControls(IDC_PARAMETERLABEL5, IDC_PARAMETEREDIT5, "Ellipse rotation (radians)", TRUE);
			break;
		}
	}

void CTestPointsDlg::UpdateControls(UINT labelID, UINT editID, CString label, BOOL enable)
	{
	CWnd *labelCtrl = GetDlgItem(labelID);
	CWnd *editCtrl = GetDlgItem(editID);
	if (labelCtrl && editCtrl)
		{
		labelCtrl->SetWindowText(label);
		labelCtrl->EnableWindow(enable);
		editCtrl->EnableWindow(enable);
		}
	}