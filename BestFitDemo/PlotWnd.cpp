// Plot.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\best-fit\BestFit.h"
#include "BestFitDemo.h"
#include "PlotWnd.h"
#include "WorldScene.h"

#include <cmath>
#include <assert.h>

// PlotWnd

IMPLEMENT_DYNAMIC(PlotWnd, CStatic)

const COLORREF PlotWnd::kBlack     = RGB(0,0,0);
const COLORREF PlotWnd::kBlue      = RGB(0,0,255);
const COLORREF PlotWnd::kRed       = RGB(255,0,0);
const COLORREF PlotWnd::kLightGrey = RGB(230,230,230);
const COLORREF PlotWnd::kDarkGrey  = RGB(200,200,200);

PlotWnd::PlotWnd(CView *parent)
	: m_parent(parent)
	 ,m_solutionType(0)
	, m_initialised(false)
{
}

PlotWnd::~PlotWnd()
{
}


BEGIN_MESSAGE_MAP(PlotWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// PlotWnd message handlers


void PlotWnd::InitialUpdate()
	{
	CRect rect;
	GetClientRect(rect);
	m_worldScene.ResetToSmall();
	m_worldScene.SetupMapping(rect);
	}

void PlotWnd::Update(int type, IteratorRange provisionals, IteratorRange adjusted, IteratorRange solution)
	{
	int numP = provisionals.second - provisionals.first;
	int numA = adjusted.second - adjusted.first;
	int numS = solution.second - solution.first;

	m_solutionType = type;
	m_provisionals = provisionals;
	m_adjusted = adjusted;
	m_solution = solution;
	m_initialised = (numP > 0);

	CRect rect;
	GetClientRect(rect);

	if (numA == 0)
		m_worldScene.ResetToSmall();
	else
		m_worldScene.ResetToMassive();

	m_worldScene.UpdateLimitsFrom(provisionals);
	m_worldScene.UpdateLimitsFrom(adjusted);
	m_worldScene.ExpandByPercent(5);
	m_worldScene.SetupMapping(rect);

	Invalidate();
	}

void PlotWnd::PlotPoints(CDC *pDC, IteratorRange points)
	{
	unsigned int numP = points.second - points.first;
	if (numP == 0 || numP % 2 != 0)
		return;

	std::vector<double>::const_iterator i, x, y;
	for (i = points.first; i != points.second; ++i)
		{
		x = i;
		y = ++i;
		PlotPoint(pDC, *x, *y);
		}
	}

void PlotWnd::PlotSolution(CDC *pDC, int type)
	{
	int numS = m_solution.second - m_solution.first;
	if (numS == 0)
		return;

	switch (type)
		{
		case 0: PlotLine(pDC); break;
		case 1: PlotCircle(pDC); break;
		case 2: PlotEllipse(pDC); break;
		default: break;
		}
	}

void PlotWnd::PlotPoint(CDC *pDC, double x, double y)
	{
	CPoint p = m_worldScene.WorldToPixel(x, y);

	pDC->MoveTo(p);
	pDC->LineTo(p + CSize(1,0));
	}

void PlotWnd::PlotLine(CDC *pDC)
	{
	double gradient  = *(m_solution.first + BestFitIO::LineGradient);
	double intercept = *(m_solution.first + BestFitIO::LineYIntercept);

	double y1 = gradient * m_worldScene.minX + intercept;
	double y2 = gradient * m_worldScene.maxX + intercept;
	CPoint p1  = m_worldScene.WorldToPixel(m_worldScene.minX, y1);
	CPoint p2  = m_worldScene.WorldToPixel(m_worldScene.maxX, y2);

	pDC->MoveTo(p1);
	pDC->LineTo(p2);
	}

void PlotWnd::PlotCircle(CDC *pDC)
	{
	double centreX = *(m_solution.first + BestFitIO::CircleCentreX);
	double centreY = *(m_solution.first + BestFitIO::CircleCentreY);
	double radius  = *(m_solution.first + BestFitIO::CircleRadius);

	CPoint upperLeft  = m_worldScene.WorldToPixel(centreX - radius, centreY - radius);
	CPoint lowerRight = m_worldScene.WorldToPixel(centreX + radius, centreY + radius);
	pDC->Ellipse(CRect(upperLeft, lowerRight));
	}

void PlotWnd::PlotEllipse(CDC *pDC)
	{
	double centreX  =  *(m_solution.first + BestFitIO::EllipseCentreX);
	double centreY  =  *(m_solution.first + BestFitIO::EllipseCentreY);
	double major    =  *(m_solution.first + BestFitIO::EllipseMajor);
	double minor    =  *(m_solution.first + BestFitIO::EllipseMinor);
	double rotation = -*(m_solution.first + BestFitIO::EllipseRotation);

	CPoint centerPt   = m_worldScene.WorldToPixel(centreX, centreY);
	CPoint upperLeft  = m_worldScene.WorldToPixel(centreX - major, centreY - minor);
	CPoint lowerRight = m_worldScene.WorldToPixel(centreX + major, centreY + minor);

	XFORM xform;
	xform.eM11 =  (float)cos(rotation);
	xform.eM12 =  (float)sin(rotation);
	xform.eM21 = -(float)sin(rotation);
	xform.eM22 =  (float)cos(rotation);
	xform.eDx = (float)(centerPt.x - cos(rotation)*centerPt.x + sin(rotation)*centerPt.y);
	xform.eDy = (float)(centerPt.y - cos(rotation)*centerPt.y - sin(rotation)*centerPt.x);

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->SetWorldTransform(&xform);
	pDC->Ellipse(CRect(upperLeft, lowerRight));

	XFORM normal;
	normal.eM11 = 1.0F;
	normal.eM12 = 0;
	normal.eM21 = 0;
	normal.eM22 = 1.0F;
	normal.eDx = 0;
	normal.eDy = 0;
	pDC->SetWorldTransform(&normal);
	pDC->SetGraphicsMode(oldMode);
	}

void PlotWnd::PlotGridLines(CDC *pDC, const CRect &client)
	{
	int midX = (client.left + client.right) / 2;
	int midY = (client.top + client.bottom) / 2;
	pDC->MoveTo(midX, 0);
	pDC->LineTo(midX, client.bottom);
	pDC->MoveTo(0, midY);
	pDC->LineTo(client.right, midY);

#if defined DRAW_GRID_LABELS

	double n, e, s, w;
	m_worldScene.PixelToWorld(client.TopLeft(), w, n);
	m_worldScene.PixelToWorld(client.BottomRight(), e, s);
	CString nlab, elab, slab, wlab;
	nlab.Format("%.1f", n);
	elab.Format("%.1f", e);
	slab.Format("%.1f", s);
	wlab.Format("%.1f", w);

	CRect textRect(client);
	COLORREF oldTextColour = pDC->SetTextColor(kDarkGrey);

	CFont *oldFont = pDC->GetCurrentFont();
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	pDC->SelectObject(&font);

	textRect.MoveToXY(midX, 0);
	pDC->DrawText(nlab, &textRect, DT_CALCRECT);
	textRect.OffsetRect(-textRect.Width(), 0);
	pDC->DrawText(nlab, &textRect, DT_SINGLELINE);

	textRect.MoveToXY(midX, client.bottom);
	pDC->DrawText(slab, &textRect, DT_CALCRECT);
	textRect.OffsetRect(-textRect.Width(), -textRect.Height());
	pDC->DrawText(slab, &textRect, DT_SINGLELINE);

	textRect.MoveToXY(0, midY);
	pDC->DrawText(wlab, &textRect, DT_CALCRECT);
	textRect.OffsetRect(0, 1);
	pDC->DrawText(wlab, &textRect, DT_SINGLELINE);

	textRect.MoveToXY(client.right, midY);
	pDC->DrawText(elab, &textRect, DT_CALCRECT);
	textRect.OffsetRect(-textRect.Width(), 1);
	pDC->DrawText(elab, &textRect, DT_SINGLELINE);

	m_worldScene.PixelToWorld(CPoint(midX, midY), e, n);
	CString lab;
	lab.Format("(%.1f,%.1f)", e, n);

	textRect.MoveToXY(midX, midY);
	pDC->DrawText(lab, &textRect, DT_CALCRECT);
	textRect.OffsetRect(1, -textRect.Height());
	pDC->DrawText(lab, &textRect, DT_SINGLELINE);

	font.DeleteObject();
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldTextColour);

#endif
	}

