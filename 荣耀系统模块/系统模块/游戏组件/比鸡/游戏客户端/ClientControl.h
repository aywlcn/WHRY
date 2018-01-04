#pragma once

#include "stdafx.h"
#include "resource.h"

#define IDM_ADMIN_COMMDN WM_USER+1000
// CClientControl 对话框

class CClientControl : public CDialog
{
	DECLARE_DYNAMIC(CClientControl)

public:
	CComboBox*			m_BxUserLise;		//用户列表
public:
	CClientControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADMIN };

public:
	void AddUserBx(CString str);
	void DeleteUserBx(CString str);
	void PromptMessage(CString strName,bool bWinLoss,BYTE cbCount);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWin();
	afx_msg void OnBnClickedLoss();
	afx_msg void OnBnClickedCancleCentrol();
	afx_msg void OnClose();

DECLARE_MESSAGE_MAP()
};
