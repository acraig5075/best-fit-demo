#pragma once

#include "BestFitDemo.h"
#include "UGCtrl.h"
#include <vector>

class SolutionGrid : public CUGCtrl
	{
	public:
		SolutionGrid();
		~SolutionGrid(void);

		void OnSetup();
		COLORREF OnGetDefBackColor(int section);
		void Update(int type, IteratorRange solution);
	};
