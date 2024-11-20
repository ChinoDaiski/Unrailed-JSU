// PathFInd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFInd.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "Define.h"
#include "Fucntion.h"



// CPathFInd ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFInd, CDialog)

CPathFInd::CPathFInd(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFInd::~CPathFInd()
{
	for_each(m_PathList.begin(), m_PathList.end(), CDeleteObj());
	m_PathList.clear();
}

void CPathFInd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFInd, CDialog)
END_MESSAGE_MAP()


// CPathFInd �޽��� ó�����Դϴ�.


void CPathFInd::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPathFInd::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	wofstream		fout;
	fout.open(L"../Data/ImgPath.txt");

	if (!fout.fail())
	{
		for (auto& iter : m_PathList)
		{
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey << L"|" << iter->iCount << L"|" << iter->wstrPath << endl;
		}

		fout.close();	// �� �Լ��� ���� ȣ������ �ʾƵ� �ȴ�.
	}

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

}


void CPathFInd::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	wifstream		fin;
	fin.open(L"../Data/ImgPath.txt");

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring		wstrCombined = L"";
		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_ListBox.AddString(wstrCombined.c_str());

		}
		fin.close();
	}
	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFInd::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";
	
	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathList);
	}

	m_ListBox.ResetContent();

	wstring		wstrCombine = L"";
	TCHAR		szBuf[MIN_STR] = L"";

	for(auto& iter : m_PathList)
	{
		// _itow_s : ������ WSTR�� ��ȯ
		// 3���� : 10������ ��ȯ�ϰڴٴ� �ǹ�
		_itow_s(iter->iCount, szBuf, 10);

		// ������(TOKEN) : ��輱
		wstrCombine = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;

		m_ListBox.AddString(wstrCombine.c_str());
	}
	
	UpdateData(FALSE);
}