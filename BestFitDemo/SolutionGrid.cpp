#include "StdAfx.h"
#include "SolutionGrid.h"

#include <cassert>

SolutionGrid::SolutionGrid()
	{
	}

SolutionGrid::~SolutionGrid(void)
	{
	}

void SolutionGrid::OnSetup()
	{
	SetNumberRows(1);
	SetNumberCols(1);
	QuickSetText(-1, 0, "1");
	QuickSetText(0, -1, "Solution");
	SetColWidth(-1, 160);
	}

COLORREF SolutionGrid::OnGetDefBackColor(int /*section*/)
{
	return GetSysColor(COLOR_APPWORKSPACE);
}

void SolutionGrid::Update(int type, IteratorRange solution, int decimals)
	{
	ResetAll();

	int numSolutions = solution.second - solution.first;

	if (numSolutions == 0)
		{
		SetNumberRows(1);
		QuickSetText(-1, 0, "1");
		QuickSetText( 0, 0, "");
		}
	else
		{
		long rows = 0;
		std::vector<std::string> headers;

		switch (type)
			{
			case 0:
				{
				rows = 2;
				const char *buf[] = { "Line gradient", "Y-intercept" };
				headers.assign(buf, buf + 2);
				break;
				}
			case 1:
				{
				rows = 3;
				const char *buf[] = { "Centre X", "Centre Y", "Circle radius" };
				headers.assign(buf, buf + 3);
				break;
				}
			case 2:
				{
				rows = 5;
				const char *buf[] = { "Centre X", "Centre Y", "Major axis", "Minor axis", "Ellipse rotation (radians)" };
				headers.assign(buf, buf + 5);
				break;
				}
			}

		SetNumberRows(rows);
		assert(rows == numSolutions);

		CUGCell cell;
		GetCell(0, 0, &cell);
		cell.SetNumberDecimals(decimals);

		std::vector<double>::const_iterator value = solution.first;

		for (long r = 0; r < rows; ++r)
			{
			cell.SetText(headers.at(r).c_str());
			cell.SetAlignment(UG_ALIGNLEFT);
			SetCell(-1, r, &cell);

			cell.SetNumber(*value);
			cell.SetAlignment(UG_ALIGNRIGHT);
			SetCell(0, r, &cell);
			++value;
			}
		}

	RedrawAll();
	}
