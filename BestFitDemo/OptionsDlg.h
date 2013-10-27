#pragma once


// COptionsDlg dialog

struct OptionsSt
{
  int verbosity;
  int decimals;
  OptionsSt() : verbosity(1), decimals(6) {}
};

class COptionsDlg : public CDialog
{
  OptionsSt &m_options;

	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(OptionsSt &options, CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
protected:
  virtual void OnOK();
};
