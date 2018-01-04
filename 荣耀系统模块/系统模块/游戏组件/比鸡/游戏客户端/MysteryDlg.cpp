// MysteryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MysteryDlg.h"

// CMysteryDlg 对话框

IMPLEMENT_DYNAMIC(CMysteryDlg, CDialog)

//构造函数
CMysteryDlg::CMysteryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMysteryDlg::IDD, pParent)
{
	m_wBankerUser = INVALID_CHAIR;
	m_wMyChairID = INVALID_CHAIR;
	m_pSinkWindow = NULL;
	ZeroMemory(m_bHaveUser, sizeof(m_bHaveUser));
}

//析构函数
CMysteryDlg::~CMysteryDlg()
{
}

//控件绑定
void CMysteryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox[0]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBox[1]);
	DDX_Control(pDX, IDC_CHECK3, m_CheckBox[2]);
	DDX_Control(pDX, IDC_CHECK4, m_CheckBox[3]);
}

BEGIN_MESSAGE_MAP(CMysteryDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
END_MESSAGE_MAP()

//初始化函数
BOOL CMysteryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("请选择要控制为胜利的玩家。"));

	return TRUE;
}

//确定消息
void CMysteryDlg::OnOK()
{
	//发送消息
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	 if (pGameFrameEngine!=NULL) pGameFrameEngine->PostMessage(IDM_MYSTERY);

	CDialog::OnOK();
}

//设置窗口
void CMysteryDlg::SetSinkWindow(CWnd * pSinkWindow)
{
	//设置变量
	m_pSinkWindow = pSinkWindow;

	return;
}

//设置窗口
void CMysteryDlg::SetMysteryInfo(tagMysteryInfo * pTagMysteryInfo)
{
	//清空窗口
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory(m_bHaveUser, sizeof(m_bHaveUser));
	for (int i = 0; i < GAME_PLAYER; i++)
		m_strUserAccount[i] = _T("");

	if (pTagMysteryInfo != NULL)
	{
		//设置庄家
		m_wBankerUser = pTagMysteryInfo->wBankerUser;
		m_wMyChairID = pTagMysteryInfo->wMyChairID;
		memcpy(m_bHaveUser, pTagMysteryInfo->bHaveUser, sizeof(m_bHaveUser));

		if (m_wBankerUser >= GAME_PLAYER || !m_bHaveUser[m_wBankerUser])
			m_wBankerUser = INVALID_CHAIR;

		if (m_wMyChairID >= GAME_PLAYER || !m_bHaveUser[m_wMyChairID])
			m_wMyChairID = INVALID_CHAIR;

		//设置名称
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bHaveUser[i])
			{
				if (m_wBankerUser == i)
					m_strUserAccount[i].Format(_T("庄 %s"), pTagMysteryInfo->szAccounts[i]);
				else
					m_strUserAccount[i].Format(_T("%s"), pTagMysteryInfo->szAccounts[i]);
			}			
		}
	}

	//清理窗口
	for (int i = 0; i < 4; i++)
	{
		m_CheckBox[i].SetWindowText(_T(""));
		m_CheckBox[i].EnableWindow(FALSE);
		m_CheckBox[i].SetCheck(BST_UNCHECKED);
	}

	//选择自己
	if (m_wMyChairID != INVALID_CHAIR)
		m_CheckBox[m_wMyChairID].SetCheck(BST_CHECKED);

	//更新窗口
	UpdateDlg();
}

//更新窗口
void CMysteryDlg::UpdateDlg()
{
	//定义变量
	bool bBankerCheck = false;//(m_wBankerUser!=INVALID_CHAIR && m_CheckBox[m_wBankerUser].GetCheck() == BST_CHECKED);

	//设置按钮
	for (int i = 0; i < 4; i++)
	{
		if (m_bHaveUser[i])
		{
			m_CheckBox[i].SetWindowText(m_strUserAccount[i]);
			if (!bBankerCheck || m_wBankerUser == i)
				m_CheckBox[i].EnableWindow(TRUE);
			else
				m_CheckBox[i].EnableWindow(FALSE);
		}
		else
		{
			m_CheckBox[i].EnableWindow(FALSE);
		}
	}

	UpdateData(FALSE);
}

//获取信息
bool CMysteryDlg::GetControlInfo(bool bControl[])
{
	//定义变量
	bool bBankerCheck = (m_wBankerUser!=INVALID_CHAIR && m_CheckBox[m_wBankerUser].GetCheck() == BST_CHECKED);
	bool bHaveMysteryUser = false;

	ZeroMemory(bControl, sizeof(bool)*GAME_PLAYER);

	if (bBankerCheck)
	{
		bControl[m_wBankerUser] = true;
		bHaveMysteryUser = true;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_wBankerUser == i)	continue;

			bControl[i] = (m_CheckBox[i].GetCheck() == BST_CHECKED);

			if (bControl[i])	bHaveMysteryUser = true;
		}
	}

	return bHaveMysteryUser;
}

//选择按钮
void CMysteryDlg::OnBnClickedCheck1()
{
	UpdateDlg();
}

//选择按钮
void CMysteryDlg::OnBnClickedCheck2()
{
	UpdateDlg();
}

//选择按钮
void CMysteryDlg::OnBnClickedCheck3()
{
	UpdateDlg();
}

//选择按钮
void CMysteryDlg::OnBnClickedCheck4()
{
	UpdateDlg();
}
