
// BestFitDemoView.h : interface of the CBestFitDemoView class
//


#pragma once
#include "afxwin.h"

#include "PointsGrid.h"
#include "SolutionGrid.h"
#include "PlotWnd.h"

class CBestFitDemoView : public CFormView
{
	struct QueryBehaviour
		{
		bool askToAdd;
		bool askToRemove;
		int doAdd;
		int doRemove;
		QueryBehaviour() : askToAdd(true), askToRemove(true), doAdd(IDYES), doRemove(IDYES) {}
		};

	bool m_initialised;
	bool m_doAdjustmentOnComboSelection;
	QueryBehaviour m_queries;

	void SetComboWithoutAdjustment(int type);

protected: // create from serialization only
	CBestFitDemoView();
	DECLARE_DYNCREATE(CBestFitDemoView)

public:
	enum{ IDD = IDD_BESTFITDEMO_FORM };

// Attributes
public:
	CBestFitDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CBestFitDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_typeCombo;
	PointsGrid m_pointsGrid;
	SolutionGrid m_solutionGrid;
	PlotWnd m_plotWnd;
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnCbnSelchangeTypecombo();
	afx_msg LRESULT OnAddPoint(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemovePoint(WPARAM wParam, LPARAM lParam);
	};

#ifndef _DEBUG  // debug version in BestFitDemoView.cpp
inline CBestFitDemoDoc* CBestFitDemoView::GetDocument() const
   { return reinterpret_cast<CBestFitDemoDoc*>(m_pDocument); }
#endif

