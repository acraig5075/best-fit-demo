#pragma once
#include "stdafx.h"
#include "BestFitDemo.h"

#include <vector>
#include <limits>

#undef max
#undef min

struct WorldScene
	{
	double minX, minY, maxX, maxY;
	CRect m_clientRect;
	double m_scale;

	WorldScene()
		{
		ResetToSmall(); // Todo: Is this needed?
		}

	void ResetToSmall()
		{
		minX = -1.0;
		maxX =  1.0;
		minY = -1.0;
		maxY =  1.0;
		m_scale = 1.0;
		m_clientRect = CRect();
		}

	void ResetToMassive()
		{
		minX = std::numeric_limits<double>::max();
		maxX = -minX;
		minY = minX;
		maxY = maxX;
		m_scale = 1.0;
		m_clientRect = CRect();
		}

	void UpdateLimitsFrom(IteratorRange range)
		{
		//int size = range.second - range.first;
		//if (size / 2 < 4)
		//	return;

		bool odd = true;
		std::vector<double>::const_iterator i;
		for (i = range.first; i != range.second; ++i, odd = !odd)
			{
			double &minVal = odd ? minX : minY;
			double &maxVal = odd ? maxX : maxY;

			if (*i < minVal)
				minVal = *i;
			if (*i > maxVal)
				maxVal = *i;
			}
		}

	void ExpandByPercent(double &minVal, double &maxVal, int percent)
		{
		double mean = (minVal + maxVal) / 2.0;
		double diff = (maxVal - minVal) * (100.0 + percent) / 200.0;
		minVal = mean - diff;
		maxVal = mean + diff;
		}

	void ExpandByPercent(int percent)
		{
		if (percent > 0)
			{
			ExpandByPercent(minX, maxX, percent);
			ExpandByPercent(minY, maxY, percent);
			}
		}

	void SetupMapping(const CRect &clientRect)
		{
		m_clientRect = clientRect;
		double clientAspect = double(clientRect.Height()) / clientRect.Width();
		double worldAspect = Aspect();

		if (worldAspect > clientAspect)
			m_scale = clientRect.Height() / Height();
		else
			m_scale = clientRect.Width() / Width();
		}

	CPoint WorldToPixel(double x, double y) const
		{
		double cx, cy;
		Centroid(cx, cy);

		CSize offset;
		offset.cx = static_cast<int>((x - cx) * m_scale);
		offset.cy = static_cast<int>((cy - y) * m_scale);
		return m_clientRect.CenterPoint() + offset;
		}

	void PixelToWorld(const CPoint &p, double &x, double &y) const
		{
		double cx, cy;
		Centroid(cx, cy);

		CSize offset = p - m_clientRect.CenterPoint();
		x = offset.cx / m_scale + cx;
		y = cy - offset.cy / m_scale;
		}

	double Width() const { return maxX - minX; }
	double Height() const { return maxY - minY; }
	double Aspect() const { return Height() / Width(); }
	void Centroid(double &x, double &y) const
		{
		x = (minX + maxX) / 2.0;
		y = (minY + maxY) / 2.0;
		}
	};
