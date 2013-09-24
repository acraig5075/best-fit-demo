#pragma once

#include "BestFitDemo.h"
#include "WorldScene.h"

#include <vector>

// PlotWnd

class PlotWnd : public CStatic
{
	static const COLORREF kBlack;
	static const COLORREF kBlue;
	static const COLORREF kRed;
	static const COLORREF kLightGrey;
	static const COLORREF kDarkGrey;

	DECLARE_DYNAMIC(PlotWnd)

	CView *m_parent;
	int m_solutionType;
	IteratorRange m_provisionals;
	IteratorRange m_adjusted;
	IteratorRange m_solution;
	WorldScene m_worldScene;
	bool m_initialised;

public:
	PlotWnd(CView *parent);
	virtual ~PlotWnd();

	void InitialUpdate();
	void Update(int type, IteratorRange provisionals, IteratorRange adjusted, IteratorRange solution);

private:
	void PlotPoints(CDC *pDC, IteratorRange points);
	void PlotSolution(CDC *pDC, int type);
	void PlotPoint(CDC *pDC, double x, double y);
	void PlotLine(CDC *pDC);
	void PlotCircle(CDC *pDC);
	void PlotEllipse(CDC *pDC);
	void PlotGridLines(CDC *pDC, const CRect &client);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	};


