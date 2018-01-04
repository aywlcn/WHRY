#pragma once
#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define		IDM_MYSTERY		(WM_USER+1000)		//控制消息

//////////////////////////////////////////////////////////////////////////

//控制信息
struct tagMysteryInfo
{
	bool bHaveUser[GAME_PLAYER];				//用户状态
	WORD wBankerUser;							//庄家位置
	WORD wMyChairID;							//自己位置
	TCHAR szAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//用户名称
};

//////////////////////////////////////////////////////////////////////////

//输赢控制
class CMysteryDlg : public CDialog
{
	DECLARE_DYNAMIC(CMysteryDlg)

	//变量定义
protected:
	bool	m_bHaveUser[GAME_PLAYER];			//用户状态
	WORD	m_wBankerUser;						//庄家用户
	WORD	m_wMyChairID;						//自己位置
	CString m_strUserAccount[GAME_PLAYER];		//用户名称
	CWnd	*m_pSinkWindow;						//通信窗口

	//控件变量
protected:
	CButton	m_CheckBox[4];						//单选按钮
	
public:
	//构造函数
	CMysteryDlg(CWnd* pParent = NULL);   
	//析构函数
	virtual ~CMysteryDlg();

	//对话框数据
	enum { IDD = IDD_MYSTERY };

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange* pDX);  
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK();

	//功能函数
public:
	//设置窗口
	void SetSinkWindow(CWnd * pSinkWindow);
	//设置窗口
	void SetMysteryInfo(tagMysteryInfo * pTagMysteryInfo);
	//更新窗口
	void UpdateDlg();
	//获取信息
	bool GetControlInfo(bool bControl[]);

	//消息映射
protected:
	//选择按钮
	afx_msg void OnBnClickedCheck1();
	//选择按钮
	afx_msg void OnBnClickedCheck2();
	//选择按钮
	afx_msg void OnBnClickedCheck3();
	//选择按钮
	afx_msg void OnBnClickedCheck4();

	DECLARE_MESSAGE_MAP()
};
