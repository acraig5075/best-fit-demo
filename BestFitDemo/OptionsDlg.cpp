// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BestFitDemo.h"
#include "OptionsDlg.h"


// COptionsDlg dialog

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(OptionsSt &options, CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
  , m_options(options)
{

}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()


// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  ((CSpinButtonCtrl *)GetDlgItem(IDC_VERBOSITYSPIN))->SetRange(0, 10);
  ((CSpinButtonCtrl *)GetDlgItem(IDC_DECIMALSSPIN))->SetRange(1, 12);

  SetDlgItemInt(IDC_VERBOSITYEDIT, m_options.verbosity);
  SetDlgItemInt(IDC_DECIMALSEDIT, m_options.decimals);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDlg::OnOK()
{
  BOOL ok1, ok2;
  m_options.verbosity = (int)GetDlgItemInt(IDC_VERBOSITYEDIT, &ok1);
  m_options.decimals =  (int)GetDlgItemInt(IDC_DECIMALSEDIT, &ok2);

  if (!ok1)
    m_options.verbosity = 1;
  if (!ok2)
    m_options.decimals = 6;

  CDialog::OnOK();
}
