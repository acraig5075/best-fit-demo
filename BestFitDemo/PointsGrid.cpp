#include "StdAfx.h"
#include "PointsGrid.h"

PointsGrid::PointsGrid()
	{
	}

PointsGrid::~PointsGrid(void)
	{
	}

COLORREF PointsGrid::OnGetDefBackColor(int /*section*/)
{
	return GetSysColor(COLOR_APPWORKSPACE);
}

void PointsGrid::OnSetup()
	{
	SetNumberRows(1);
	SetNumberCols(5);
	QuickSetText(-1, 0, "1");
	QuickSetText(0, -1, "Prov. X");
	QuickSetText(1, -1, "Prov. Y");
	QuickSetText(2, -1, "Adj. X");
	QuickSetText(3, -1, "Adj. Y");
	QuickSetText(4, -1, "Residual");
	EnableMenu(TRUE);
	}

int PointsGrid::FitToWindow(int startCol, int endCol)
	{
	CRect rect;
	GetClientRect(rect);

	int numCols = GetNumberCols();
	int colWidth = rect.Width() / numCols;

	SetColWidth(0, colWidth);
	SetColWidth(1, colWidth);
	SetColWidth(2, colWidth);
	SetColWidth(3, colWidth);
	SetColWidth(4, rect.Width() - colWidth * (numCols - 1));

	return CUGCtrl::FitToWindow(startCol, endCol);
	}

void PointsGrid::Update(IteratorRange provisionals, IteratorRange adjusted, IteratorRange residuals, int decimals)
	{
	ResetAll();

	int pnum = provisionals.second - provisionals.first;
	int anum = adjusted.second - adjusted.first;
	int rnum = residuals.second - residuals.first;

	if (pnum == 0)
		{
		SetNumberRows(1);
		QuickSetText(-1, 0, "1");
		QuickSetText( 0, 0, "");
		QuickSetText( 1, 0, "");
		QuickSetText( 2, 0, "");
		QuickSetText( 3, 0, "");
		QuickSetText( 4, 0, "");
		}
	else
		{
		long rows = pnum / 2;
		SetNumberRows(rows);

		CString rowHdr;
		CUGCell cell;
		GetCell(0, 0, &cell);
		cell.SetNumberDecimals(decimals);

		std::vector<double>::const_iterator pvalue = provisionals.first;
		std::vector<double>::const_iterator avalue = adjusted.first;
		std::vector<double>::const_iterator rvalue = residuals.first;

		for (long r = 0; r < rows; ++r)
			{
			rowHdr.Format("%ld", r + 1);
			cell.SetText(rowHdr.GetString());
			cell.SetAlignment(UG_ALIGNCENTER);
			SetCell(-1, r, &cell);

			cell.SetNumber(*pvalue);
			cell.SetAlignment(UG_ALIGNRIGHT);
			SetCell(0, r, &cell);
			++pvalue;

			cell.SetNumber(*pvalue);
			SetCell(1, r, &cell);
			++pvalue;

			if (anum > 0)
				{
				cell.SetNumber(*avalue);
				SetCell(2, r, &cell);
				++avalue;

				cell.SetNumber(*avalue);
				SetCell(3, r, &cell);
				++avalue;
				}
			else
				{
				QuickSetText(2, r, "");
				QuickSetText(3, r, "");
				}

			if (rnum > 0)
				{
				cell.SetNumber(*rvalue);
				SetCell(4, r, &cell);
				++rvalue;
				}
			else
				QuickSetText(4, r, "");

			}
		}

	RedrawAll();
	}

int PointsGrid::OnMenuStart(int col, long row, int section)
	{
	EmptyMenu();

	switch (section)
		{
		case UG_TOPHEADING:
			if (0 <= col && col <= 4)
				{
				AddMenuItem(ID_COLUMNSORTASC, "Sort ascending");
				AddMenuItem(ID_COLUMNSORTDESC, "Sort descending");
				return TRUE;
				}
			break;
		case UG_SIDEHEADING:
		case UG_GRID:
			if (row < GetNumberRows())
				{
				AddMenuItem(ID_REMOVEROW, "Remove row from adjustment");
				return TRUE;
				}
			break;
		}

	return FALSE;
	}

void PointsGrid::OnMenuCommand(int col, long row, int section, int item)
	{
	switch (item)
		{
		case ID_COLUMNSORTASC:
			if (UG_SUCCESS == SortBy(col, UG_SORT_ASCENDING))
				RedrawAll();
			break;
		case ID_COLUMNSORTDESC:
			if (UG_SUCCESS == SortBy(col, UG_SORT_DESCENDING))
				RedrawAll();
			break;
		case ID_REMOVEROW:
			{
			CUGCell xcell, ycell;
			if (UG_SUCCESS == GetCell(0, row, &xcell) && UG_SUCCESS == GetCell(1, row, &ycell))
				{
				double x, y;
				if (UG_SUCCESS == xcell.GetNumber(&x) && UG_SUCCESS == ycell.GetNumber(&y))
					GetParent()->SendMessage(USERMESSAGE_REMPOINT, (WPARAM)(&x), (LPARAM)(&y));
				}
			}
			break;
		}
	}
