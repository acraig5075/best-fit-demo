
// BestFitDemoDoc.cpp : implementation of the CBestFitDemoDoc class
//

#include "stdafx.h"
#include "BestFitDemo.h"
#include "BestFitDemoDoc.h"
#include "WorldScene.h"
#include "..\..\best-fit\BestFit.h"
#include "MainFrm.h"
#include "TestPointsDlg.h"
#include "OptionsDlg.h"
#include "..\..\test-points\TestPoints.h"

#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#undef max // interferes with std::numeric_limit::max

// CBestFitDemoDoc

IMPLEMENT_DYNCREATE(CBestFitDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CBestFitDemoDoc, CDocument)
	ON_COMMAND(ID_GENERATE_TESTLINE, &CBestFitDemoDoc::OnGenerateTestline)
	ON_COMMAND(ID_GENERATE_TESTCIRCLE, &CBestFitDemoDoc::OnGenerateTestcircle)
	ON_COMMAND(ID_GENERATE_TESTELLIPSE, &CBestFitDemoDoc::OnGenerateTestellipse)
  ON_COMMAND(ID_TOOLS_OPTIONS, &CBestFitDemoDoc::OnToolsOptions)
END_MESSAGE_MAP()


// CBestFitDemoDoc construction/destruction

CBestFitDemoDoc::CBestFitDemoDoc()
	: m_adjustmentType(0)
{
	// TODO: add one-time construction code here

}

CBestFitDemoDoc::~CBestFitDemoDoc()
{
}

BOOL CBestFitDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_input.clear();
	m_output.clear();
	m_solution.clear();
	m_residuals.clear();

	ClearOutputBar();
	UpdateAllViews(NULL);

	return TRUE;
}




// CBestFitDemoDoc serialization

void CBestFitDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CBestFitDemoDoc diagnostics

#ifdef _DEBUG
void CBestFitDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBestFitDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBestFitDemoDoc commands

BOOL CBestFitDemoDoc::OnOpenDocument(LPCTSTR lpszPathName)
	{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (!FileIsValidFormat(lpszPathName))
		{
		LPCTSTR msg = "File is not in the expected format:\n\n" \
			"First line: <Number of points to follow>\n" \
			"Remaining lines: <x>,<y>\n\n" \
			"An initial # will ignore the line.";
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
		return FALSE;
		}

	m_input.clear();
	ParseFile(lpszPathName);
	DoAdjustment();

	return TRUE;
	}

bool CBestFitDemoDoc::FileIsValidFormat(const std::string &filename)
	{
	bool first = true;
	bool valid = true;
	int count;
	double x, y;
	char comma;

	std::ifstream fin(filename.c_str(), std::ios::in);
	while (fin.good() && valid)
		{
		std::string line;
		std::getline(fin, line);

		if (!line.empty() && line[0] != '#')
			{
			std::stringstream ss(line);
			if (first)
				{
				if ((ss >> count) == 0)
					valid = false;
				}
			else
				{
				if ((ss >> x >> comma >> y) == 0)
					valid = false;
				}
			}
		first = false;
		}
	fin.close();
	return valid;
	}

void CBestFitDemoDoc::ParseFile(const std::string &filename)
	{
	std::ifstream fin(filename.c_str(), std::ios::in);
	ParseStream(fin);
	}

void CBestFitDemoDoc::ParseStream(std::istream &is)
	{
	if (is.good())
		{
		std::string line;
		std::getline(is, line);
		std::stringstream ss(line);

		int count;
		char comma;
		double x, y;
		ss >> count;

		while (is.good() && count-- > 0)
			{
			std::getline(is, line);
			std::stringstream ss2(line);
			if (line[0] != '#')
				{
				ss2 >> x >> comma >> y;
				m_input.push_back(x);
				m_input.push_back(y);
				}
			}
		}
	}

BOOL CBestFitDemoDoc::OnSaveDocument(LPCTSTR lpszPathName)
	{
	if (!m_input.empty())
		{
		std::ofstream of(lpszPathName);

		int count = m_input.size() / 2;
		of << count << "\n";

		std::vector<double>::const_iterator itr = m_input.begin();
		for (int i = 0; i < count; ++i)
			{
			double x = *itr;
			++itr;
			double y = *itr;
			++itr;
			of << x << "," << y << "\n";
			}

		of.close();
		return 1;
		}

	return 0;
	//return CDocument::OnSaveDocument(lpszPathName);
	}

void CBestFitDemoDoc::DoAdjustment()
	{
	CWaitCursor wcsr;

	m_output.clear();
	m_solution.clear();
	m_residuals.clear();

	int bufsize = m_input.size();
	int count = bufsize / 2;
	if (count == 0)
		return;

	double *outbuf = new double[bufsize];

	BestFitIO input;
	input.numPoints = count;
	input.points = &m_input[0];
	input.verbosity = m_options.verbosity;

	BestFitIO output;
	output.points = outbuf;
	output.wantAdjustedObs = true;
	output.wantResiduals = true;

	std::stringstream ss;

	BestFit *b = BestFitFactory::Create(m_adjustmentType, ss);
	if (b->Compute(input, output))
		{
		m_output.insert(m_output.end(), &outbuf[0], &outbuf[bufsize]);

		m_solution.insert(m_solution.end(), &output.outputFields[0], &output.outputFields[output.numOutputFields]);

		m_residuals.insert(m_residuals.end(), &output.residuals[0], &output.residuals[count]);
		}

	delete [] outbuf;
	if (output.wantResiduals)
		delete [] output.residuals;
	delete b;

	AppendToOutputBar(ss.str());

	UpdateAllViews(NULL);
	}

