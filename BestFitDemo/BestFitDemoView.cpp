
// BestFitDemoView.cpp : implementation of the CBestFitDemoView class
//

#include "stdafx.h"
#include "BestFitDemo.h"
#include "BestFitDemoDoc.h"
#include "BestFitDemoView.h"
#include "XMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBestFitDemoView

IMPLEMENT_DYNCREATE(CBestFitDemoView, CFormView)

BEGIN_MESSAGE_MAP(CBestFitDemoView, CFormView)
	ON_CBN_SELCHANGE(IDC_TYPECOMBO, &CBestFitDemoView::OnCbnSelchangeTypecombo)
	ON_MESSAGE(USERMESSAGE_ADDPOINT, &CBestFitDemoView::OnAddPoint)
	ON_MESSAGE(USERMESSAGE_REMPOINT, &CBestFitDemoView::OnRemovePoint)
END_MESSAGE_MAP()

// CBestFitDemoView construction/destruction

CBestFitDemoView::CBestFitDemoView()
	: CFormView(CBestFitDemoView::IDD)
	, m_initialised(false)
	, m_doAdjustmentOnComboSelection(true)
	, m_plotWnd(this)
{
	// TODO: add construction code here

}

CBestFitDemoView::~CBestFitDemoView()
{
}

void CBestFitDemoView::DoDataExchange(CDataExchange* pDX)
{
CFormView::DoDataExchange(pDX);
DDX_Control(pDX, IDC_TYPECOMBO, m_typeCombo);
DDX_Control(pDX, IDC_PLOT, m_plotWnd);
	}

BOOL CBestFitDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CBestFitDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	if (!m_initialised)
		{
		m_typeCombo.AddString("Line");
		m_typeCombo.AddString("Circle");
		m_typeCombo.AddString("Ellipse");
		m_typeCombo.SetCurSel(0);

		GetDocument()->SetAdjustmentType(0);

		m_pointsGrid.AttachGrid(this, IDC_POINTSGRID);
		m_solutionGrid.AttachGrid(this, IDC_SOLUTIONGRID);
		m_plotWnd.InitialUpdate();
		m_initialised = true;
		}
}

void CBestFitDemoView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBestFitDemoView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CBestFitDemoView diagnostics

#ifdef _DEBUG
void CBestFitDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBestFitDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBestFitDemoDoc* CBestFitDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBestFitDemoDoc)));
	return (CBestFitDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CBestFitDemoView message handlers

void CBestFitDemoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
	{
	if (m_initialised)
		{
		IteratorRange provisionals = GetDocument()->GetIteratorRangeOfProvisionals();
		IteratorRange adjusted     = GetDocument()->GetIteratorRangeOfAdjusted();
		IteratorRange solution     = GetDocument()->GetIteratorRangeOfSolution();
		IteratorRange residuals    = GetDocument()->GetIteratorRangeOfResiduals();

		int type = GetDocument()->GetAdjustmentType();
		SetComboWithoutAdjustment(type);

		int decimals = GetDocument()->GetNumberDecimals();

		m_pointsGrid.Update(provisionals, adjusted, residuals, decimals);

		m_solutionGrid.Update(type, solution, decimals);

		m_plotWnd.Update(type, provisionals, adjusted, solution);
		}
	}

void CBestFitDemoView::SetComboWithoutAdjustment(int type)
	{
	m_doAdjustmentOnComboSelection = false;
	m_typeCombo.SetCurSel(type);
	m_doAdjustmentOnComboSelection = true;
	}

void CBestFitDemoView::OnCbnSelchangeTypecombo()
	{
	GetDocument()->SetAdjustmentType(m_typeCombo.GetCurSel());

	if (m_doAdjustmentOnComboSelection)
		GetDocument()->DoAdjustment();
	}

afx_msg LRESULT CBestFitDemoView::OnAddPoint(WPARAM wParam, LPARAM lParam)
	{
	if (m_queries.askToAdd)
		m_queries.doAdd = XMessageBox(m_hWnd, "Add this clicked point to the adjustment?", "Add Observation", MB_YESNO | MB_ICONQUESTION | MB_DONOTASKAGAIN);

	if ((m_queries.doAdd & MB_DONOTASKAGAIN) == MB_DONOTASKAGAIN)
		m_queries.askToAdd = false;

	if ((m_queries.doAdd & 0xFF) == IDYES)
		GetDocument()->AddPoint(*(double *)wParam, *(double *)lParam);

	return 0;
	}

afx_msg LRESULT CBestFitDemoView::OnRemovePoint(WPARAM wParam, LPARAM lParam)
	{
	if (m_queries.askToRemove)
		m_queries.doRemove = XMessageBox(m_hWnd, "Remove the nearest point from the adjustment?", "Remove Observation", MB_YESNO | MB_ICONQUESTION | MB_DONOTASKAGAIN);

	if (m_queries.doRemove & MB_DONOTASKAGAIN)
		m_queries.askToRemove = false;

	if ((m_queries.doRemove & 0xFF) == IDYES)
		GetDocument()->RemovePoint(*(double *)wParam, *(double *)lParam);

	return 0;
	}
