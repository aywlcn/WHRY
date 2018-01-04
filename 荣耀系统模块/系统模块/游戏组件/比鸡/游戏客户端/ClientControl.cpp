// ClientControl.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientControl.h"


// CClientControl 对话框

IMPLEMENT_DYNAMIC(CClientControl, CDialog)
CClientControl::CClientControl(CWnd* pParent /*=NULL*/)
	: CDialog(CClientControl::IDD, pParent)
{
	//m_BxUserLise = new CComboBox();
}

CClientControl::~CClientControl()
{
}

void CClientControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientControl, CDialog)
	ON_BN_CLICKED(ID_WIN, OnBnClickedWin)
	ON_BN_CLICKED(ID_LOSS, OnBnClickedLoss)
	ON_BN_CLICKED(ID_CANCLE_CENTROL, OnBnClickedCancleCentrol)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CClientControl 消息处理程序

void CClientControl::OnBnClickedWin()
{
	CMD_C_ControlData ControlData;

	ControlData.bWinloss = true;
	ControlData.cbCount = ((BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->GetCurSel())+1;
	CString strUserName;
	((CComboBox*)GetDlgItem(IDC_COMBO_USER))->GetWindowText(strUserName);
	_sntprintf(ControlData.chUserName, sizeof(ControlData.chUserName), TEXT("%s"),strUserName.GetBuffer(NULL));

	if(ControlData.cbCount<=0)
	{
		AfxMessageBox(TEXT("请选择受控次数!"));
		return;
	}
	if(strUserName.GetLength()==0)
	{
		AfxMessageBox(TEXT("请选择受控玩家!"));
		return;
	}
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlData,0);
	CString str;
	if(strUserName.GetLength()>6)
	{
		strUserName = strUserName.Left(6);
		strUserName += TEXT("...");
	}
	str.Format(TEXT("控制[%s](赢) ,次数:%d"),strUserName,ControlData.cbCount);
	SetWindowText(str);
}

void CClientControl::OnBnClickedLoss()
{
	CMD_C_ControlData ControlData;

	ControlData.bWinloss = false;
	ControlData.cbCount = ((BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->GetCurSel())+1;
	CString strUserName;
	((CComboBox*)GetDlgItem(IDC_COMBO_USER))->GetWindowText(strUserName);
	_sntprintf(ControlData.chUserName, sizeof(ControlData.chUserName), TEXT("%s"),strUserName.GetBuffer(NULL));

	if(ControlData.cbCount<=0)
	{
		AfxMessageBox(TEXT("请选择受控次数!"));
		return;
	}
	if(strUserName.GetLength()==0)
	{
		AfxMessageBox(TEXT("请选择受控玩家!"));
		return;
	}
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlData,0);
	CString str;
	if(strUserName.GetLength()>6)
	{
		strUserName = strUserName.Left(6);
		strUserName += TEXT("...");
	}
	str.Format(TEXT("控制[%s](输) ,次数:%d"),strUserName,ControlData.cbCount);
	SetWindowText(str);
}

void CClientControl::OnBnClickedCancleCentrol()
{
	CMD_C_ControlData ControlData;
	ZeroMemory(&ControlData,sizeof(ControlData));
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlData,0);
	SetWindowText(TEXT(""));
}

BOOL CClientControl::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COUNT))->AddString(TEXT("5"));

	//m_BxUserLise = ((CComboBox*)GetDlgItem(IDC_COMBO_USER));

	return TRUE;
}

void CClientControl::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CClientControl::AddUserBx(CString str)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_USER))->AddString(str);
}

void CClientControl::DeleteUserBx(CString str)
{
	CString strUserName;
	for (WORD wIndex=0; wIndex<GAME_PLAYER; wIndex++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_USER))->GetLBText(wIndex,strUserName);
		if(str.Compare(strUserName) == 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_USER))->DeleteString(wIndex);

			return;
		}
	}
}

void CClientControl::PromptMessage(CString strName,bool bWinLoss,BYTE cbCount)
{
	if(cbCount>0)
	{
		//CStringW str2(strName);
		if(strName.GetLength()>6)
		{
			strName = strName.Left(6);
			strName += TEXT("...");
		}
		CString strPrompt;
		if(bWinLoss)
			strPrompt.Format(TEXT("控制[%s](赢) ,次数:%d"),strName,cbCount);
		else
			strPrompt.Format(TEXT("控制[%s](输) ,次数:%d"),strName,cbCount);
		SetWindowText(strPrompt);
	}
	else
		SetWindowText(TEXT(""));
}