struct OutputFunctor
	{
	CMainFrame *m_mainFrame;
	OutputFunctor(CMainFrame *wnd)
		: m_mainFrame(wnd)
		{
		std::string demarcation(80, '-');
		m_mainFrame->AppendToOutputBar(demarcation.c_str());
		}
	void operator() (const std::string &text)
		{
		m_mainFrame->AppendToOutputBar(text.c_str());
		}
	};

void CBestFitDemoDoc::ClearOutputBar() const
	{
	CMainFrame *mainFrame = (CMainFrame *)AfxGetMainWnd();
	if (mainFrame)
		mainFrame->ClearOutputBar();
	}

void CBestFitDemoDoc::AppendToOutputBar(const std::string &text) const
	{
	CMainFrame *mainFrame = (CMainFrame *)AfxGetMainWnd();
	if (mainFrame)
		{
		std::vector<std::string> tokens = Split(text, '\n');
		
		if (tokens.size() > 0)
			{
			OutputFunctor LogToOutputBar(mainFrame);
			std::for_each(tokens.begin(), tokens.end(), LogToOutputBar);
			}
		}
	}

std::vector<std::string> CBestFitDemoDoc::Split(const std::string &text, char delim) const
	{
	std::vector<std::string> tokens;

	std::stringstream ss(text);
	std::string token;
	while (std::getline(ss, token, delim))
		tokens.push_back(token);

	return tokens;
	}

IteratorRange CBestFitDemoDoc::GetIteratorRangeOfProvisionals() const
	{
	return std::make_pair(m_input.begin(), m_input.end());
	}

IteratorRange CBestFitDemoDoc::GetIteratorRangeOfAdjusted() const
	{
	return std::make_pair(m_output.begin(), m_output.end());
	}

IteratorRange CBestFitDemoDoc::GetIteratorRangeOfSolution() const
	{
	return std::make_pair(m_solution.begin(), m_solution.end());
	}

IteratorRange CBestFitDemoDoc::GetIteratorRangeOfResiduals() const
	{
	return std::make_pair(m_residuals.begin(), m_residuals.end());
	}

int CBestFitDemoDoc::GetAdjustmentType() const
	{
	return m_adjustmentType;
	}

void CBestFitDemoDoc::SetAdjustmentType(int type)
	{
	m_adjustmentType = type;
	}

int CBestFitDemoDoc::GetNumberDecimals() const
	{
	return m_options.decimals;
	}

void CBestFitDemoDoc::AddPoint(double x, double y)
	{
	m_input.push_back(x);
	m_input.push_back(y);
	DoAdjustment();
	}

void CBestFitDemoDoc::RemovePoint(double x, double y)
	{
	std::vector<double>::iterator i = FindNearestPoint(x, y);
	if (i != m_input.end())
		{
		std::vector<double>::iterator xitr = i;
		std::vector<double>::iterator yitr = ++i;

		int xidx = i - m_input.begin();
		assert(xidx % 2 == 1);
		assert(yitr != m_input.end());

		m_input.erase(xitr, ++yitr);

		DoAdjustment();
		}
	}

std::vector<double>::iterator CBestFitDemoDoc::FindNearestPoint(double x, double y)
	{
	if (m_input.empty() || m_input.size() % 2 == 1)
		return m_input.end();

	double distance = std::numeric_limits<double>::max();

	std::vector<double>::iterator xitr = m_input.begin();
	std::vector<double>::iterator ret = m_input.end();

	while (xitr != m_input.end())
		{
		std::vector<double>::iterator yitr = xitr + 1;

		double x1 = *xitr;
		double y1 = *yitr;
		double d = _hypot(x1 - x, y1 - y);

		if (Double::IsLessThan(d, distance))
			{
			distance = d;
			ret = xitr;
			}

		xitr = ++yitr;
		}

	return ret;
	}

void CBestFitDemoDoc::OnGenerateTestline()
	{
	GenerateTestPoints(0);
	}

void CBestFitDemoDoc::OnGenerateTestcircle()
	{
	GenerateTestPoints(1);
	}

void CBestFitDemoDoc::OnGenerateTestellipse()
	{
	GenerateTestPoints(2);
	}

void CBestFitDemoDoc::GenerateTestPoints(int type)
	{
	TestPointsSt io;
	io.type = type;

	CTestPointsDlg dlg(io, NULL);
	if (IDOK == dlg.DoModal())
		{
		if (OnNewDocument())
			{
			std::auto_ptr<TestPoints> tp;

			switch (io.type)
				{
				case 0:
					tp = std::auto_ptr<TestPoints>(new TestPointsLine(io.params[0], io.params[1]));
					break;
				case 1:
					tp = std::auto_ptr<TestPoints>(new TestPointsCircle(io.params[0], io.params[1], io.params[2]));
					break;
				case 2:
					tp = std::auto_ptr<TestPoints>(new TestPointsEllipse(io.params[0], io.params[1], io.params[2], io.params[3], io.params[4]));
					break;
				}

			std::stringstream ss;
			tp->SetError(io.error);
			tp->Generate(io.count, 10, ss);
			ParseStream(ss);

			SetAdjustmentType(io.type);
			DoAdjustment();
			}
		}
	}

void CBestFitDemoDoc::OnToolsOptions()
{
	COptionsDlg dlg(m_options, NULL);
	if (IDOK == dlg.DoModal())
	{
		DoAdjustment();
	}
}
