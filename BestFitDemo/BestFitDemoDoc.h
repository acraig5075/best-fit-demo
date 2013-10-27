
// BestFitDemoDoc.h : interface of the CBestFitDemoDoc class
//


#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "OptionsDlg.h"


class CBestFitDemoDoc : public CDocument
{
	int m_adjustmentType;
	std::vector<double> m_input;
	std::vector<double> m_output;
	std::vector<double> m_solution;
	std::vector<double> m_residuals;
	std::string m_outputText;

	OptionsSt m_options;

	bool FileIsValidFormat(const std::string &filename);
	void ParseFile(const std::string &filename);
	void ParseStream(std::istream &is);
	void ClearOutputBar() const;
	void AppendToOutputBar(const std::string &text) const;
	void GenerateTestPoints(int type);

	std::vector<std::string> Split(const std::string &text, char delim) const;

	std::vector<double>::iterator FindNearestPoint(double x, double y);

protected: // create from serialization only
	CBestFitDemoDoc();
	DECLARE_DYNCREATE(CBestFitDemoDoc)

// Attributes
public:
	IteratorRange GetIteratorRangeOfProvisionals() const;
	IteratorRange GetIteratorRangeOfAdjusted() const;
	IteratorRange GetIteratorRangeOfSolution() const;
	IteratorRange GetIteratorRangeOfResiduals() const;

	void DoAdjustment();
	void AddPoint(double x, double y);
	void RemovePoint(double x, double y);

// Operations
public:
	int GetAdjustmentType() const;
	void SetAdjustmentType(int type);
	int GetNumberDecimals() const;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBestFitDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnGenerateTestline();
	afx_msg void OnGenerateTestcircle();
	afx_msg void OnGenerateTestellipse();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  afx_msg void OnToolsOptions();
};


