#pragma once

#include "BestFitDemo.h"
#include "UGCtrl.h"
#include <vector>

class PointsGrid : public CUGCtrl
	{
	enum { ID_COLUMNSORTASC = WM_USER + 2000, ID_COLUMNSORTDESC, ID_REMOVEROW };

	public:
		PointsGrid();
		~PointsGrid(void);

		void OnSetup();
		COLORREF OnGetDefBackColor(int section);
		int FitToWindow(int startCol, int endCol);
		void Update(IteratorRange provisionals, IteratorRange adjusted, IteratorRange residuals, int decimals);
		int OnMenuStart(int col,long row,int section);
		void OnMenuCommand(int col, long row, int section, int item);

	};