void PlotWnd::OnPaint()
	{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, dc.GetBkColor());

	//dc.DrawText("Hello world!", &textRect,  DT_SINGLELINE | DT_CALCRECT | DT_TOP);

	CRgn clipRgn;
	if (clipRgn.CreateRectRgnIndirect(rect))
		dc.SelectClipRgn(&clipRgn);

	CPen blackPen, redPen, bluePen, greyPen;
	blackPen.CreatePen(PS_SOLID, 1, kBlack);
	bluePen.CreatePen(PS_SOLID, 4, kBlue);
	redPen.CreatePen(PS_SOLID, 4, kRed);
	greyPen.CreatePen(PS_SOLID, 1, kLightGrey);

	CPen *oldPen = dc.SelectObject(&greyPen);
	PlotGridLines(&dc, rect);

	if (m_initialised)
		{
		dc.SelectObject(&redPen);
		PlotPoints(&dc, m_provisionals);

		dc.SelectObject(&blackPen);
		dc.SelectStockObject(NULL_BRUSH);
		PlotSolution(&dc, m_solutionType);

		dc.SelectObject(&bluePen);
		PlotPoints(&dc, m_adjusted);

		dc.SelectObject(oldPen);
		dc.SelectClipRgn(NULL);

		blackPen.DeleteObject();
		redPen.DeleteObject();
		bluePen.DeleteObject();
		}

	greyPen.DeleteObject();
	clipRgn.DeleteObject();
	}

void PlotWnd::OnLButtonDown(UINT nFlags, CPoint point)
	{
	if (m_parent)
		{
		double x, y;
		m_worldScene.PixelToWorld(point, x, y);
		GetParent()->SendMessage(USERMESSAGE_ADDPOINT, (WPARAM)(&x), (LPARAM)(&y));
		}

	CStatic::OnLButtonDown(nFlags, point);
	}

void PlotWnd::OnRButtonDown(UINT nFlags, CPoint point)
	{
	if (m_parent)
		{
		double x, y;
		m_worldScene.PixelToWorld(point, x, y);
		GetParent()->SendMessage(USERMESSAGE_REMPOINT, (WPARAM)(&x), (LPARAM)(&y));
		}

	CStatic::OnRButtonDown(nFlags, point);
	}
